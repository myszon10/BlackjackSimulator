#include <iostream>
#include <ctime>
#include "windows.h"
#include "card.h"
#include "deck.h"
using namespace std;

int main() {
	SetConsoleOutputCP(CP_UTF8);
	srand(time(0));

	Deck test;

	test.PrintDeck();
	cout << endl << "---" << endl << endl;
	test.Shuffle();
	test.PrintDeck();
}