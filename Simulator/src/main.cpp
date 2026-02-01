#include <iostream>
#include <iomanip>
#ifdef _WIN32
#include <windows.h>
#endif
#include "multiSim.h"
using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);

#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif // _WIN32

	MultiSimConfig config;
	config.startingBalance = 1'000'000'000; //1'000'000;
	config.roundsPerStrategy = 2'000'000; //500'000;
	config.minBalanceStop = 0;
	config.threadsPerStrategy = 2;

	auto stats = RunMultiStrategySimulation(config);

#ifdef _WIN32
	system("cls");
#else
	cout << "\x1B[2J\x1B[H";
#endif

	cout << "================== RESULTS ==================\n";
	cout << "Rounds per strategy (target): " << config.roundsPerStrategy << "\n";
	cout << "Starting bankroll per worker: " << config.startingBalance << "\n";
	cout << "Threads per strategy: " << config.threadsPerStrategy << "\n\n";

	cout << left << setw(22) << "Strategy"
		<< right << setw(12) << "Rounds"
		<< right << setw(16) << "Total Bet"
		<< right << setw(16) << "Total Return"
		<< right << setw(16) << "Net"
		<< right << setw(14) << "Avg/Round"
		<< right << setw(12) << "Edge"
		<< "\n";

	cout << string(22 + 12 + 16 + 16 + 16 + 14 + 12, '-') << "\n";

	for (auto& s : stats) {
		cout << left << setw(22) << s.name
			<< right << setw(12) << s.rounds
			<< right << setw(16) << s.totalBet
			<< right << setw(16) << s.totalReturn
			<< right << setw(16) << s.net()
			<< right << setw(14) << fixed << setprecision(3) << s.avgPerRound()
			<< right << setw(11) << fixed << setprecision(3) << (s.edge() * 100.0) << "%\n";
	}

	cout << "\nEdge = Net / TotalBet (profit per unit wagered).\n";
	cout << "Avg/Hand = Net / Rounds.\n";
	cout << "\nSimulation complete.\n";
}