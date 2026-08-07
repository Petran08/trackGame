#pragma once
#include <raylib.h>
#include <string>

class block
{
public:
	Vector3 position;
	int rotation;
	std::string name;
	int blockID = 0;

	block(std::string n, Vector3 pos, int rot, int id)
	{
		position = pos;
		name = n;
		rotation = rot;
		blockID = id;
	}
};

