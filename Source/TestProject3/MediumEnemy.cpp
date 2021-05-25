#include "MediumEnemy.h"
#include <stdlib.h>

MediumEnemy::MediumEnemy(int x, int y) : Enemy(x, y)
{
	moveCounter = new int(0);

	// Set initial move direction according to spawn position (on x-axis).
	if (x < 10)
	{
		moveLeft = new bool(false);
	}
	else if (x > 39)
	{
		moveLeft = new bool(true);
	}
	else
	{
		moveLeft = new bool(rand() % 2 == 1);
	}
}


MediumEnemy::~MediumEnemy()
{
	delete moveCounter;
	delete moveLeft;
}

void MediumEnemy::Move()
{
	if (*moveCounter >= 10)
	{
		*moveLeft = !(*moveLeft);
		*moveCounter = 0;
	}

	(*y)++;
	*moveLeft ? (*x)-- : (*x)++;

	(*moveCounter)++;
}
