#pragma once
#include "card.h"

struct CounterEvents {
	virtual ~CounterEvents() = default;
	virtual void OnReshuffle() = 0;
	virtual void OnCardRevealed(Card& card) = 0;
};