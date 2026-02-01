// Multithreaded simulation runner
#pragma once
#include <atomic>
#include <string>
#include <vector>
#include <cstdint>


struct StrategyStats {
    std::string name;
    long long rounds = 0;
    long long totalBet = 0;
    long long totalReturn = 0;
    long long net() const { return totalReturn - totalBet; }
    double avgPerRound() const { return rounds ? (double)net() / (double)rounds : 0.0; }
    double edge() const { return totalBet ? (double)net() / (double)totalBet : 0.0; } // profit per unit bet
};

struct ProgressState {
    static constexpr int STRATS = 5;
    std::atomic<long long> done[STRATS];
    long long target[STRATS]{};
    std::atomic<bool> stop{ false };

    ProgressState() {
        for (int i = 0; i < STRATS; i++) done[i] = 0;
    }
};

enum class StrategyId {
    Basic = 0,
    Counter = 1,
    Hit17 = 2,
    Hit12 = 3,
    Random = 4
};

struct MultiSimConfig {
    int startingBalance = 1'000'000;
    long long roundsPerStrategy = 500'000; // TOTAL per strategy (split across 2 threads)
    int minBalanceStop = 0;
    int threadsPerStrategy = 2; 
};

std::vector<StrategyStats> RunMultiStrategySimulation(const MultiSimConfig& cfg);