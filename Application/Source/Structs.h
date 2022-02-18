#ifndef STRUCTS_H
#define STRUCTS_H

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
	Player(int health = 100, int damage = 20, int ammo = 7) { Set(health, damage, ammo); }
	void Set(int health, int damage, int ammo) {
		maxHealth = health; currentHealth = health; this->damage = damage; maxAmmo = ammo, currentAmmo = ammo;
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
	void attack() {
		//do shooting here
	}
	void reload() {
		currentAmmo = maxAmmo;
	}
};

#endif