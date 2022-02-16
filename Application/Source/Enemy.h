#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include "Vector3.h"
#include "Structs.h"
class Enemy : public Entity
{
private:
	bool chase;
	Vector3 directionVector;
	float detectRange, attackRange;
	float DistBetweenPoints(float x1, float z1, float x2, float z2);
public:
	Enemy(int a, int b, float facing, Vector3 pos, Vector3 direction, bool chasing, float detectR, float attackR, float velo);
	~Enemy();
	void move(Vector3 playerPos, float dt);
	void attack(); //do later
	
};

#endif