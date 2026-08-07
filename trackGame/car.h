#pragma once
#include <raylib.h>
#include <raymath.h>

class car
{
public:
	Vector3 position;
	Vector3 angleY;
	float speed;
	short int gear;
	float acceleration;
	Model model;

	void updatePhysics(Camera& camera)
	{
		speed = 0.1 * (IsKeyDown(KEY_UP) - IsKeyDown(KEY_DOWN));

		position.x += speed;
		camera.target = position;
		camera.position = Vector3Add(position, Vector3{ -15, 10, 0 });
	}
};

