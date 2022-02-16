#ifndef ENTITY_H
#define ENTITY_H

#include "Vector3.h"
#include "Structs.h"
class Entity
{
protected:
	int maxhealth, currenthealth, damage;
	Vector3 entityPos;
	float velocity;
	float entityFacing;

	char type; //N for none(entity), P for player(not added yet), E for enemy
public:
	Entity();
	~Entity();
	virtual void move(Vector3, float);
	virtual void attack();
	void takedamage(int a);
	void heal(int a);
	int getdamage();
	int getcurrenthealth();
	int getmaxhealth();
	void updatePosition(Vector3 pos);
	Vector3 getPosition();
	float getFacing();
	char getType();
};

#endif