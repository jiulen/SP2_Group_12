#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"
#include "MyMath.h"
#include <cmath>

Camera3::Camera3()
{
}

Camera3::~Camera3()
{
}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	
	lastX = Application::GetWindowWidth() * 0.5f;
	lastY = Application::GetWindowHeight() * 0.5f;
	totalPitch = 0.f;
	firstMouse = true;

    this->position = defaultPosition = pos;
    this->target = defaultTarget = target;
    Vector3 view = (target - position).Normalized();
    Vector3 right = view.Cross(up);
    right.y = 0;
    right.Normalize();
    this->up = defaultUp = right.Cross(view).Normalized();
}

void Camera3::Update(double dt, std::vector<Hitbox> hitboxes)
{
	static const float ROTATE_SPEED = 90.f;
	float MOVE_SPEED;
	//looking around
	LookingAround();
	if (Application::IsKeyPressed(VK_SHIFT))
	{
		MOVE_SPEED = 40.f;
	}
	else
	{
		MOVE_SPEED = 15.f;
	}
	static const float JUMP_SPEED = 25.f;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	if (Application::IsKeyPressed('A'))
	{
		Vector3 startPos = position;
		position -= right * static_cast<float>(dt) * MOVE_SPEED;
		if (position.x < -97.5) {
			position.x = -97.5;
		}
		else if (position.x > 97.5) {
			position.x = 97.5;
		}
		if (position.z < -97.5) {
			position.z = -97.5;
		}
		else if (position.z > 97.5) {
			position.z = 97.5;
		}
		if (playerCollision(hitboxes)) {
			position = startPos;
		}
		position.y = startPos.y;
		target = position + view;
	}
	if (Application::IsKeyPressed('D'))
	{
		Vector3 startPos = position;
		position += right * static_cast<float>(dt) * MOVE_SPEED;
		if (position.x < -97.5) {
			position.x = -97.5;
		}
		else if (position.x > 97.5) {
			position.x = 97.5;
		}
		if (position.z < -97.5) {
			position.z = -97.5;
		}
		else if (position.z > 97.5) {
			position.z = 97.5;
		}
		if (playerCollision(hitboxes)) {
			position = startPos;
		}
		position.y = startPos.y;
		target = position + view;
	}
	if (Application::IsKeyPressed('W'))
	{
		Vector3 startPos = position;
		position += view * static_cast<float>(dt) * MOVE_SPEED;
		if (position.x < -97.5) {
			position.x = -97.5;
		}
		else if (position.x > 97.5) {
			position.x = 97.5;
		}
		if (position.z < -97.5) {
			position.z = -97.5;
		}
		else if (position.z > 97.5) {
			position.z = 97.5;
		}
		if (playerCollision(hitboxes)) {
			position = startPos;
		}
		position.y = startPos.y;
		target = position + view;
	}
	if (Application::IsKeyPressed('S'))
	{
		Vector3 startPos = position;
		position -= view * static_cast<float>(dt) * MOVE_SPEED;
		if (position.x < -97.5) {
			position.x = -97.5;
		}
		else if (position.x > 97.5) {
			position.x = 97.5;
		}
		if (position.z < -97.5) {
			position.z = -97.5;
		}
		else if (position.z > 97.5) {
			position.z = 97.5;
		}
		if (playerCollision(hitboxes)) {
			position = startPos;
		}
		position.y = startPos.y;
		target = position + view;
	}
	if ((Application::IsKeyPressed(VK_SPACE)) && (jump==-1))
	{
		jump = 1;
	}
	if (jump == 1)
	{
		position.y += static_cast<float>(dt) * JUMP_SPEED;
		if (position.y > 10)
			jump = 0;
		target = position + view;
	}
	if (jump == 0)
	{
		position.y -= static_cast<float>(dt) * JUMP_SPEED;
		if (position.y <= defaultPosition.y)
		{
			position.y = defaultPosition.y;
			jump = -1;
		}
		target = position + view;
	}
	if (Application::IsKeyPressed('R'))
	{
		Reset();
	}
}

