#pragma once
#include "LinearBlendNode.h"
#include "ClipNode.h"
#include <animation/skeleton.h>
#include <graphics/skinned_mesh_instance.h>
#include <graphics/mesh.h>



class OutputNode : public BlendNode
{
public:

	//Setters
	void SetPoseInput(gef::SkeletonPose skeleton);
	void SetInput(LinearBlendNode* blendNode);
	void SetInput(ClipNode* clipNode);

	//Public functions
	bool Update(float deltaTime, BlendTree* tree) override;
	bool Process(float deltaTime) override;

	//Input state enum
	enum State{Blend, Clip};

private:

	//base pose if there is an error in process or input
	gef::SkeletonPose localPose;

	//Local input storage
	LinearBlendNode* blendNodeLocal;
	ClipNode* clipNodeLocal;

	//Input state
	State localState;
};

