#pragma once
#include <raylib.h>
#include <raymath.h>
#include "matrice.h"

class car
{
public:
	Vector3 position;
	Vector3 angle;
	Vector3 up, right, forward;
	float speed;
	short int gear;
	float rpm;
	float deceleration;
	float acceleration;
	Model model;

	void updatePhysics(Camera& camera)
	{
		speed = 0.1 * (IsKeyDown(KEY_UP) - IsKeyDown(KEY_DOWN));

		angle.z += speed * 10 * (IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT));

		position.x += speed;
		camera.target = position;
		camera.position = Vector3Add(position, Vector3{ -15, 10, 0 });
	}

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

		forward = rotatePointAroundAxis(forward, up, angle.y);
		right = rotatePointAroundAxis(right, up, angle.y);
	}
};

