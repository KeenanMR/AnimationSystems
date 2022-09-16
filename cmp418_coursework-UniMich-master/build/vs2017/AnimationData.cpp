#include "AnimationData.h"

//Constructor to set the base animation data
AnimationData::AnimationData(const char* name, gef::Platform& platform)
{
	animationName = name;
	animation = LoadAnimation(platform, "");

	if (animation)
	{
		animationPlayer.set_clip(animation);
		animationPlayer.set_looping(true);
		animationPlayer.set_anim_time(0.0f);
	}
}

//Setter for playback speed
void AnimationData::SetPlayback(float duration)
{
	animationPlayer.set_playback_speed(animation->duration() / duration);
}

//Getter for name
const char* AnimationData::GetName()
{
	return animationName;
}

//Getter for the animation motiojn clip player
MotionClipPlayer& AnimationData::GetPlayer()
{
	return animationPlayer;
}

//Load animation
gef::Animation* AnimationData::LoadAnimation(gef::Platform& platform, const char* anim_name)
{
	gef::Animation* anim = NULL;

	gef::Scene anim_scene;
	if (anim_scene.ReadSceneFromFile(platform, animationName))
	{
		// if the animation name is specified then try and find the named anim
		// otherwise return the first animation if there is one
		std::map<gef::StringId, gef::Animation*>::const_iterator anim_node_iter;
		if (anim_name)
			anim_node_iter = anim_scene.animations.find(gef::GetStringId(anim_name));
		else
			anim_node_iter = anim_scene.animations.begin();

		if (anim_node_iter != anim_scene.animations.end())
			anim = new gef::Animation(*anim_node_iter->second);
	}

	return anim;
}