#include "Entity.h"
Entity::Entity(int a, int b)
{
	maxhealth = a;
	currenthealth = a;
	damage = b;
}
Entity::~Entity()
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
