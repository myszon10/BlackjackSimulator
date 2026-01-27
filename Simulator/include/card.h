#pragma once
#include <iostream>

enum Suit { Hearts, Diamonds, Clubs, Spades };

enum Rank { Two = 2, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };

class Card {
public:
	Card(Suit suit, Rank rank);
	int GetCardValue();
	Rank GetRank();
	friend std::ostream& operator<<(std::ostream& _stream, const Card& c);
private:
	Suit _suit;
	Rank _rank;
	int _value; // blackjack value of the card
	bool _isRed;
};