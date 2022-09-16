#include "OutputNode.h"
#include "BlendTree.h"

//Gets the input and sets the state from a blend node
void OutputNode::SetInput(LinearBlendNode* blendNode)
{
	blendNodeLocal = blendNode;
	localState = Blend;
}

//Gets the input and sets the state from a clip node
void OutputNode::SetInput(ClipNode* blendNode)
{
	clipNodeLocal = blendNode;
	localState = Clip;
}

//Update function for use with clip or blend node
bool OutputNode::Update(float deltaTime, BlendTree* tree)
{
	// initialise check that all inputs are valid
	bool allInputsValid = true;

		// recursively call BlendNode::Update to perform depth-first traversal
		bool inputValid = false;
		switch (localState)
		{
		case Blend:
			if (blendNodeLocal)
				inputValid = blendNodeLocal->Update(deltaTime,tree);
			inputPose = blendNodeLocal->Output();
			break;
		case Clip:
			if (clipNodeLocal)
				inputValid = clipNodeLocal->Update(deltaTime,tree);
			inputPose = clipNodeLocal->Output();
			break;
		}
		
		// check the output of each input node is valid
		if (!inputValid)
			allInputsValid = false;

	// post-order processing starts here
	bool outputValid = false;
	// only process this node is all inputs are valid
	if (allInputsValid)
		outputValid = Process(deltaTime);
	return outputValid;
}

//Sets the base pose
void OutputNode::SetPoseInput(gef::SkeletonPose skeleton)
{
	localPose = skeleton;
}

//there is a valid input pose returns true, if not a base pose is used and false is returned
bool OutputNode::Process(float deltaTime)
{
	if (inputPose.local_pose().size() > 1)
	{
		outputPose = inputPose;
		return true;
	}
	else
	{
		outputPose = localPose;
		return false;

	}
}
