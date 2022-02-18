#include "Enemy.h"
float Enemy::DistBetweenPoints(float x1, float z1, float x2, float z2)
{
	float distX = x1 - x2;
	float distZ = z1 - z2;
	float dist = 0.0f;
	dist = sqrt((distX * distX) + (distZ * distZ));
	return dist;
}
Enemy::Enemy()
{
	maxhealth = 10;
	currenthealth = maxhealth;
	damage = 1;
	entityPos = Vector3(0, 0, 0);
	entityFacing = 0;
	directionVector = Vector3(0, 0, 1);
	chase = false;
	detectRange = 10;
	attackRange = 1;
	velocity = 10;
	type = 'E';
	name = "";
	atkCd = 1.f;
	hitbox = Hitbox(entityPos.x, entityPos.y + 2.8, entityPos.z, 2.8, 5.6f, 1.4f);
	//takes longer edge to use to get radius
	if (hitbox.sizeX > hitbox.sizeZ) {
		enemyRadius = hitbox.sizeX * 0.5f;
	}
	else {
		enemyRadius = hitbox.sizeZ * 0.5f;
	}
}
Enemy::~Enemy()
{
}
void Enemy::move(Vector3 playerPos, float dt, std::vector<Hitbox> hitboxes, std::vector<Entity*> entities)
{
	if (!chase) {
		if (DistBetweenPoints(playerPos.x, playerPos.z, entityPos.x, entityPos.z) <= detectRange) { //if enemy is within detect range
			chase = true;
		}
	}
	if (chase) {
		Vector3 targetVector = Vector3(playerPos.x, 0, playerPos.z) - Vector3(entityPos.x, 0, entityPos.z);
		targetVector = targetVector.Normalized();
		if (DistBetweenPoints(playerPos.x, playerPos.z, entityPos.x, entityPos.z) > attackRange) //if enemy is out of attack range
		{			
			Vector3 newVector = targetVector * velocity * dt;
			entityPos.x += newVector.x;
			entityPos.z += newVector.z;
		}
		entityFacing = acosf(directionVector.Dot(targetVector));
		if (targetVector.x > 0) {
			entityFacing = Math::RadianToDegree(entityFacing);
		}
		else {
			entityFacing = -Math::RadianToDegree(entityFacing);
		}
	}
}

void Enemy::attack(Vector3, float, Player&, float)
{
}
