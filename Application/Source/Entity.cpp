#include "Entity.h"
float Entity::DistBetweenPoints(float x1, float z1, float x2, float z2)
{
	float distX = x1 - x2;
	float distZ = z1 - z2;
	float dist = 0.0f;
	dist = sqrt((distX * distX) + (distZ * distZ));
	return dist;
}
void Entity::checkCollisionStructures(std::vector<Hitbox> hitboxes)
{
	for (int i = 0; i < hitboxes.size(); i++) {
		if (CollisionAABB((hitboxes[i]).posX, (hitboxes[i]).posY, (hitboxes[i]).posZ, (hitboxes[i]).sizeX, (hitboxes[i]).sizeY, (hitboxes[i]).sizeZ)) {
			Vector3 finalPos = CollisionCircleRect((hitboxes[i]).posX, (hitboxes[i]).posZ, (hitboxes[i]).sizeX, (hitboxes[i]).sizeZ);
			entityPos.x = finalPos.x;
			entityPos.z = finalPos.z;
			hitbox.posX = entityPos.x;
			hitbox.posZ = entityPos.z;
		}
	}
	return; //no collision
}
void Entity::checkCollisionEntities(std::vector<Entity*> entities)
{
	for (int i = 0; i < entities.size(); i++) {
		if (CollisionAABB(entities[i]->getHitbox().posX, entities[i]->getHitbox().posY, entities[i]->getHitbox().posZ, entities[i]->getHitbox().sizeX, entities[i]->getHitbox().sizeY, entities[i]->getHitbox().sizeZ)) {
			Vector3 finalPos = CollisionCircleRect(entities[i]->getHitbox().posX, entities[i]->getHitbox().posZ, entities[i]->getHitbox().sizeX, entities[i]->getHitbox().sizeZ);
			entityPos.x = finalPos.x;
			entityPos.z = finalPos.z;
			hitbox.posX = entityPos.x;
			hitbox.posZ = entityPos.z;
		}
	}
	return; //no collision
}
bool Entity::CollisionAABB(float r2x, float r2y, float r2z, float r2w, float r2h, float r2d)
{
	return	(hitbox.posX - enemyRadius < r2x + r2w * 0.5f && hitbox.posX + enemyRadius > r2x - r2w * 0.5f) &&
			(hitbox.posY - hitbox.sizeY * 0.5f < r2y + r2h * 0.5f && hitbox.posY + hitbox.sizeY * 0.5f > r2y - r2h * 0.5f) &&
			(hitbox.posZ - enemyRadius < r2z + r2d * 0.5f && hitbox.posZ + enemyRadius > r2z - r2d * 0.5f);
}
Vector3 Entity::CollisionCircleRect(float rx, float ry, float rw, float rh) {
	//assume all y = 0 since y not needed
	Vector3 endPos = Vector3(hitbox.posX, 0, hitbox.posZ);
	//Work out nearest point to future player position, around perimeter of cell rectangle. 
	//We can test the distance to this point to see if we have collided. 
	Vector3 nearestPoint;
	nearestPoint.x = Math::Clamp(hitbox.posX, rx - 0.5f * rw, rx + 0.5f * rw);
	nearestPoint.y = 0;
	nearestPoint.z = Math::Clamp(hitbox.posZ, ry - 0.5f * rh, ry + 0.5f * rh);
	Vector3 rayToNearest = nearestPoint - endPos;
	float overlap = enemyRadius - rayToNearest.Length();
	if (rayToNearest.Length() == 0) overlap = 0;
	//If overlap is positive, then a collision has occurred, so we displace backwards by the overlap amount. 
	//The potential position is then tested against other tiles in the area therefore "statically" resolving the collision

	if (overlap > 0)
	{
		//Statically resolve the collision
		endPos = endPos - rayToNearest.Normalized() * overlap;
	}

	return endPos;
}
Entity::Entity()
{
	maxhealth = 10;
	currenthealth = maxhealth;
	damage = 1;
	entityPos = Vector3(0,0,0);
	velocity = 1.0f;
	entityFacing = 0.f;
	type = 'N';
	name = "";
	hitbox = (0.f, 0.f, 0.f, 1.f, 1.f, 1.f);
	//takes longer edge to use to get radius
	if (hitbox.sizeX > hitbox.sizeZ) {
		enemyRadius = hitbox.sizeX * 0.5f;
	}
	else {
		enemyRadius = hitbox.sizeZ * 0.5f;
	}
}
Entity::~Entity()
{
}
void Entity::move(Vector3, float, std::vector<Hitbox>, std::vector<Entity*> entities)
{
}
//void Entity::attack()
//{
//}
void Entity::takedamage(int a)
{
	currenthealth -= a;
	if (currenthealth < 0)
	{
		currenthealth = 0;
	}
}
void Entity::heal(int a)
{
	currenthealth += a;
	if (currenthealth > maxhealth)
	{
		currenthealth = maxhealth;
	}
}
int Entity::getdamage()
{
	return damage;
}
int Entity::getcurrenthealth()
{
	return currenthealth;
}
int Entity::getmaxhealth()
{
	return maxhealth;
}

void Entity::updatePosition(Vector3 pos)
{
	entityPos = pos;
}

Vector3 Entity::getPosition()
{
	return entityPos;
}

float Entity::getFacing()
{
	return entityFacing;
}

char Entity::getType()
{
	return type;
}

Hitbox Entity::getHitbox()
{
	return hitbox;
}

std::string Entity::getName()
{
	return name;
}
