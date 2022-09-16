#pragma once
#include "BlendNode.h"
#include <animation/animation.h>

//Forward declarations
class BlendTree;

class ClipNode :  public BlendNode
{

public:
	//Public functions
	void Init(gef::Platform& platform, const char* animName);//, BlendTree* tree);
	bool Update(float deltaTime,BlendTree* tree) override;

	//Getters
	const char* GetName();

	//Setter
	void SetPoseMesh(gef::SkinnedMeshInstance* mesh);
private:

	//Animation data within the node
	gef::Animation* nodeAnim;

	//Animation mesh data
	gef::SkinnedMeshInstance* bindPoseMesh;

	//Animation name
	const char* clipName;

	//Animation variables
	float animTime = 0.0f, animPlaybackSpeed = 1.0f;
	bool looping = true;
	
	//Private animation loading functions
	gef::Animation* LoadAnimation(gef::Platform& platform, const char* anim_scene_filename, const char* anim_name);

	//Private process function
	bool Process(float deltaTime, BlendTree* tree);

};

