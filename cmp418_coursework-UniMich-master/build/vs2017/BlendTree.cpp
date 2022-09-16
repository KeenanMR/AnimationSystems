#include "BlendTree.h"


//Init for a single clip
void BlendTree::Init(gef::SkinnedMeshInstance* skinnedMesh, ClipNode* clip)
{
	output.SetInput(clip);
	output.SetPoseInput(skinnedMesh->bind_pose());
}

//Init for a blend node
void BlendTree::Init(gef::SkinnedMeshInstance* skinnedMesh, LinearBlendNode* blendNode)
{
	output.SetInput(blendNode);
	output.SetPoseInput(skinnedMesh->bind_pose());
	variables.blendValue = 0;
}

//Init the input manager
void BlendTree::InitInput(gef::Platform& platform)
{
	manager = gef::InputManager::Create(platform);
}

//calls the update function of the output
void BlendTree::Update(float deltaTime)
{
	output.Update(deltaTime,this);
}

//Handles variable input control
void BlendTree::Input(const char* currentAnimName)
{
	if (manager)
	{
		manager->Update();

		// keyboard input
		gef::Keyboard* keyboard = manager->keyboard();
		if (keyboard)
		{
		
			if (keyboard->IsKeyPressed(gef::Keyboard::KC_NUMPAD7))
			{
				if(variables.blendValue < 1)
				variables.blendValue += 0.1f;
			}

			if (keyboard->IsKeyPressed(gef::Keyboard::KC_NUMPAD4))
			{
				if(variables.blendValue > 0)
				variables.blendValue -= 0.1f;
			}

			if (keyboard->IsKeyPressed(gef::Keyboard::KC_NUMPAD8))
			{
				variables.animPlaybacks.at(currentAnimName) += 0.1f;
			}

			if (keyboard->IsKeyPressed(gef::Keyboard::KC_NUMPAD5))
			{
				variables.animPlaybacks.at(currentAnimName) -= 0.1f;
			}

			if (keyboard->IsKeyPressed(gef::Keyboard::KC_NUMPAD9))
			{
				variables.animLoops.at(currentAnimName) = true;
			}

			if (keyboard->IsKeyPressed(gef::Keyboard::KC_NUMPAD6))
			{
				variables.animLoops.at(currentAnimName) = false;
			}
		}
	}
}

//return the output piose for the model
gef::SkeletonPose BlendTree::Output()
{
	return output.Output();
}

//Sets the initial value of the tree variables for all clip nodes
void BlendTree::InitTreeVariables(std::vector<ClipNode*> allClips)
{


	for (int i = 0; i < allClips.size(); i++)
	{
		std::pair<const char*, float> playbackPair;
		std::pair<const char*, bool> loopPair;

		playbackPair.first = allClips.at(i)->GetName();
		loopPair.first = allClips.at(i)->GetName();

		playbackPair.second = 1.f;
		loopPair.second = true;

		variables.animPlaybacks.insert(playbackPair);
		variables.animLoops.insert(loopPair);
	}

	return;
}

//Update the input node with a new clip
void BlendTree::UpdateInputNode(ClipNode* clip)
{
	output.SetInput(clip);
}

//Update the input node with a new linear blend node
void BlendTree::UpdateInputNode(LinearBlendNode* blendNode)
{
	output.SetInput(blendNode);
}

//Returns the outpuit node
OutputNode BlendTree::GetOutputNode()
{
	return output;
}

//returns the blend tree variables
BlendTree::TreeVariables BlendTree::GetVariableTable()
{
	return variables;
}
