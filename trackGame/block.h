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
	Model blockModel;
	double size = 15;//for a normal block

	block(std::string n, Vector3 pos, int rot, int id, Model model)
	{
		position = pos;
		name = n;
		rotation = rot;
		blockID = id;
		blockModel = model;
	}
};