void Camera3::Reset()
{
    position = defaultPosition;
    target = defaultTarget;
    up = defaultUp;
}

void Camera3::LookingAround() //bug: cant look directly up/down aft a while
{
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();

	double x, y;
	Application::GetCursorPos(&x, &y);
	if (firstMouse) {
		lastX = x;
		lastY = y;
		firstMouse = false;
	}
	float xoffset = x - lastX;
	float yoffset = lastY - y;
	lastX = x;
	lastY = y;

	float sensitivity = 0.05f;
	xoffset *= -sensitivity;
	yoffset *= sensitivity;

	//limits pitch to 90 degrees both up and down
	totalPitch += yoffset;
	if (totalPitch > 90.f) {
		yoffset -= totalPitch - 90.f;
		totalPitch = 90.f;
	}
	else if (totalPitch < -90.f) {
		yoffset -= totalPitch + 90.f;
		totalPitch = -90.f;
	}
	//std::cout << yoffset << std::endl;
	//std::cout << totalPitch << std::endl;

	//yaw
	Mtx44 rotation;
	rotation.SetToRotation(xoffset, up.x, up.y, up.z);
	up = rotation * up;
	view = rotation * view;
	target = position + view;

	//pitch
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
	rotation.SetToRotation(yoffset, right.x, right.y, right.z);
	view = rotation * view;
	target = position + view;
}

Vector3 Camera3::CollisionCircleRect(float cx, float cy, float radius, float rx, float ry, float rw, float rh) {
	//assume all y = 0 since y not needed
	Vector3 endPos = Vector3(cx, 0, cy);
	//Work out nearest point to future player position, around perimeter of cell rectangle. 
	//We can test the distance to this point to see if we have collided. 
	Vector3 nearestPoint;
	nearestPoint.x = Math::Clamp(cx, rx - 0.5f * rw, rx + 0.5f * rw);
	nearestPoint.y = 0;
	nearestPoint.z = Math::Clamp(cy, ry - 0.5f * rh, ry + 0.5f * rh);
	Vector3 rayToNearest = nearestPoint - endPos;
	float overlap = radius - rayToNearest.Length();

	//If overlap is positive, then a collision has occurred, so we displace backwards by the overlap amount. 
	//The potential position is then tested against other tiles in the area therefore "statically" resolving the collision
	
	if (overlap > 0)
	{
		//Statically resolve the collision
		endPos = endPos - rayToNearest.Normalized() * overlap;
	}
	
	return endPos;
}

bool Camera3::CollisionAABB(float r1x, float r1y, float r1z, float r1w, float r1h, float r1d, float r2x, float r2y, float r2z, float r2w, float r2h, float r2d)
{
	return	(r1x - r1w * 0.5f <= r2x + r2w * 0.5f && r1x + r1w * 0.5f >= r2x - r2w * 0.5f) &&
			(r1y - r1h * 0.5f <= r2y + r2h * 0.5f && r1y + r1h * 0.5f >= r2y - r2h * 0.5f) &&
			(r1z - r1d * 0.5f <= r2z + r2d * 0.5f && r1z + r1d * 0.5f >= r2z - r2d * 0.5f);
}
bool Camera3::playerCollision(std::vector<Hitbox> hitboxes) { //for player with non-moving objects - for now camera is 2x4.5x2 (4.5 is camera.defaultPos.y)
	for (int i = 0; i < hitboxes.size(); i++) {
		if (CollisionAABB(position.x, position.y / 2.f, position.z, 2.f, defaultPosition.y, 2.f, (hitboxes[i]).posX, (hitboxes[i]).posY, (hitboxes[i]).posZ, (hitboxes[i]).sizeX, (hitboxes[i]).sizeY, (hitboxes[i]).sizeZ)) {
			return true;
		}
	}
	return false; //no collision
}
