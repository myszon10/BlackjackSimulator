#include "multiSim.h"
#include <thread>
#include <mutex>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#ifdef _WIN32
#include <windows.h>
#endif
#include "rules.h"
#include "shoe.h"
#include "gameEngine.h"
#include "playerSession.h"
#include "playerBasicStrategy.h"
#include "playerCardCounting.h"
#include "playerWhile17.h"
#include "playerUntil12.h"
#include "playerRandom.h"
#include "HiLoCounter.h"

template <typename T>
static T clampT(T v, T lo, T hi) {
    return (v < lo) ? lo : (v > hi) ? hi : v;
}

static const char* StrategyName(StrategyId id) {
    switch (id) {
    case StrategyId::Basic: 
        return "Basic strategy";
    case StrategyId::Counter: 
        return "Hi-Lo card counter";
    case StrategyId::Hit17: 
        return "Hit while <= 17";
    case StrategyId::Hit12:   
        return "Hit while <= 12";
    case StrategyId::Random:  
        return "Random actions";
    default: 
        return "Unknown";
    }
}

static void ClearConsole() {
#ifdef _WIN32
    system("cls");
#else
    std::cout << "\x1B[2J\x1B[H";
#endif
}

static void MoveCursorHome() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { 0, 0 };
    SetConsoleCursorPosition(hOut, coord);
#else
    std::cout << "\x1B[H";
#endif
}

static std::string Bar(double pct, int width = 30) {
    pct = clampT(pct, 0.0, 1.0);
    int filled = (int)std::round(pct * width);
    std::string s;
    s.reserve(width);
    s.append(filled, '#');
    s.append(width - filled, ' ');
    return s;
}

struct WorkerResult {
    StrategyId id;
    StrategyStats stats;
};

static WorkerResult RunWorker(
    StrategyId id,
    long long roundsTarget,
    int startingBalance,
    int minBalanceStop,
    ProgressState& progress
) {
    Shoe shoe(Rules::NumberOfDecks);
    GameEngine engine;
    PlayerSession session;
    session.SetPlayerBalance(startingBalance);
    RoundState state;

    HiLoCounter counter;
    CounterEvents* events = nullptr;

    std::unique_ptr<IActionPolicy> action;
    std::unique_ptr<IBettingPolicy> bets;

    switch (id) {
    case StrategyId::Basic:
        action = std::make_unique<BasicStrategyActions>();
        bets = std::make_unique<BasicStrategyBets>(startingBalance);
        break;

    case StrategyId::Counter:
        action = std::make_unique<CardCounterActions>(counter, shoe);
        bets = std::make_unique<CardCounterBets>(startingBalance, counter, shoe);
        events = &counter;
        break;

    case StrategyId::Hit17:
        action = std::make_unique<Player17Actions>();
        bets = std::make_unique<Player17Bets>(startingBalance);
        break;

    case StrategyId::Hit12:
        action = std::make_unique<Player12Actions>();
        bets = std::make_unique<Player12Bets>(startingBalance);
        break;

    case StrategyId::Random:
        action = std::make_unique<PlayerRandomActions>();
        bets = std::make_unique<PlayerRandomBets>(startingBalance);
        break;
    }

    StrategyStats st;
    st.name = StrategyName(id);

    for (long long i = 0; i < roundsTarget; i++) {
        if (session.GetPlayerBalance() <= minBalanceStop) break;

        RoundResult r = engine.playRound(state, shoe, *action, *bets, session, events);

        session.UpdatePlayerBalance(r.unitsWon);

        st.rounds++;
        st.totalBet += r.unitsBet;
        st.totalReturn += r.unitsWon;

        progress.done[(int)id].fetch_add(1, std::memory_order_relaxed);
    }

    return WorkerResult{ id, st };
}

static void ProgressUIThread(const ProgressState& progress) {
    using namespace std::chrono_literals;

    // Draw a static header once, then overwrite from home.
    ClearConsole();

    while (!progress.stop.load(std::memory_order_relaxed)) {
        MoveCursorHome();

        std::cout << "Running simulations (2 threads per strategy)...\n\n";

        for (int i = 0; i < ProgressState::STRATS; i++) {
            long long done = progress.done[i].load(std::memory_order_relaxed);
            long long target = progress.target[i];
            double pct = target > 0 ? (double)done / (double)target : 1.0;
            pct = clampT(pct, 0.0, 1.0);

            std::cout << std::left << std::setw(22) << StrategyName((StrategyId)i)
                << " [" << Bar(pct, 28) << "] "
                << std::fixed << std::setprecision(2) << (pct * 100.0) << "% "
                << "(" << done << "/" << target << ")\n";
        }

        std::cout << "\n(Press Ctrl+C to abort)\n";

        std::this_thread::sleep_for(100ms);
    }
}

std::vector<StrategyStats> RunMultiStrategySimulation(const MultiSimConfig& cfg) {
    ProgressState progress;

    for (int i = 0; i < ProgressState::STRATS; i++) {
        progress.target[i] = cfg.roundsPerStrategy;
    }

    // UI thread
    std::thread ui([&] { ProgressUIThread(progress); });

    // Worker threads
    std::vector<std::thread> workers;
    std::mutex resultsMutex;
    std::vector<WorkerResult> results;
    results.reserve(ProgressState::STRATS * cfg.threadsPerStrategy);

    auto launchTwoWorkers = [&](StrategyId id) {
        long long total = cfg.roundsPerStrategy;
        int t = cfg.threadsPerStrategy;

        // split rounds across t threads (handles odd totals)
        for (int k = 0; k < t; k++) {
            long long start = (total * k) / t;
            long long end = (total * (k + 1)) / t;
            long long share = end - start;

            workers.emplace_back([&, id, share] {
                WorkerResult wr = RunWorker(id, share, cfg.startingBalance, cfg.minBalanceStop, progress);
                std::lock_guard<std::mutex> lock(resultsMutex);
                results.push_back(std::move(wr));
                });
        }
        };

    launchTwoWorkers(StrategyId::Basic);
    launchTwoWorkers(StrategyId::Counter);
    launchTwoWorkers(StrategyId::Hit17);
    launchTwoWorkers(StrategyId::Hit12);
    launchTwoWorkers(StrategyId::Random);

    for (auto& th : workers) th.join();

    // stop UI and join
    progress.stop.store(true, std::memory_order_relaxed);
    ui.join();

    // Aggregate per strategy
    std::vector<StrategyStats> aggregated(ProgressState::STRATS);
    for (int i = 0; i < ProgressState::STRATS; i++) aggregated[i].name = StrategyName((StrategyId)i);

    for (auto& wr : results) {
        StrategyStats& a = aggregated[(int)wr.id];
        a.rounds += wr.stats.rounds;
        a.totalBet += wr.stats.totalBet;
        a.totalReturn += wr.stats.totalReturn;
    }

    return aggregated;
}