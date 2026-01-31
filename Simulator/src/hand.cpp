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
	int aces = 0;

	for (Card& c : _cards) {
		value += c.GetCardValue();
		if (c.GetRank() == Ace) {
			aces++;
		}
	}

	// Reduce aces from 11 -> 1 while busting
	while (value > 21 && aces > 0) {
		value -= 10;
		aces--;
	}

	// If any ace remains "unreduced", then we are soft
	isSoft = (aces > 0);

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
	return (CalculateValue() == 17 && isSoft);
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

Card Hand::GetFirstCard() {
	if (_cards.size() == 0) {
		throw runtime_error("No cards to check");
	}
	return _cards[0];
}

Card Hand::PopLastCard() {
	if (_cards.size() == 0) {
		throw runtime_error("No cards to pop");
	}
	Card last = _cards.back();
	_cards.pop_back();
	return last;
}

Card Hand::GetCard(int index) {
	if (index < 0 || index >= (int)_cards.size()) {
		throw runtime_error("Card index out of range");
	}
	return _cards[index];
}