#pragma once
#include <iostream>

enum Suit { Hearts, Diamonds, Clubs, Spades };
enum Rank { Two = 2, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };

static Suit suits[] = { Hearts, Diamonds, Clubs, Spades };
static Rank ranks[] = { Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };
 
const int RANK_COUNT = 13;
const int SUIT_COUNT = 4;

class Card {
public:
	Card();
	Card(Suit suit, Rank rank);
	int GetCardValue();
	Rank GetRank();
	friend std::ostream& operator<<(std::ostream& _stream, const Card& c);
	bool IsAce();
	Card operator=(const Card& other);
private:
	Suit _suit;
	Rank _rank;
	int _value; // blackjack value of the card
	bool _isRed;
};