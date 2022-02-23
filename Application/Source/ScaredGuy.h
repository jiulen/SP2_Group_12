#ifndef SCARED_GUY_H
#define SCARED_GUY_H

#include "Entity.h"
#include "Vector3.h"
#include "Structs.h"
#include <vector>
class ScaredGuy : public Entity
{
protected:
	float DistBetweenPoints(float x1, float z1, float x2, float z2);
public:
	ScaredGuy(float facing, Vector3 pos, Vector3 direction);
	~ScaredGuy();
	virtual void alert(std::vector<Entity*>& entities);
	virtual void takedamage(int a);
	virtual void move(Vector3 playerPos, float dt, std::vector<Hitbox> hitboxes, std::vector<Entity*> entities, Hitbox playerHitbox);
	virtual void attack(Vector3, float, Player&, float);
};

#endif