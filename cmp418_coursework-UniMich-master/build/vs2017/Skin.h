#pragma once
#include <string>
#include <maths/matrix33.h>
#include <maths/math_utils.h>

//Sprite skin class
class Skin
{
public:
	std::string name;
	float x, y, skX;
	gef::Matrix33 skinTransform, skinRotation, skinTranslation;


	void BuildTransform();

};

//Sprite bone class
class Bone
{
public:

	Bone* parentBone;
	std::string name, parent;
	float x, y, skX, animRot = 0;
	gef::Vector2 animTrans = gef::Vector2(0, 0);

	gef::Matrix33 localTransform, worldTransform, boneTranslate, boneRotate;

	void BuildLocalTransform();
};

//Struct for armature
struct Armature
{
	std::string type, * name;
	float frameRate, x, y, width, height;
};
