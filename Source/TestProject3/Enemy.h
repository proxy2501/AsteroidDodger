#pragma once
class Enemy
{
public:
	Enemy(int x, int y);
	~Enemy();
	int GetX();
	int GetY();
	virtual void Move();

protected:
	int* x;
	int* y;
};

