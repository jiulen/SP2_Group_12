#include "BasicMelee.h"
BasicMelee::BasicMelee(float facing, Vector3 pos, Vector3 direction, Vector3 patrol, float delay) {
	maxhealth = 60;
	currenthealth = maxhealth;
	damage = 10;
	entityPos = pos;
	entityFacing = facing;
	directionVector = direction;
	chase = false;
	detectRange = 20;
	attackRange = 2.f;
	velocity = 10.f;
	type = 'E';
	name = "BasicMelee";
	hitbox = Hitbox(entityPos.x, entityPos.y + 2.8, entityPos.z, 2.8, 5.6f, 1.4f);
	atkCd = 1.f;
	//takes longer edge to use to get radius
	if (hitbox.sizeX > hitbox.sizeZ) {
		enemyRadius = hitbox.sizeX * 0.5f;
	}
	else {
		enemyRadius = hitbox.sizeZ * 0.5f;
	}
	patrolVector = patrol;
	patrolTime = 0.f - delay;
	startPos = entityPos;
	defaultY = entityPos.y;
}
BasicMelee::~BasicMelee()
{
}
void BasicMelee::takedamage(int a)
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
void BasicMelee::move(Vector3 playerPos, float dt, std::vector<Hitbox> hitboxes, std::vector<Entity*> entities, Hitbox playerHitbox)
{
	if (!chase) {
		if (DistBetweenPoints(playerPos.x, playerPos.z, entityPos.x, entityPos.z) <= detectRange) { //if player is within detect range
			chase = true;
		}
	}
	if (!chase && patrolVector.Length() != 0) { //patrol
		patrolVector.Normalize();
		if (patrolTime >= 0.f) {
			Vector3 newVector = patrolVector * velocity * dt;
			entityPos.x += newVector.x;
			entityPos.z += newVector.z;
			//face player when chasing
			entityFacing = acosf(directionVector.Dot(patrolVector));
			if (patrolVector.x > 0) {
				entityFacing = Math::RadianToDegree(entityFacing);
			}
			else {
				entityFacing = -Math::RadianToDegree(entityFacing);
			}
		}
		//reverse patrol vector if time reached
		patrolTime += dt;
		if (patrolTime > 4.f) {
			patrolVector *= -1;
			patrolTime = 0.f;
		}
	}
	if (chase) {
		Vector3 targetVector = Vector3(playerPos.x, 0, playerPos.z) - Vector3(entityPos.x, 0, entityPos.z);
		targetVector = targetVector.Normalized();
		//chases player
		Vector3 newVector = targetVector * velocity * dt;
		entityPos.x += newVector.x;
		entityPos.z += newVector.z;
		//face player when chasing
		entityFacing = acosf(directionVector.Dot(targetVector)); 
		if (targetVector.x > 0) {
			entityFacing = Math::RadianToDegree(entityFacing);
		}
		else {
			entityFacing = -Math::RadianToDegree(entityFacing);
		}
	}
	hitbox.posX = entityPos.x;
	hitbox.posZ = entityPos.z;

	//Collision
	checkCollisionStructures(hitboxes);
	checkCollisionEntities(entities);
	checkCollisionPlayer(playerHitbox);
}

void BasicMelee::attack(Vector3 playerPos, float playerRadius, Player& player, float dt)
{
	//for jump animation
	static bool attack = false;
	static float jumpSpeed = 10.f;
	static float jumpTime = 0.f;

	//for actual atk
	static float timePassed = atkCd;
	timePassed += dt;
	if (timePassed >= atkCd) {
		if (DistBetweenPoints(entityPos.x, entityPos.z, playerPos.x, playerPos.z) <= attackRange + playerRadius) {
			timePassed = 0.f;
			player.takedamage(damage);
			attack = true;
			jumpSpeed = 10.f;
			jumpTime = 0.f;
		}
	}
	//jump animation
	if (attack) {
		jumpTime += dt;
		entityPos.y += static_cast<float>(dt) * jumpSpeed;
		if (jumpTime > 0.05) {
			jumpSpeed -= 5.f;
			jumpTime = 0.f;
		}
		if (entityPos.y <= defaultY)
		{
			entityPos.y = defaultY;
			attack = false;
		}
		hitbox.posY = entityPos.y + 2.8;
	}
	if (!attack) { 
		entityPos.y = defaultY; 
		hitbox.posY = entityPos.y + 2.8;
	}
}
