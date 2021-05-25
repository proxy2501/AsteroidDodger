#pragma once
#include "Enemy.h"

class MediumEnemy : public Enemy // (numba... two?)
{
public:
	MediumEnemy(int x, int y);
	~MediumEnemy();
	void Move();

private:
	int* moveCounter;
	bool* moveLeft;
};

