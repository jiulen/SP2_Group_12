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
}

void Camera3::Update(double dt)
{
	static const float ROTATE_SPEED = 90.f;
	static const float MOVE_SPEED = 20.f;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	if (Application::IsKeyPressed('A'))
	{
		Vector3 startPos = position;
		position -= right * static_cast<float>(dt) * MOVE_SPEED;
		if (position.x < -46.5) {
			position.x = -46.5;
		}
		else if (position.x > 46.5) {
			position.x = 46.5;
		}
		if (position.z < -46.5) {
			position.z = -46.5;
		}
		else if (position.z > 46.5) {
			position.z = 46.5;
		}
		int i = 0;
		bool stop = false;
		for (int i = 0; i <= 21; ++i) {
			Vector3 finalPos = PlayerCollision(i); //ignore y
			position.x = finalPos.x;
			position.z = finalPos.z;
		}
		position.y = startPos.y;
		target = position + view;
	}
	if (Application::IsKeyPressed('D'))
	{
		Vector3 startPos = position;
		position += right * static_cast<float>(dt) * MOVE_SPEED;
		if (position.x < -46.5) {
			position.x = -46.5;
		}
		else if (position.x > 46.5) {
			position.x = 46.5;
		}
		if (position.z < -46.5) {
			position.z = -46.5;
		}
		else if (position.z > 46.5) {
			position.z = 46.5;
		}
		for (int i = 0; i <= 21; ++i) {
			Vector3 finalPos = PlayerCollision(i); //ignore y
			position.x = finalPos.x;
			position.z = finalPos.z;
		}
		position.y = startPos.y;
		target = position + view;
	}
	if (Application::IsKeyPressed('W'))
	{
		Vector3 startPos = position;
		position += view * static_cast<float>(dt) * MOVE_SPEED;
		if (position.x < -46.5) {
			position.x = -46.5;
		}
		else if (position.x > 46.5) {
			position.x = 46.5;
		}
		if (position.z < -46.5) {
			position.z = -46.5;
		}
		else if (position.z > 46.5) {
			position.z = 46.5;
		}
		for (int i = 0; i <= 21; ++i) {
			Vector3 finalPos = PlayerCollision(i); //ignore y
			position.x = finalPos.x;
			position.z = finalPos.z;
		}
		position.y = startPos.y;
		target = position + view;
	}
	if (Application::IsKeyPressed('S'))
	{
		Vector3 startPos = position;
		position -= view * static_cast<float>(dt) * MOVE_SPEED;
		if (position.x < -46.5) {
			position.x = -46.5;
		}
		else if (position.x > 46.5) {
			position.x = 46.5;
		}
		if (position.z < -46.5) {
			position.z = -46.5;
		}
		else if (position.z > 46.5) {
			position.z = 46.5;
		}
		for (int i = 0; i <= 21; ++i) {
			Vector3 finalPos = PlayerCollision(i); //ignore y
			position.x = finalPos.x;
			position.z = finalPos.z;
		}
		position.y = startPos.y;
		target = position + view;
	}
	if (Application::IsKeyPressed(VK_RIGHT))
	{
		float yaw = -ROTATE_SPEED * static_cast<float>(dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		up = rotation * up;
		view = rotation * view;
		target = position + view;
	}
	if (Application::IsKeyPressed(VK_LEFT))
	{
		float yaw = ROTATE_SPEED * static_cast<float>(dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		up = rotation * up;
		view = rotation * view;
		target = position + view;
	}
	if (Application::IsKeyPressed(VK_DOWN)) //limit
	{
		float pitch = -ROTATE_SPEED * static_cast<float>(dt);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
		if (target.y < position.y - 0.5) {
			target.y = position.y - 0.5;
		}
	}
	if (Application::IsKeyPressed(VK_UP)) //limit
	{
		float pitch = ROTATE_SPEED * static_cast<float>(dt);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
		if (target.y > position.y + 0.5) {
			target.y = position.y + 0.5;
		}
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
	if (std::isnan(overlap))
	{
		overlap = 0; //in case ray same pos as nearestPoint
	}

	// If overlap is positive, then a collision has occurred, so we displace backwards by the overlap amount. 
	// The potential position is then tested against other tiles in the area therefore "statically" resolving the collision
	
	if (overlap > 0)
	{
		// Statically resolve the collision
		endPos = endPos - rayToNearest.Normalized() * overlap;
	}
	
	return endPos;
}
Vector3 Camera3::PlayerCollision(unsigned count) {
	if (count == 0) { //collision with middle table (table 1)
		return CollisionCircleRect(position.x, position.z, 1.5, 0, 0, 4, 4);
	}
	else if (count == 1) { //collision with table 1 up bench
		return CollisionCircleRect(position.x, position.z, 0.625, 0, -3.45, 3.4, 1.7);
	}
	else if (count == 2) { //collision with table 1 down bench
		return CollisionCircleRect(position.x, position.z, 0.625, 0, 3.45, 3.4, 1.7);
	}
	else if (count == 3) { //collision with table 1 left bench
		return CollisionCircleRect(position.x, position.z, 0.625, -3.45, 0, 1.7, 3.4);
	}
	else if (count == 4) { //collision with table 1 right bench
		return CollisionCircleRect(position.x, position.z, 0.625, 3.45, 0, 1.7, 3.4);
	}
	else if (count == 5) { //collision with top table (table 2)
		return CollisionCircleRect(position.x, position.z, 1.5, 0, -20, 4, 4);
	}
	else if (count == 6) { //collision with table 2 up bench
		return CollisionCircleRect(position.x, position.z, 0.625, 0, -23.45, 3.4, 1.7);
	}
	else if (count == 7) { //collision with table 2 down bench
		return CollisionCircleRect(position.x, position.z, 0.625, 0, -16.55, 3.4, 1.7);
	}
	else if (count == 8) { //collision with table 2 left bench
		return CollisionCircleRect(position.x, position.z, 0.625, -3.45, -20, 1.7, 3.4);
	}
	else if (count == 9) { //collision with table 2 right bench
		return CollisionCircleRect(position.x, position.z, 0.625, 3.45, -20, 1.7, 3.4);
	}
	else if (count == 10) { //collision with bottom left table (table 3)
		return CollisionCircleRect(position.x, position.z, 1.5, -15, 20, 4, 4);
	}
	else if (count == 11) { //collision with table 3 up bench
		return CollisionCircleRect(position.x, position.z, 0.625, -15, 16.55, 3.4, 1.7);
	}
	else if (count == 12) { //collision with table 3 down bench
		return CollisionCircleRect(position.x, position.z, 0.625, -15, 23.45, 3.4, 1.7);
	}
	else if (count == 13) { //collision with table 3 left bench
		return CollisionCircleRect(position.x, position.z, 0.625, -18.45, 20, 1.7, 3.4);
	}
	else if (count == 14) { //collision with table 3 right bench
		return CollisionCircleRect(position.x, position.z, 0.625, -11.55, 20, 1.7, 3.4);
	}
	else if (count == 15) { //collision with bottom right table (table 4)
		return CollisionCircleRect(position.x, position.z, 1.5, 15, 20, 4, 4);
	}
	else if (count == 16) { //collision with table 4 up bench
		return CollisionCircleRect(position.x, position.z, 0.625, 15, 16.55, 3.4, 1.7);
	}
	else if (count == 17) { //collision with table 4 down bench
		return CollisionCircleRect(position.x, position.z, 0.625, 15, 23.45, 3.4, 1.7);
	}
	else if (count == 18) { //collision with table 4 left bench
		return CollisionCircleRect(position.x, position.z, 0.625, 11.55, 20, 1.7, 3.4);
	}
	else if (count == 19) { //collision with table 4 right bench
		return CollisionCircleRect(position.x, position.z, 0.625, 18.45, 20, 1.7, 3.4);
	}
	else if (count == 20) { //collision with light1
		return CollisionCircleRect(position.x, position.z, 1.5, 46, 46, 1.3, 1.3);
	}
	else if (count == 21) { //collision with sinks
		return CollisionCircleRect(position.x, position.z, 1.5, 35.85, 46.8, 14.25, 4.5);
	}
	else {
		return Vector3(0, 0, 0); //wont trigger
	}
}
