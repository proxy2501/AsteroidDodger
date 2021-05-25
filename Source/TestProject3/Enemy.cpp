#include "Enemy.h"

Enemy::Enemy(int x, int y)
{
	this->x = new int(x);
	this->y = new int(y);
}

Enemy::~Enemy()
{
	delete x;
	delete y;
}

int Enemy::GetX()
{
	return *x;
}

int Enemy::GetY()
{
	return *y;
}

void Enemy::Move()
{
	// Define how enemy moves down the screen.
	(*y)++;
}
