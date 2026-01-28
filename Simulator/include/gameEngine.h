#pragma once
#include "actionRules.h"
#include "payoutCalculator.h"
#include "shoe.h"
#include "interfaces.h"
#include "playerSession.h"

class GameEngine {
public:
	void ApplyPlayerAction(RoundState& s, Action a, Shoe& shoe, int& totalBet);
	RoundResult playRound(
		RoundState& s, 
		Shoe& shoe, 
		IActionPolicy& actionDecider, 
		IBettingPolicy& betDecider,
		PlayerSession& session);
};