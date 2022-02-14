#pragma once
class Entity
{
private:
	int maxhealth, currenthealth, damage;
public:
	Entity(int a, int b);
	~Entity();
	void takedamage(int a);
	void heal(int a);
	int getdamage();
	int getcurrenthealth();
	int getmaxhealth();
};

