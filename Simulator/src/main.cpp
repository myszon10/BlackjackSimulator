#include <iostream>
#include <ctime>
#include "windows.h"
#include "card.h"
#include "deck.h"
#include "shoe.h"
using namespace std;

int main() {
	SetConsoleOutputCP(CP_UTF8);
	srand(time(0));

	Shoe test(2);
	test.PrintShoe();
}