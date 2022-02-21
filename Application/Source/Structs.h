#ifndef STRUCTS_H
#define STRUCTS_H

#include "MyMath.h"

struct Hitbox //AABB hitbox
{
    float posX, posY, posZ, sizeX, sizeY, sizeZ;
    Hitbox(float posX = 0, float posY = 0, float posZ = 0, float sizeX = 1, float sizeY = 1, float sizeZ = 1) { Set(posX, posY, posZ, sizeX, sizeY, sizeZ); }
    void Set(float posX, float posY, float posZ, float sizeX, float sizeY, float sizeZ) {
        this->posX = posX; this->posY = posY; this->posZ = posZ;
        this->sizeX = sizeX; this->sizeY = sizeY; this->sizeZ = sizeZ;
    }
};

//Player struct
struct Player {
	int maxHealth, currentHealth, damage, maxAmmo, currentAmmo;
	float shootCD, reloadCD;
	float timeBetweenShot;
	Player(int health = 100, int damage = 20, int ammo = 7, float cd = 0.5, float rd = 2) { Set(health, damage, ammo, cd, rd); }
	void Set(int health, int damage, int ammo, float cd, float rd) {
		maxHealth = health; currentHealth = health; this->damage = damage; maxAmmo = ammo, currentAmmo = ammo; shootCD = cd; reloadCD = rd; timeBetweenShot = shootCD;
	}
	void takedamage(int dmg) {
		currentHealth -= dmg;
		if (currentHealth < 0) {
			currentHealth = 0;
		}
	}
	void heal(int healing) {
		currentHealth += healing;
		if (currentHealth > maxHealth) {
			currentHealth = maxHealth;
		}
	}
	bool attack(float dt) {
		if (timeBetweenShot >= shootCD)
		{
			if (currentAmmo > 0)
			{
				currentAmmo--;
				timeBetweenShot = 0;
				std::cout << "Ammo left: " << currentAmmo << std::endl;
				return true;
			}
		}
		return false;
	}
	void reload(float dt, bool& reloading) {
		static float reloadTime = 0;
		reloadTime += dt;
		if (reloadTime >= reloadCD)
		{
			currentAmmo = maxAmmo;
			reloading = false;
			reloadTime = 0;
			std::cout << "reloaded" << std::endl;
		}
	}

	void Update(float dt) {
		timeBetweenShot += dt;
	}
};

struct Bullet {
	int bulletDamage;
	float bulletSpeed;
	Vector3 directionVector;
	Hitbox bulletHitbox;

	Bullet(int damage = 0, float speed = 0, float sizeX = 1, float sizeY = 1, float sizeZ = 1, Vector3 direction = (0, 0, 0), Vector3 pos = (0, 0, 0)) { Set(damage, speed, sizeX, sizeY, sizeZ, direction, pos); }
	void Set(int damage, float speed, float sizeX, float sizeY, float sizeZ, Vector3 direction, Vector3 pos) {
		bulletDamage = damage; bulletSpeed = speed; bulletHitbox.posX = pos.x; bulletHitbox.posY = pos.y; bulletHitbox.posZ = pos.z; bulletHitbox.sizeX = sizeX; bulletHitbox.sizeY = sizeY; bulletHitbox.sizeZ = sizeZ; directionVector = direction;
	}
	bool bulletHit(Hitbox hitbox) {
		float x = Math::Clamp(bulletHitbox.posX, hitbox.posX - 0.5f * hitbox.sizeX, hitbox.posX + 0.5f * hitbox.sizeX);
		float y = Math::Clamp(bulletHitbox.posY, hitbox.posY - 0.5f * hitbox.sizeY, hitbox.posY + 0.5f * hitbox.sizeY);
		float z = Math::Clamp(bulletHitbox.posZ, hitbox.posY - 0.5f * hitbox.sizeY, hitbox.posY + 0.5f * hitbox.sizeY);

		//check distancebetween nearest point of sphere and cube
		float distance = sqrt((x - bulletHitbox.posX) * (x - bulletHitbox.posX) + (y - bulletHitbox.posY) * (y - bulletHitbox.posY) + (z - bulletHitbox.posZ) * (z - bulletHitbox.posZ));
		return distance < (bulletHitbox.sizeX * 0.5f);
	}

	void Update(float dt) {
		Vector3 newVector = directionVector * bulletSpeed * dt;
		bulletHitbox.posX += newVector.x;
		bulletHitbox.posY += newVector.y;
		bulletHitbox.posZ += newVector.z;
	}

};

#endif