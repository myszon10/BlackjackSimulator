#pragma once
#include "counterEvents.h"

class HiLoCounter : public CounterEvents {
public:
	HiLoCounter();
	void OnReshuffle() override;
	void OnCardRevealed(Card& card) override;
	int RunningCount() const { return _running; }
	double TrueCount(int decksRemaining) const;
private:
	int _running = 0;
	int _cardsSeen = 0;

	static int HiLoValue(Card& card);
};