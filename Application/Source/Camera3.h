#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"
#include "Structs.h"
#include <vector>


class Camera3 : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt, std::vector<Hitbox> hitboxes);
	virtual void Reset();
	Vector3 CollisionCircleRect(float cx, float cy, float radius, float rx, float ry, float rw, float rh);
	bool CollisionAABB(float r1x, float r1y, float r1z, float r1w, float r1h, float r1d, float r2x, float r2y, float r2z, float r2w, float r2h, float r2d); //(w)idth is x, (h)eight is y, (d)epth is z
	void playerWASDCollision(std::vector<Hitbox> hitboxes);
	void playerCeilingCollision(std::vector<Hitbox> hitboxes, bool& collide, bool up); //when jumping - only does y axis

	void setFirstMouse();
	Hitbox getPlayerHitbox();
	float getPlayerRadius();
	Vector3 getRightVector();
private:
	int jump;
	bool jumping;
	float jumpTime;
	float lastX, lastY;
	float totalPitch;
	float pitch;
	bool firstMouse;
	void LookingAround();
	float playerRadius, playerHeight;
	Hitbox playerHitbox;
	Vector3 returningRightVector;
	double time = 0;
	int resstamina = 0;

	float sensitivityoption = 1;
};

#endif