#pragma once

#include "motion_clip_player.h"
#include <system/application.h>
#include <graphics/scene.h>
#include <animation/animation.h>



//Class for loading/storage/altering of animation data  
class AnimationData
{
private:

	//Gef animation sroage variables
	MotionClipPlayer animationPlayer;
	const char* animationName;
	gef::Animation* animation;
public:
	//Constructor
	AnimationData(const char* name, gef::Platform& platform);

	//Getters
	const char* GetName();
	MotionClipPlayer& GetPlayer();

	//Setter
	void SetPlayback(float duration);

	//Aniamtion load
	gef::Animation* LoadAnimation(gef::Platform& platform, const char* anim_name);

};

