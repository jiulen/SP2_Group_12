#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include "Vector3.h"
#include "Structs.h"
#include <vector>
class Enemy : public Entity
{
protected:
	float DistBetweenPoints(float x1, float z1, float x2, float z2);
public:
	Enemy();
	~Enemy();
	virtual void alert(std::vector<Entity*>& entities);
	virtual void takedamage(int a);
	virtual void move(Vector3 playerPos, float dt, std::vector<Hitbox> hitboxes, std::vector<Entity*> entities, Hitbox playerHitbox);
	virtual void attack(Vector3, float, Player&, float);
};

#endif