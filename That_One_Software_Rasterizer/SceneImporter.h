#pragma once


#include "Point.h"

#include <string>
#include <vector>

struct Vertex
{
	Point4D pos;
	// todo: MORE DATA HAHAHAHHAHAH
};

bool ImportScene(std::string const& file, std::vector<Vertex>& outVerices, std::vector<uint32_t>& outIndices);