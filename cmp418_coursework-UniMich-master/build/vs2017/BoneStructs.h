#pragma once
#include <vector>


//Class to hold the bone data for each limb
class BoneStructs
{

public:

	struct BoneArray
	{
		std::vector<int> rightArm{ 16,17,18 };
		std::vector<int> leftArm{ 35,36,37 };
		std::vector<int> leftLeg{ 2,5,6,7 };
		std::vector<int> rightLeg{ 3,8,9,10 };
	};

	BoneArray boneArray;

};