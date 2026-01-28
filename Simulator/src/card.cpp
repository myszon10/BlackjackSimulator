#include "card.h"
using namespace std;

Card::Card() { 
	_suit = Hearts;
	_rank = Ace;
	_isRed = true;
	_value = 11;
}

Card::Card(Suit suit, Rank rank) {
	_suit = suit;
	_rank = rank;

	_isRed = (suit == Hearts || suit == Diamonds);

	switch (rank) {
	case Ace:
		_value = 11;
		break;
	case King:
	case Queen:
	case Jack:
		_value = 10;
		break;
	default:
		_value = (int)rank;
		break;
	}
}

int Card::GetCardValue() { return _value; }

Rank Card::GetRank() { return _rank; }

ostream& operator<<(ostream& _stream, Card const& c) {
	if (c._isRed) {
		_stream << "\x1B[31m";
	}
	
	switch (c._rank) {
	case Ace:
		_stream << "A";
		break;
	case Jack:
		_stream << "J";
		break;
	case Queen:
		_stream << "Q";
		break;
	case King:
		_stream << "K";
		break;
	default:
		_stream << static_cast<int>(c._rank);
		break;
	}

	switch (c._suit) {
	case Hearts:
		_stream << u8"♥";
		break;
	case Diamonds:
		_stream << u8"♦";
		break;
	case Spades:
		_stream << u8"♠";
		break;
	case Clubs:
		_stream << u8"♣";
		break;
	}

	if (c._isRed) {
		_stream << "\x1B[37m";
	}

	return _stream;
}

bool Card::IsAce() {
	return _rank == Ace;
}

Card Card::operator=(const Card& other) {
	_suit = other._suit;
	_rank = other._rank;
	_value = other._value;
	_isRed = other._isRed;
	return *this;
}
