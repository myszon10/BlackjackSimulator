#include <iostream>
#include <ctime>
#include "windows.h"
#include "shoe.h"
#include "hand.h"
#include "playerWhile17.h"
#include "playerUntil12.h"
#include "playerBasicStrategy.h"
#include "roundRunner.h"
using namespace std;

int main() {
	SetConsoleOutputCP(CP_UTF8);
	//srand(time(0));
	srand(2);

	const int startingBalance = 10000;
	const int numberOfRounds = 5;
	const int minBalanceStop = 0;

	BasicStrategyActions actionPolicy;
	BasicStrategyBets bettingPolicy(startingBalance);

	RoundRunner runner(actionPolicy, bettingPolicy, startingBalance);

	runner.PlayRounds(numberOfRounds, minBalanceStop);

	cout << endl << "Simulation complete." << endl;
}