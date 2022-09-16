#include "BlendNode.h"
#include "BlendTree.h"

//Sets the input noide from base blend node
void BlendNode::SetInput(std::vector<BlendNode*> poses)
{
	for (auto it : poses)
	{
		inputNodes.push_back(it);
	}
}

//Set the input from a single blend node
void BlendNode::SetInput(BlendNode* poses)
{
	inputNodes.push_back(poses);
}

bool BlendNode::Update(float deltaTime, BlendTree* tree)
{
	// initialise check that all inputs are valid
	bool allInputsValid = true;
	for (auto it : inputNodes)
	{
		// recursively call BlendNode::Update to perform depth-first traversal
		bool inputValid = false;
		if (it)
			inputValid = it->Update(deltaTime,tree);
		// check the output of each input node is valid
		if (!inputValid)
			allInputsValid = false;
	}
	// post-order processing starts here
	bool outputValid = false;
	// only process this node is all inputs are valid
	if (allInputsValid)
		outputValid = Process(deltaTime);
	return outputValid;
}

//Simple base process function
bool BlendNode::Process(float deltaTime)
{
	outputPose = inputPose;
	return true;
}

//return the output pose
gef::SkeletonPose BlendNode::Output()
{
	return outputPose;
}
