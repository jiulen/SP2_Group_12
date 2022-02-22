#ifndef BOSS_H
#define BOSS_H

#include "Enemy.h"
#include "Vector3.h"
#include "Structs.h"
#include <vector>
class Boss : public Enemy
{
private:

public:
	Boss(float facing, Vector3 pos, Vector3 direction);
	~Boss();
	void takedamage(int a);
	void move(Vector3 playerPos, float dt, std::vector<Hitbox> hitboxes, std::vector<Entity*> entities, Hitbox playerHitbox);
	void attack(Vector3 playerPos, float playerRadius, Player& player, float dt); //do later
};

#endif

