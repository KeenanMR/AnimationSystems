#pragma once
#include <system/application.h>
#include <graphics/sprite.h>
#include <maths/vector2.h>
#include <maths/vector4.h>
#include <maths/matrix44.h>
#include <vector>
#include <graphics/skinned_mesh_instance.h>
#include <animation/skeleton.h>
#include <animation/animation.h>
#include "motion_clip_player.h"
#include "ModelData3D.h"
#include "CameraDataStruct.h"
#include "AnimationData.h"
#include <unordered_map>


//Class to manage non - blend tree 3d animation
class AnimManager
{

public:
	//Constructor
	AnimManager();

	//Public functions
	void Init(gef::Platform& platform, std::vector<AnimationData*> dataVec, std::vector<const char*> names);
	void Cleanup();


private:

	//Animation loading
	gef::Animation* LoadAnimation(gef::Platform& platform, const char* anim_scene_filename, const char* anim_name);

	//Gef animation storage variables
	MotionClipPlayer *currentAnim,*nextAnim;
	std::vector<gef::Animation*> anims;
	
	//Local model data storage
	ModelData3D modelClass;
};

