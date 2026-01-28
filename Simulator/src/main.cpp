#include <iostream>
#include <ctime>
#include "windows.h"
#include "shoe.h"
#include "hand.h"
#include "playerWhile17.h"
#include "roundRunner.h"
using namespace std;

int main() {
	SetConsoleOutputCP(CP_UTF8);
	//srand(time(0));
	srand(2);

	const int startingBalance = 10000;
	const int numberOfRounds = 10;
	const int minBalanceStop = 0;

	Player17Actions actionPolicy;
	Player17Bets bettingPolicy(startingBalance);

	RoundRunner runner(actionPolicy, bettingPolicy, startingBalance);

	runner.PlayRounds(numberOfRounds, minBalanceStop);

	cout << endl << "Simulation complete." << endl;
}