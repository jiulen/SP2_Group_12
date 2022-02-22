#ifndef WATCHER_H
#define WATCHER_H

#include "Enemy.h"
#include "Vector3.h"
#include "Structs.h"
#include <vector>
class Watcher : public Enemy
{
private:
public:
	Watcher(float facing, Vector3 pos, Vector3 direction);
	~Watcher();
	void takedamage(int a);
	void alert(std::vector<Entity*>& entities);
	void move(Vector3 playerPos, float dt, std::vector<Hitbox> hitboxes, std::vector<Entity*> entities, Hitbox playerHitbox);
	void attack(Vector3 playerPos, float playerRadius, Player& player, float dt);
};

#endif