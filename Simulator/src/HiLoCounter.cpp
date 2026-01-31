#include "HiLoCounter.h"

HiLoCounter::HiLoCounter() : _running(0), _cardsSeen(0) { }

void HiLoCounter::OnCardRevealed(Card& card) {
	_running += HiLoValue(card);
	_cardsSeen++;
}

void HiLoCounter::OnReshuffle() {
	_running = 0;
	_cardsSeen = 0;
}

double HiLoCounter::TrueCount(int decksRemaining) const {
	if (decksRemaining <= 0) {
		return static_cast<double>(_running);
	}
	return static_cast<double>(_running) / static_cast<double>(decksRemaining);
}

int HiLoCounter::HiLoValue(Card& card) {
	const int v = card.GetCardValue();
	const Rank r = card.GetRank();

	// Treat ace as -1
	if (r == Ace) {
		return -1;
	}
	else if (v >= 2 && v <= 6) {
		return 1;
	}
	else if (v >= 7 && v <= 9) {
		return 0;
	}
	else {
		return -1;
	}
}