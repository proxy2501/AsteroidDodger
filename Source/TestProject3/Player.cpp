#include "Player.h"

Player::Player(int x, int y)
{
	this->x = new int(x);
	this->y = new int(y);
	isAlive = new bool(true);
}

Player::~Player()
{
	delete x;
	delete y;
	delete isAlive;
}

int Player::GetX()
{
	return *x;
}

void Player::SetX(int value)
{
	*x = value;
}

int Player::GetY()
{
	return *y;
}

void Player::SetY(int value)
{
	*y = value;
}

bool Player::GetIsAlive()
{
	return *isAlive;
}

void Player::SetIsAlive(bool value)
{
	*isAlive = value;
}
