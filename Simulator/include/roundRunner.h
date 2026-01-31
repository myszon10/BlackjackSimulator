#pragma once
#include "interfaces.h"
#include "playerSession.h"
#include "gameEngine.h"

class RoundRunner {
public:
	RoundRunner(
		IActionPolicy& actionDecider, 
		IBettingPolicy& betDecider, 
		int startingBalance, 
		Shoe& shoe,
		CounterEvents* events = nullptr);

	void PlayRounds(int numberOfRounds, int minBalanceStop);

	void DisplayRoundStatistics(RoundState state, RoundResult result, int roundNumber);
private:
	IActionPolicy& _actionDecider;
	IBettingPolicy& _betDecider;
	PlayerSession _session;
	GameEngine _engine;
	Shoe& _shoe;
	CounterEvents* _events;
};