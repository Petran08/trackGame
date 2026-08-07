#pragma once
#include <raylib.h>
#include <raymath.h>
#include "matrice.h"

class car
{
public:
	Vector3 position;
	Vector3 angle = Vector3Zero();
	Vector3 up = { 0.0f, 1.0f, 0.0f }, right = { 0.0f, 0.0f, 1.0f }, forward = {1.0f, 0.0f, 0.0f};
	float speed;
	short int gear;
	float rpm;
	float deceleration;
	float acceleration;
	Model model;
	
	void carAxisUpdate()
	{
		up = Vector3Normalize(up);

		Vector3 forwardRef = { 1.0, 0.0, 0.0 };
		if (fabsf(Vector3DotProduct(up, forwardRef)) > 0.99f)//if up is almost on the x-axis
		{
			if (up.x > 0.0f)
			{
				forwardRef = { 0.0f, -1.0f, 0.0f };//change the forward reference
			}
			else if (up.x < 0.0f)
			{
				forwardRef = { 0.0f, 1.0f, 0.0f };//change the forward reference
			}
		}

		right = Vector3Normalize(Vector3CrossProduct(forwardRef, up));
		forward = Vector3Normalize(Vector3CrossProduct(up, right));

		forward = Vector3RotateByAxisAngle(forward, up, angle.y);
		right = Vector3RotateByAxisAngle(right, up, angle.y);
	}
	
	void updatePhysics(Camera& camera)
	{
		speed = 0.1 * 1 * (IsKeyDown(KEY_UP) - IsKeyDown(KEY_DOWN));

		angle.y += speed / (speed + 1e-9) * DEG2RAD * ( - IsKeyDown(KEY_RIGHT) + IsKeyDown(KEY_LEFT));

		carAxisUpdate();

		position = Vector3Add(position, Vector3Scale(forward, speed));
		camera.target = position;
		camera.position = Vector3Add(position, Vector3Add(Vector3Scale(forward, -15), Vector3Scale(up, 10)));
	}
};

