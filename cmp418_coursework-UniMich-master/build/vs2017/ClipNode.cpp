#include "ClipNode.h"
#include "BlendTree.h"

//Load in the animation supplied and save the anme
void ClipNode::Init(gef::Platform& platform, const char* animName)//, BlendTree* tree)
{
	nodeAnim = LoadAnimation(platform,animName,"");
	clipName = animName;
}

//Load in the mesh for the animation
void ClipNode::SetPoseMesh(gef::SkinnedMeshInstance* mesh)
{
	bindPoseMesh = mesh;
	outputPose = bindPoseMesh->bind_pose();
}

//Call process
bool ClipNode::Update(float deltaTime, BlendTree* tree)
{
	return Process(deltaTime, tree);
}

//Gets the name of the animation
const char* ClipNode::GetName()
{
	return clipName;
}

//Processes the animation within the node (Uses code from motion clip player adapted to the node)
bool ClipNode::Process(float deltaTime, BlendTree* tree)
{

	bool processed = false;

	if (nodeAnim)
	{
		// update the animation playback time
		animTime += deltaTime * tree->GetVariableTable().animPlaybacks[clipName];

		// check to see if the playback has reached the end of the animation
		if (animTime > nodeAnim->duration())
		{
			// if the animation is looping then wrap the playback time round to the beginning of the animation
			// other wise set the playback time to the end of the animation and flag that we have reached the end
			if (tree->GetVariableTable().animLoops[clipName])
				animTime = std::fmodf(animTime, nodeAnim->duration());
			else
			{
				animTime = nodeAnim->duration();
				processed = true;
			}
		}

		// add the clip start time to the playback time to calculate the final time
		// that will be used to sample the animation data
		float time = animTime + nodeAnim->start_time();

		// sample the animation data at the calculated time
		// any bones that don't have animation data are set to the bind pose
		outputPose.SetPoseFromAnim(*nodeAnim, bindPoseMesh->bind_pose(), time);
		processed = true;
	
	}
	else
	{
		// no animation associated with this player
		// just set the pose to the bind pose
		outputPose = bindPoseMesh->bind_pose();
		processed = true;
	}

	// return true if the animation has been succesfully processed, else false
	return processed;
}

//Load animaiton
gef::Animation* ClipNode::LoadAnimation(gef::Platform& platform, const char* anim_scene_filename, const char* anim_name)
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