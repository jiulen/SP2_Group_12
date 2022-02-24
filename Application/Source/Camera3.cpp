#include "Camera3.h"
#include "SceneSettings.h"
#include "Application.h"
#include "Mtx44.h"
#include "MyMath.h"
#include <cmath>
#include "Application.h"

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
	jump = -1;
	jumpTime = 0.f;
	jumping = false;
	lastX = Application::GetWindowWidth() * 0.5f;
	lastY = Application::GetWindowHeight() * 0.5f;
	firstMouse = true;
	playerHitbox = Hitbox(position.x, position.y + 0.5f - playerHeight * 0.5f, position.z, 2.8, playerHeight, 1.4f); //use circle for collision in camera (when player colliding), use rect for collision in others (when enemy colliding)
	//takes longer edge to use to get radius
	if (playerHitbox.sizeX > playerHitbox.sizeZ) {
		playerRadius = playerHitbox.sizeX * 0.5f;
	}
	else {
		playerRadius = playerHitbox.sizeZ * 0.5f;
	}
	
	playerHeight = 5; //camera 0.5 below top of player
}

void Camera3::Update(double dt, std::vector<Hitbox> hitboxes)
{
	static const float ROTATE_SPEED = 90.f;
	float MOVE_SPEED;
	//looking around
	LookingAround();
	if (Application::GetStamina() > 0)
	{
		if ((Application::IsKeyPressed(VK_SHIFT))&&((Application::IsKeyPressed('A')) || (Application::IsKeyPressed('W')) || (Application::IsKeyPressed('S')) || (Application::IsKeyPressed('D'))))
		{
			time = 0;
			resstamina = 0;
			MOVE_SPEED = 30.f;/* 22.5f;*/
			Application::SetStamina(Application::GetStamina() - dt);
		}
		else
			resstamina = 1;
	}
	else
		resstamina = 1;
	if (resstamina==1)
	{
		MOVE_SPEED = 15.f;
		if (Application::GetStamina() < 5)
		{
			if (time < 2)
				time += dt;
			if (time >= 2)
			{
				Application::SetStamina(Application::GetStamina() + dt * 2);
			}
		}
	}
	static float JUMP_SPEED = 25.f;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	if (Application::IsKeyPressed('A'))
	{
		Vector3 startPos = position;
		position -= right * static_cast<float>(dt) * MOVE_SPEED;
		position.y = startPos.y;
		playerWASDCollision(hitboxes);
		playerHitbox.posX = position.x;
		playerHitbox.posZ = position.z;
		target = position + view;
	}
	if (Application::IsKeyPressed('D'))
	{
		Vector3 startPos = position;
		position += right * static_cast<float>(dt) * MOVE_SPEED;
		position.y = startPos.y;
		playerWASDCollision(hitboxes);
		playerHitbox.posX = position.x;
		playerHitbox.posZ = position.z;
		target = position + view;
	}
	if (Application::IsKeyPressed('W'))
	{
		Vector3 startPos = position;
		position += view * static_cast<float>(dt) * MOVE_SPEED;
		position.y = startPos.y;
		playerWASDCollision(hitboxes);
		playerHitbox.posX = position.x;
		playerHitbox.posZ = position.z;
		target = position + view;
	}
	if (Application::IsKeyPressed('S'))
	{
		Vector3 startPos = position;
		position -= view * static_cast<float>(dt) * MOVE_SPEED;
		position.y = startPos.y;
		playerWASDCollision(hitboxes);
		playerHitbox.posX = position.x;
		playerHitbox.posZ = position.z;
		target = position + view;
	}
	static bool jumpPressed = false;
	if (!jumpPressed && Application::IsKeyPressed(VK_SPACE) && !jumping)
	{
		jumpPressed = true;
		jumpTime = 0.f;
		jumping = true;
		JUMP_SPEED = 25.f;
	}
	else if (jumpPressed && !Application::IsKeyPressed(VK_SPACE)) {
		jumpPressed = false;
	}
	if (jumping && JUMP_SPEED >= 0) {
		jumpTime += dt;
		position.y += static_cast<float>(dt) * JUMP_SPEED;
		bool collide = false;
		playerCeilingCollision(hitboxes, collide, true);
		if (collide) {
			JUMP_SPEED = -5.f;
			collide = false;
		}
		if (jumpTime > 0.05) {
			JUMP_SPEED -= 5;
			jumpTime = 0.f;
		}
		if (JUMP_SPEED < 0) {
			jumpTime = 0.f;
		}
		playerHitbox.posY = position.y + 0.5f - playerHeight * 0.5f;
		target = position + view;
	}
	if (JUMP_SPEED < 0) {
		jumpTime += dt;
		position.y += static_cast<float>(dt) * JUMP_SPEED;
		bool collide = false;
		playerCeilingCollision(hitboxes, collide, false);
		if (collide) {
			JUMP_SPEED = -0.001f; //may still fall down so negative for falling
			collide = false;
			jumping = false;
		}
		if (jumpTime > 0.05) {
			JUMP_SPEED -= 5;
			jumpTime = 0.f;
		}
		if (position.y <= defaultPosition.y)
		{
			position.y = defaultPosition.y; //cant fall below this
			JUMP_SPEED = 0;
			jumping = false;
		}
		playerHitbox.posY = position.y + 0.5f - playerHeight * 0.5f;
		target = position + view;
	}

	//if (Application::IsKeyPressed('R'))
	//{
	//	Reset();
	//}
}

