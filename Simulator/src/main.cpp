#include <iostream>
#include <ctime>
#include "windows.h"
#include "shoe.h"
#include "hand.h"
#include "playerWhile17.h"
#include "playerUntil12.h"
#include "playerBasicStrategy.h"
#include "playerCardCounting.h"
#include "playerRandom.h"
#include "roundRunner.h"
#include "HiLoCounter.h"
using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);

	SetConsoleOutputCP(CP_UTF8);
	srand(time(0));
	//srand(2);

	const int startingBalance = 1000000;
	const int numberOfRounds = 500000;
	const int minBalanceStop = 0;

	Shoe shoe(Rules::NumberOfDecks);
	HiLoCounter counter;

	/*PlayerRandomActions actionPolicy;
	PlayerRandomBets bettingPolicy(startingBalance);*/

	/*BasicStrategyActions actionPolicy;
	BasicStrategyBets bettingPolicy(startingBalance);*/

	CardCounterActions actionPolicy(counter, shoe);
	CardCounterBets bettingPolicy(startingBalance, counter, shoe);

	RoundRunner runner(actionPolicy, bettingPolicy, startingBalance, shoe, &counter);

	runner.PlayRounds(numberOfRounds, minBalanceStop);

	cout << endl << "Simulation complete." << endl;
}