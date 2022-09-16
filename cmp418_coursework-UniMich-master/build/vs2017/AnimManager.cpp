#include "AnimManager.h"


AnimManager::AnimManager()
{

}

void AnimManager::Init(gef::Platform& platform, std::vector<AnimationData*> dataVec, std::vector<const char*> names)
{
	modelClass.Init(platform);

	int iter = 0;

	for (auto it : names)
	{
		gef::Animation* temp = LoadAnimation(platform, it, "");
		anims.push_back(temp);
		iter++;
	}

	iter = 0;

	for (auto it : dataVec)
	{
		it->GetPlayer().set_clip(anims.at(iter));
		it->GetPlayer().set_looping(true);
		it->GetPlayer().set_anim_time(0.0f);

		if (it->GetName() == "xbot/xbot@walking_inplace.scn")
		{
			it->GetPlayer().set_playback_speed(anims.at(iter)->duration() / anims.at(iter - 1)->duration());
		}
		iter++;
	}
}


void AnimManager::Cleanup()
{

	for (auto it : anims)
	{
		delete it;
		it = NULL;
	}
	anims.clear();
}

gef::Animation* AnimManager::LoadAnimation(gef::Platform& platform,const char* anim_scene_filename, const char* anim_name)
{
	gef::Animation* anim = NULL;

	gef::Scene anim_scene;
	if (anim_scene.ReadSceneFromFile(platform, anim_scene_filename))
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