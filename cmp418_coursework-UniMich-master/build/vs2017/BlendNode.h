#pragma once
#include <system/application.h>
#include <vector>
#include <graphics/scene.h>
#include <animation/skeleton.h>
#include <graphics/skinned_mesh_instance.h>

//Blend tree forward declaration
class BlendTree;

class BlendNode
{
protected:

	//Base data for child nodes
	std::vector<BlendNode*> inputNodes;
	gef::SkeletonPose inputPose;
	gef::SkeletonPose outputPose;

public:

	//Base functions for child nodes
	void SetInput(std::vector<BlendNode*> poses);
	void SetInput(BlendNode* pose);
	virtual bool Update(float deltaTime, BlendTree* tree);
	virtual bool Process(float deltaTime);
	virtual gef::SkeletonPose Output();
};

