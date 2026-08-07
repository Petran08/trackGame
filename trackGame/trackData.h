#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "block.h"

class trackData
{
public:
	std::vector<block> blocks;

	void readTrackFile(std::string fileName)
	{
		std::string nm;
		int rot, id;
		float x, y, z;
		std::ifstream in(fileName);
		while (in >> nm)
		{
			in >> x >> y >> z >> rot >> id;
			block temp(nm, Vector3{x, y, z}, rot, id);
			blocks.push_back(temp);
		}
	}

	void saveTrackFile(std::string fileName)
	{
		std::ofstream out(fileName);
		for (auto blc : blocks)
		{
			out << blc.name << ' ' << blc.position.x << ' ' << blc.position.y << ' ' << blc.position.z << ' ' << blc.rotation << ' ' << blc.blockID << '\n';
		}
	}
};

