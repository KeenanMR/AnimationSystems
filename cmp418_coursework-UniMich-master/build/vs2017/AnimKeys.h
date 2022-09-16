#pragma once
#include <maths/vector2.h>
#include <string>
#include <vector>

//Structs for all animation keys
struct TranslationKey
{
	float nextKeyTimer, startTimer  = 0;
	gef::Vector2 translationVector;
};

struct RotationKey
{
	float nextKeyTimer, startTimer = 0, rotation;
};

struct BoneKey
{
	std::string name;
	std::vector<TranslationKey*> translationKeys;
	std::vector<RotationKey*> rotationKeys;
};

struct Anim
{
	float duration;
	std::string name;
	std::vector<BoneKey*> boneKeys;
	bool render;
};