#ifndef BASIC_MELEE_H
#define BASIC_MELEE_H

#include "Enemy.h"
#include "Vector3.h"
#include "Structs.h"
#include <vector>
class BasicMelee : public Enemy
{
private:
	
public:
	BasicMelee(float facing, Vector3 pos, Vector3 direction);
	~BasicMelee();
	void move(Vector3 playerPos, float dt, std::vector<Hitbox> hitboxes, std::vector<Entity*> entities, Hitbox playerHitbox);
	void attack(Vector3 playerPos, float playerRadius, Player& player, float dt); //do later
};

#endif