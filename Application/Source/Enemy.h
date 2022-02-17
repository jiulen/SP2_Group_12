#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include "Vector3.h"
#include "Structs.h"
#include <vector>
class Enemy : public Entity
{
protected:
	bool chase;
	Vector3 directionVector;
	float detectRange, attackRange;
	float DistBetweenPoints(float x1, float z1, float x2, float z2);

	virtual void checkCollisionStructures(std::vector<Hitbox> hitboxes);
public:
	Enemy();
	~Enemy();
	virtual void move(Vector3 playerPos, float dt, std::vector<Hitbox> hitboxes);
	virtual void attack();
};

#endif