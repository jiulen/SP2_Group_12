#include "ScaredGuy.h"
float ScaredGuy::DistBetweenPoints(float x1, float z1, float x2, float z2)
{
	float distX = x1 - x2;
	float distZ = z1 - z2;
	float dist = 0.0f;
	dist = sqrt((distX * distX) + (distZ * distZ));
	return dist;
}
ScaredGuy::ScaredGuy(float facing, Vector3 pos, Vector3 direction)
{
	maxhealth = 10;
	currenthealth = maxhealth;
	damage = 0;
	entityPos = pos;
	entityFacing = facing;
	directionVector = direction;
	chase = false;
	detectRange = 10;
	attackRange = 1;
	velocity = 30;
	type = 'F';
	name = "ScaredGuy";
	atkCd = 1.f;
	hitbox = Hitbox(entityPos.x, entityPos.y + 2.8, entityPos.z, 2.8, 5.6f, 1.4f);
	defaultY = entityPos.y;
	//takes longer edge to use to get radius
	if (hitbox.sizeX > hitbox.sizeZ) {
		enemyRadius = hitbox.sizeX * 0.5f;
	}
	else {
		enemyRadius = hitbox.sizeZ * 0.5f;
	}
}
ScaredGuy::~ScaredGuy()
{
}
void ScaredGuy::takedamage(int a)
{
	currenthealth -= a;
	if (currenthealth < 0)
	{
		currenthealth = 0;
	}
}
void ScaredGuy::move(Vector3 playerPos, float dt, std::vector<Hitbox> hitboxes, std::vector<Entity*> entities, Hitbox playerHitbox)
{
	static float runTime = 0.f;
	if (chase) {
		runTime += dt;
		if (runTime < 5.f) { //runs away from player for 5 seconds
			Vector3 targetVector = Vector3(playerPos.x, 0, playerPos.z) - Vector3(entityPos.x, 0, entityPos.z);
			targetVector = targetVector.Normalized();
			//runs away from player
			Vector3 newVector = targetVector * velocity * dt;
			entityPos.x -= newVector.x;
			entityPos.z -= newVector.z;
			//face player when chasing
			entityFacing = acosf(directionVector.Dot(targetVector));
			if (targetVector.x > 0) {
				entityFacing = Math::RadianToDegree(entityFacing) + 180;
			}
			else {
				entityFacing = -Math::RadianToDegree(entityFacing) + 180;
			}
			hitbox.posX = entityPos.x;
			hitbox.posZ = entityPos.z;

			//Collision
			checkCollisionStructures(hitboxes);
			checkCollisionEntities(entities);
			checkCollisionPlayer(playerHitbox);
		}
		else {
			runTime = 0;
			chase = false;
		}		
	}
}

void ScaredGuy::attack(Vector3, float, Player&, float)
{
}

void ScaredGuy::alert(std::vector<Entity*>& entities) {

}
