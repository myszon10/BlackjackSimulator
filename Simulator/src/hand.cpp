#include "hand.h"
using namespace std;

Hand::Hand() { }

void Hand::ClearHand() {
	_cards.clear();
}

void Hand::AddCard(Card added) {
	_cards.push_back(added);
}

int Hand::CalculateValue() {
	int value = 0;
	int numberOfAces = 0;

	for (Card c : _cards) {
		if (c.GetRank() == Ace) {
			numberOfAces++;
		}

		value += c.GetCardValue();
	}

	// "Bust" but Aces were treated as 11 -> treat them as 1
	while (value > 21 && numberOfAces > 0) {
		value -= 10;
		numberOfAces--;
	}

	return value;
}

bool Hand::IsBust() {
	return this->CalculateValue() > 21;
}

void Hand::PrintHand() {
	if (_cards.size() > 0) {
		for (int i = 0; i < _cards.size() - 1; i++) {
			cout << _cards[i] << ", ";
		}
		cout << _cards[_cards.size() - 1];
	}
	cout << "\nValue: " << this->CalculateValue() << endl;
}

bool Hand::Soft17() {
	bool hasAces = false;
	int value = 0;

	for (Card c : _cards) {
		if (c.GetRank() == Ace) {
			value++;
			hasAces = true;
		}
		else {
			value += c.GetCardValue();
		}
	}

	if (hasAces && value + 10 == 17) {
		return true;
	}
	return false;
}

int Hand::CardCount() {
	return _cards.size();
}

Card Hand::GetLastCard() {
	if (_cards.size() == 0) {
		throw runtime_error("No cards to check");
	}
	return _cards.back();
}

bool Hand::ShouldHitDealer() {
	int value = this->CalculateValue();
	if (value < 17) {
		return true;
	}
	else if (value == 17 && this->Soft17()) {
		return true;
	}
	return false;
}