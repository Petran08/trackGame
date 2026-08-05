#pragma once
#include <raylib.h>

class car
{
public:
	Vector3 position;
	Vector3 angle;
	float speed;
	short int gear;
	float acceleration;
	Model model;
};

