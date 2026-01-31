#include "roundRunner.h"
#include "rules.h"
#include "counterEvents.h"
#include <iostream>
#include <iomanip>
using namespace std;

RoundRunner::RoundRunner(
	IActionPolicy& actionDecider, 
	IBettingPolicy& betDecider, 
	int startingBalance,
	Shoe& shoe,
	CounterEvents* events
)
	: _actionDecider(actionDecider), 
	  _betDecider(betDecider),
	  _shoe(shoe),
	  _events(events) 
{
	_session.SetPlayerBalance(startingBalance);
}

static string OutcomeToString(RoundResult::Outcome o) {
	switch (o) {
	case RoundResult::PlayerWon: 
		return "(won)";
	case RoundResult::PlayerLost:      
		return "(lost)";
	case RoundResult::PlayerBlackjack: 
		return "(blackjack)";
	case RoundResult::Draw:            
		return "(draw)";
	default:                           
		return "(?)";
	}
}

void RoundRunner::DisplayRoundStatistics(RoundState state, RoundResult result, int roundNumber) {
	cout << "===== ROUND " << roundNumber + 1 << " STATISTICS =====\n";

	cout << "Dealer's hand: ";
	state.dealerHand.PrintHand();

	// Print all player hands
	for (size_t i = 0; i < state.playerHands.size(); i++) {
		cout << "Player hand #" << (i + 1);

		if ((int)i == state.activeHand) cout << " (active)";

		if (state.handBlackjack.size() == state.playerHands.size() && state.handBlackjack[i]) {
			cout << " [BJ]";
		}

		cout << " | bet: " << (state.handBets.size() == state.playerHands.size() ? state.handBets[i] : 0) << "\n";
		state.playerHands[i].PrintHand();
	}

	cout << "Round bet total: " << result.unitsBet << "\n";
	cout << "Round returned: " << result.unitsWon << " " << OutcomeToString(result.outcome) << "\n";
	cout << "Round profit: " << (result.unitsWon - result.unitsBet) << "\n";
	cout << "Player balance: " << _session.GetPlayerBalance() << "\n\n";
}

void RoundRunner::PlayRounds(int numberOfRounds, int minBalanceStop) {
	RoundState s;

	long long totalBet = 0;
	long long totalReturn = 0;
	int roundsPlayed = 0;
	long long startingBalance = _session.GetPlayerBalance();

	for (int i = 0; i < numberOfRounds; i++) {
		if (_session.GetPlayerBalance() <= minBalanceStop) {
			break;
		}
		
		RoundResult result = _engine.playRound(s, _shoe, _actionDecider, _betDecider, _session, _events);

		_session.UpdatePlayerBalance(result.unitsWon);

		totalBet += result.unitsBet;
		totalReturn += result.unitsWon;
		roundsPlayed++;

		//this->DisplayRoundStatistics(s, result, i);
	}

	long long finalBalance = _session.GetPlayerBalance();
	long long net = finalBalance - startingBalance;

	cout << "===== SESSION SUMMARY =====\n";
	cout << "Rounds played: " << roundsPlayed << "\n";
	cout << "Total bet: " << totalBet << "\n";
	cout << "Total returned: " << totalReturn << "\n";
	cout << "Net profit: " << net << "\n";
	if (roundsPlayed > 0) {
		double avgPerRound = (double)net / (double)roundsPlayed;
		cout << "Avg / round: " << fixed << setprecision(2) << avgPerRound << "\n";
	}
	cout << "Final balance: " << finalBalance << "\n\n";
}