#pragma once
#include <input/input_manager.h>
#include <animation/skeleton.h>
#include <input/keyboard.h>
#include <vector>
#include "OutputNode.h"
#include "ClipNode.h"
#include "LinearBlendNode.h"
#include "BlendTree.h"


//Manager class for blend tree animations

class BlendTreeManager
{
public:

	//Public functions
	void Init(gef::Platform& platform, std::vector<const char*> animNames, gef::SkinnedMeshInstance* skinnedMesh);
	void Input();
	void Update(float deltaTime);
	void ResetClipNodes();

	//Getters
	gef::SkeletonPose GetOutputPose();

private:

	//Nodes for the blend tree
	LinearBlendNode* blendingNode;
	std::vector<ClipNode*> clipNodes;
	OutputNode outputNode;

	//Blend tree variables
	BlendTree tree;

	//Gef variables
	gef::InputManager* manager;

	//Animation iterator
	int animIterator = 0;

};

