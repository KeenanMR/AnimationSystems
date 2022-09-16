#pragma once
#include <vector>
#include <string>

//Struct to hold sprite data from tex JSON
struct SpriteData
{
	const char* name;
	float height, width, frameHeight, frameWidth, X, Y, frameX, frameY;
};

//Struct to hold sprite data from skel JSON
struct SkelData
{
	int frameRate;
	std::vector<std::string> frameOrder;
};