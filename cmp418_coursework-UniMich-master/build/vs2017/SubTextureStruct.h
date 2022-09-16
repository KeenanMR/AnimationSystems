#pragma once
#include <maths/matrix33.h>
#include <string>
#include <map>

//Sprite subtexture data struct
class SubTexture
{

public:
	std::string name;
	float frameY, frameX, Y, X, frameHeight, frameWidth, width, height;
	gef::Matrix33 subTransform, subScale, subTranslate;

	void BuildTransform();

};