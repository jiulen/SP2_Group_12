#ifndef ENTITY_H
#define ENTITY_H

#include "Vector3.h"
#include "Structs.h"
#include <vector>
class Entity
{
protected:
	int maxhealth, currenthealth, damage;
	Vector3 entityPos;
	float velocity;
	Vector3 directionVector;
	float detectRange, attackRange;
	float entityFacing;
	Hitbox hitbox;
	float enemyRadius;
	float atkCd;
	bool chase;
	float defaultY;

	char type; //N for none(entity), E for enemy, maybe other letter for friendly npcs(if going to add)
	std::string name;

	float DistBetweenPoints(float x1, float z1, float x2, float z2);
	void checkCollisionStructures(std::vector<Hitbox> hitboxes);
	void checkCollisionEntities(std::vector<Entity*> entities);
	void checkCollisionPlayer(Hitbox playerHitbox);
	bool CollisionAABB(float r2x, float r2y, float r2z, float r2w, float r2h, float r2d); //similar to the one in camera, 2 represents the collided object (1 is for colliding object)
	Vector3 CollisionCircleRect(float rx, float ry, float rw, float rh);
public:
	Entity();
	~Entity();
	virtual void move(Vector3, float, std::vector<Hitbox>, std::vector<Entity*> entities, Hitbox playerHitbox);
	virtual void attack(Vector3, float, Player&, float) = 0;
	virtual void takedamage(int a);
	virtual void heal(int a);
	virtual void alert(std::vector<Entity*>& entities) = 0;
	int getdamage();
	int getcurrenthealth();
	int getmaxhealth();
	void updatePosition(Vector3 pos);
	Vector3 getPosition();
	float getFacing();
	char getType();
	Hitbox getHitbox();
	std::string getName();
	bool getChase();
	void setChase(bool chasing);
};

#endif