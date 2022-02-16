#include "Enemy.h"
float Enemy::DistBetweenPoints(float x1, float z1, float x2, float z2)
{
	float distX = x1 - x2;
	float distZ = z1 - z2;
	float dist = 0.0f;
	dist = sqrt((distX * distX) + (distZ * distZ));
	return dist;
}
Enemy::Enemy(int a, int b, float facing, Vector3 pos, Vector3 direction, bool chasing, float detectR, float attackR, float velo) {
	maxhealth = a;
	currenthealth = maxhealth;
	damage = b;
	entityPos = pos;
	entityFacing = facing;
	directionVector = direction;
	chase = chasing;
	detectRange = detectR;
	attackRange = attackR;
	velocity = velo;
	type = 'E';
}
Enemy::~Enemy()
{
}
void Enemy::move(Vector3 playerPos, float dt)
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

void Enemy::attack()
{
}
