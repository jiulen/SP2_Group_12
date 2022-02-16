#include "Entity.h"
Entity::Entity()
{
	maxhealth = 10;
	currenthealth = maxhealth;
	damage = 1;
	entityPos = Vector3(0,0,0);
	velocity = 1.0f;
	entityFacing = 0.f;
	type = 'N';
}
Entity::~Entity()
{
}
void Entity::move(Vector3, float)
{
}
void Entity::attack()
{
}
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
