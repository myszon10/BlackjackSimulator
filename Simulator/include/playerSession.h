#pragma once

class PlayerSession {
public:
	int GetPlayerBalance() const {
		return _playerBalance;
	}
	void SetPlayerBalance(int amount) {
		_playerBalance = amount;
	}
	void UpdatePlayerBalance(int amount) {
		_playerBalance += amount;
	}
private:
	int _playerBalance;
};