void Camera3::Reset()
{
    position = defaultPosition;
    target = defaultTarget;
    up = defaultUp;
}

void Camera3::LookingAround()
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

	Application::GetSensitivity();
    float sensitivity = 0.05f * sensitivityoption;
	xoffset *= -sensitivity;
	yoffset *= sensitivity;

	//limits pitch to 80 degrees both up and down
	//test
	static float Pitch = 0.f;
	float totalPitch = Pitch + yoffset; //check if past +-80
	if (totalPitch > 67.5f) {
		yoffset -= totalPitch - 67.5f;
	}
	else if (totalPitch < -67.5f) {
		yoffset -= totalPitch + 67.5f;
	}

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
	returningRightVector = right;
	rotation.SetToRotation(yoffset, right.x, right.y, right.z);
	view = rotation * view;
	target = position + view;

	Pitch = Math::RadianToDegree(asinf(view.y));
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
	return	(r1x - r1w * 0.5f < r2x + r2w * 0.5f && r1x + r1w * 0.5f > r2x - r2w * 0.5f) &&
			(r1y - r1h * 0.5f < r2y + r2h * 0.5f && r1y + r1h * 0.5f > r2y - r2h * 0.5f) &&
			(r1z - r1d * 0.5f < r2z + r2d * 0.5f && r1z + r1d * 0.5f > r2z - r2d * 0.5f);
}

void Camera3::playerWASDCollision(std::vector<Hitbox> hitboxes) { //for player with objects
	for (int i = 0; i < hitboxes.size(); i++) {
		if (CollisionAABB(position.x, position.y + 0.5f - playerHeight * 0.5f, position.z, playerRadius * 2.f, playerHeight, playerRadius * 2.f, (hitboxes[i]).posX, (hitboxes[i]).posY, (hitboxes[i]).posZ, (hitboxes[i]).sizeX, (hitboxes[i]).sizeY, (hitboxes[i]).sizeZ)) {
			Vector3 finalPos = CollisionCircleRect(position.x, position.z, playerRadius, (hitboxes[i]).posX, (hitboxes[i]).posZ, (hitboxes[i]).sizeX, (hitboxes[i]).sizeZ);
			position.x = finalPos.x;
			position.z = finalPos.z;
		}
	}
	return; //no collision
}

void Camera3::playerCeilingCollision(std::vector<Hitbox> hitboxes, bool& collide, bool up)
{
	collide = false;
	for (int i = 0; i < hitboxes.size(); i++) {
		if (CollisionAABB(position.x, position.y + 0.5f - playerHeight * 0.5f, position.z, playerRadius * 2.f, playerHeight, playerRadius * 2.f, (hitboxes[i]).posX, (hitboxes[i]).posY, (hitboxes[i]).posZ, (hitboxes[i]).sizeX, (hitboxes[i]).sizeY, (hitboxes[i]).sizeZ)) {
			//check circle rect collision
			Vector3 nearestPoint;
			nearestPoint.x = Math::Clamp(position.x, (hitboxes[i]).posX - 0.5f * (hitboxes[i]).sizeX, (hitboxes[i]).posX + 0.5f * (hitboxes[i]).sizeX);
			nearestPoint.y = 0;
			nearestPoint.z = Math::Clamp(position.z, (hitboxes[i]).posZ - 0.5f * (hitboxes[i]).sizeZ, (hitboxes[i]).posZ + 0.5f * (hitboxes[i]).sizeZ);
			Vector3 rayToNearest = nearestPoint - Vector3(position.x, 0, position.z);;
			float crOverlap = playerRadius - rayToNearest.Length();
			if (crOverlap > 0) { //collision
				//collision resolution
				float endPosY = position.y + 0.5f - playerHeight * 0.5f;
				float nearestPointY = Math::Clamp(endPosY, (hitboxes[i]).posY - 0.5f * (hitboxes[i]).sizeY, (hitboxes[i]).posY + 0.5f * (hitboxes[i]).sizeY);

				float overlap = 0.f;
				if (up) {
					overlap = playerHeight * 0.5f - (nearestPointY - endPosY);
				}
				else {
					overlap = playerHeight * 0.5f - (endPosY - nearestPointY);
				}
				
				if (overlap > 0)
				{
					//Statically resolve the collision
					if (up) {
						position.y -= overlap;
					}
					else {
						position.y += overlap;
					}
					collide = true;
				}
			}		
			
		}
	}
	return; //no collision
}

void Camera3::setFirstMouse()
{
	firstMouse = true;
}

float Camera3::getPlayerRadius()
{
	return playerRadius;
}

Vector3 Camera3::getRightVector()
{
	return returningRightVector;
}

Hitbox Camera3::getPlayerHitbox()
{
	return playerHitbox;
}
