#pragma once
class Player
{
public:
	Player(int x, int y);
	~Player();
	int GetX();
	void SetX(int value);
	int GetY();
	void SetY(int value);
	bool GetIsAlive();
	void SetIsAlive(bool value);

private:
	int* x;
	int* y;
	bool* isAlive;
};

