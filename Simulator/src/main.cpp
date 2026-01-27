#include <iostream>
#include <ctime>
#include "windows.h"
#include "shoe.h"
#include "hand.h"
using namespace std;

int main() {
	SetConsoleOutputCP(CP_UTF8);
	srand(time(0));

	Hand test;

	test.AddCard(Card(Hearts, Ace));
	test.AddCard(Card(Spades, Ten));
	test.AddCard(Card(Diamonds, Queen));
	test.AddCard(Card(Clubs, Two));

	test.PrintHand();
}