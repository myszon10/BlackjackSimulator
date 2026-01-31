#pragma once
#include "actionRules.h"
#include "payoutCalculator.h"
#include "shoe.h"
#include "interfaces.h"
#include "playerSession.h"
#include "counterEvents.h"

class GameEngine {
public:
	void ApplyPlayerAction(RoundState& s, Action a, Shoe& shoe, int& totalBet, PlayerSession& session, CounterEvents* events);
	
	RoundResult playRound(
		RoundState& s, 
		Shoe& shoe, 
		IActionPolicy& actionDecider, 
		IBettingPolicy& betDecider,
		PlayerSession& session,
		CounterEvents* events);
};