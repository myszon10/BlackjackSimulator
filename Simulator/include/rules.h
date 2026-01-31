#pragma once

namespace Rules {
	constexpr int NumberOfDecks = 6;
	constexpr double ReshuffleWhenPlayed = 0.75; // reshuffle shoe if 75% of the shoe was played
	constexpr double BlackjackPayout = 1.5; // Blackjack pays 3:2
	constexpr int playerR = 10; // betting variation range (17/12/random players)
	constexpr int playerD = 1000; // 1/playerD = base bet for initial bankroll
	constexpr double randomDoubleDownChance = 0.2; // chance to double down randomly (random player)
	constexpr double randomHitChance = 0.5; // chance to hit randomly (random player)
	constexpr int maxHands = 4; // maximum number of hands after splits
};