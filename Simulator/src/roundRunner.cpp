#include "roundRunner.h"
#include "rules.h"
using namespace std;

RoundRunner::RoundRunner(IActionPolicy& actionDecider, IBettingPolicy& betDecider, int startingBalance)
	: _actionDecider(actionDecider), _betDecider(betDecider) {
	_session.SetPlayerBalance(startingBalance);
}

void RoundRunner::DisplayRoundStatistics(RoundState state, RoundResult result, int roundNumber) {
	string winLoss = result.outcome == RoundResult::PlayerWon ? "(won)" :
					 result.outcome == RoundResult::PlayerLost ? "(lost)" :
					 result.outcome == RoundResult::PlayerBlackjack ? "(blackjack)" : "(draw)";

	cout << "===== ROUND " << roundNumber + 1 << " STATISTICS =====" << endl;
	cout << "Dealer's hand: ";
	state.dealerHand.PrintHand();
	cout << "Player's hand: ";
	state.playerHand.PrintHand();
	cout << "--- Player action ---" << endl;
	cout << "Player bet: " << state.bet << " " << winLoss << endl;
	cout << "Player current balance: " << _session.GetPlayerBalance() << endl << endl;
	
}

void RoundRunner::PlayRounds(int numberOfRounds, int minBalanceStop) {
	RoundState s;
	Shoe shoe(Rules::NumberOfDecks); 

	for (int i = 0; i < numberOfRounds; i++) {
		if (_session.GetPlayerBalance() <= minBalanceStop) {
			break;
		}
		
		RoundResult result = _engine.playRound(s, shoe, _actionDecider, _betDecider, _session);

		int netBalance = result.unitsWon - result.unitsBet;
		_session.UpdatePlayerBalance(netBalance);

		this->DisplayRoundStatistics(s, result, i);
	}
}