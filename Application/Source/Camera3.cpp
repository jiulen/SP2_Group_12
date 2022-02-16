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
    this->position = defaultPosition = pos;
    this->target = defaultTarget = target;
    Vector3 view = (target - position).Normalized();
    Vector3 right = view.Cross(up);
    right.y = 0;
    right.Normalize();
    this->up = defaultUp = right.Cross(view).Normalized();
	//
	lastX = Application::GetWindowWidth() * 0.5f;
	lastY = Application::GetWindowHeight() * 0.5f;
	firstMouse = true;
	//for now camera collision is a sphere - 2 x 5 x 2
	playerRadius = 1.f; // subject to change
	playerHeight = 5; //camera 0.5 below top of player
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
		position.y = startPos.y;
		playerWASDCollision(hitboxes);
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
		position.y = startPos.y;
		playerWASDCollision(hitboxes);
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
		position.y = startPos.y;
		playerWASDCollision(hitboxes);
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
		position.y = startPos.y;
		playerWASDCollision(hitboxes);
		target = position + view;
	}
	if ((Application::IsKeyPressed(VK_SPACE)) && (jump==-1))
	{
		jump = 1;
	}
	if (jump == 1)
	{
		position.y += static_cast<float>(dt) * JUMP_SPEED;
		bool collide = false;
		playerCeilingCollision(hitboxes, collide);
		if (collide) {
			jump = 0;
		}
		else if (position.y >= 10)
		{
			position.y = 10;
			jump = 0;
		}	
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

	//limits pitch to 90 degrees both up and down (still got problem)
	float totalPitch = pitch + yoffset; //check if past +-90
	if (totalPitch > 90.f) {
		yoffset -= totalPitch - 90.f;
	}
	else if (totalPitch < -90.f) {
		yoffset -= totalPitch + 90.f;
	}
	float a = pitch + yoffset;

	//yaw
	Mtx44 rotation;
	rotation.SetToRotation(xoffset, 0, 1, 0);
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

	//updates pitch
	Vector3 line = view;
	line.y = 0;
	pitch = acosf( (view.Dot(line)) / view.Length() * line.Length());
	if (view.y >= 0) {
		pitch = Math::RadianToDegree(pitch);
	}
	else {
		pitch = -Math::RadianToDegree(pitch);
	}
	std::cout << pitch << std::endl;
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
	if (rayToNearest.Length() == 0) overlap = 0; //may change later
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
	bool x = r1x - r1w * 0.5f <= r2x + r2w * 0.5f && r1x + r1w * 0.5f >= r2x - r2w * 0.5f;
	bool y = r1y - r1h * 0.5f <= r2y + r2h * 0.5f && r1y + r1h * 0.5f >= r2y - r2h * 0.5f;
	bool z = r1z - r1d * 0.5f <= r2z + r2d * 0.5f && r1z + r1d * 0.5f >= r2z - r2d * 0.5f;
	return	(r1x - r1w * 0.5f <= r2x + r2w * 0.5f && r1x + r1w * 0.5f >= r2x - r2w * 0.5f) &&
			(r1y - r1h * 0.5f <= r2y + r2h * 0.5f && r1y + r1h * 0.5f >= r2y - r2h * 0.5f) &&
			(r1z - r1d * 0.5f <= r2z + r2d * 0.5f && r1z + r1d * 0.5f >= r2z - r2d * 0.5f);
}

void Camera3::playerWASDCollision(std::vector<Hitbox> hitboxes) { //for player with non-moving objects (not sure if can get moveable entities to work with this
	for (int i = 0; i < hitboxes.size(); i++) {
		if (CollisionAABB(position.x, position.y + 0.5f - playerHeight * 0.5f, position.z, playerRadius * 2.f, playerHeight, playerRadius * 2.f, (hitboxes[i]).posX, (hitboxes[i]).posY, (hitboxes[i]).posZ, (hitboxes[i]).sizeX, (hitboxes[i]).sizeY, (hitboxes[i]).sizeZ)) {
			Vector3 finalPos = CollisionCircleRect(position.x, position.z, playerRadius, (hitboxes[i]).posX, (hitboxes[i]).posZ, (hitboxes[i]).sizeX, (hitboxes[i]).sizeZ);
			position.x = finalPos.x;
			position.z = finalPos.z;
		}
	}
	return; //no collision
}

void Camera3::playerCeilingCollision(std::vector<Hitbox> hitboxes, bool& collide) //need make this can jump when touch wall and stop fall
{
	collide = false;
	for (int i = 0; i < hitboxes.size(); i++) {
		if (CollisionAABB(position.x, position.y + 0.5f - playerHeight * 0.5f, position.z, playerRadius * 2.f, playerHeight, playerRadius * 2.f, (hitboxes[i]).posX, (hitboxes[i]).posY, (hitboxes[i]).posZ, (hitboxes[i]).sizeX, (hitboxes[i]).sizeY, (hitboxes[i]).sizeZ)) {
			
			//test
			float endPosY = position.y - playerHeight * 0.5f;
			float nearestPointY = Math::Clamp(endPosY, (hitboxes[i]).posY - 0.5f * (hitboxes[i]).sizeY, (hitboxes[i]).posY + 0.5f * (hitboxes[i]).sizeY);
		
			float overlap = playerHeight * 0.5f - (nearestPointY - endPosY);
	
			if (overlap > 0)
			{
				//Statically resolve the collision
				position.y -= overlap;
				collide = true;
			}
			//
			
		}
	}
	return; //no collision
}

void Camera3::setFirstMouse()
{
	firstMouse = true;
}
