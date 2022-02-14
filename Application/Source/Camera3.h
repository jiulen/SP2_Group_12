#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"

class Camera3 : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	int jump=-1;

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();
	//Vector3 CollisionCircleRect(float cx, float cy, float radius, float rx, float ry, float rw, float rh);
	//Vector3 PlayerCollision(unsigned count);
private:
	float lastX, lastY;
	bool firstMouse;
	void LookingAround();
};

#endif