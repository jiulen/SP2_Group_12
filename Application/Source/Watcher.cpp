#include "Watcher.h"
Watcher::Watcher(float facing, Vector3 pos, Vector3 direction) {
	maxhealth = 100;
	currenthealth = maxhealth;
	damage = 10;
	entityPos = pos;
	entityFacing = facing;
	directionVector = direction;
	chase = false;
	detectRange = 40;
	attackRange = 2.f;
	velocity = 10.f;
	type = 'E';
	name = "Watcher";
	hitbox = Hitbox(entityPos.x, entityPos.y + 2.8, entityPos.z, 2.8, 5.6f, 1.4f);
	atkCd = 1.f;
	//takes longer edge to use to get radius
	if (hitbox.sizeX > hitbox.sizeZ) {
		enemyRadius = hitbox.sizeX * 0.5f;
	}
	else {
		enemyRadius = hitbox.sizeZ * 0.5f;
	}
}
Watcher::~Watcher()
{
}
void Watcher::takedamage(int a)
{
	if (!chase) 
	{ 
		chase = true; 
	} //aggro if hit
	currenthealth -= a;
	if (currenthealth < 0)
	{
		currenthealth = 0;
	}
}
void Watcher::move(Vector3 playerPos, float dt, std::vector<Hitbox> hitboxes, std::vector<Entity*> entities, Hitbox playerHitbox)
{
	if (!chase) {
		if (DistBetweenPoints(playerPos.x, playerPos.z, entityPos.x, entityPos.z) <= detectRange) { //if player is within detect range
			chase = true;
		}
	}
	Vector3 targetVector = Vector3(playerPos.x, 0, playerPos.z) - Vector3(entityPos.x, 0, entityPos.z);
	targetVector = targetVector.Normalized();
	//always face player
	entityFacing = acosf(directionVector.Dot(targetVector));
	if (targetVector.x > 0) {
		entityFacing = Math::RadianToDegree(entityFacing);
	}
	else {
		entityFacing = -Math::RadianToDegree(entityFacing);
	}
	if (chase) {
		//chases player
		Vector3 newVector = targetVector * velocity * dt;
		entityPos.x += newVector.x;
		entityPos.z += newVector.z;
	}
	hitbox.posX = entityPos.x;
	hitbox.posZ = entityPos.z;

	//Collision
	checkCollisionStructures(hitboxes);
	checkCollisionEntities(entities);
	checkCollisionPlayer(playerHitbox);
}

void Watcher::attack(Vector3 playerPos, float playerRadius, Player& player, float dt)
{
	static float timePassed = atkCd;
	timePassed += dt;
	if (timePassed >= atkCd) {
		if (DistBetweenPoints(entityPos.x, entityPos.z, playerPos.x, playerPos.z) <= attackRange + playerRadius) {
			timePassed = 0.f;
			player.takedamage(damage);
		}
	}
}

void Watcher::alert(std::vector<Entity*>& entities) {
	if (chase) {
		for (int i = 0; i < entities.size(); i++) {
			if (entities[i]->getType() == 'E') {
				entities[i]->setChase(true); //alerts all enemies
			}
		}
	}
}
