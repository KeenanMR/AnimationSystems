#include "LinearBlendNode.h"
#include "BlendTree.h"

//Gets the input from a clip node, must be called multiple times 
void LinearBlendNode::SetInput(ClipNode* inputNode)
{
    clipNodes.push_back(inputNode);
}

//Basic update function for blend node
bool LinearBlendNode::Update(float deltaTime, BlendTree* tree)
{
	// initialise check that all inputs are valid
	bool allInputsValid = true;
	for (auto it : clipNodes)
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
		outputValid = Process(deltaTime,tree->GetVariableTable().blendValue);
	return outputValid;
}

//Process
bool LinearBlendNode::Process(float deltaTime, float blendValue)
{
	//Resize the input to only have 2 clips
    inputNodes.resize(2);

    gef::SkeletonPose blendPoses[2];
   
	//Get the clip poses at the current time
    for (int i = 0; i < 2; i++)
    {
       blendPoses[i] = clipNodes.at(i)->Output();
    }

	//Blend the poses together
    blendPoses[0].Linear2PoseBlend(blendPoses[0], blendPoses[1], blendValue);
    alpha += deltaTime/2;
    if (alpha >= 1)
        alpha = 0;

	outputPose = blendPoses[0];

    return true;
}

//changes the clips being blended
void LinearBlendNode::UpdateInputNodes(ClipNode* newNodes[])
{
	clipNodes.clear();

	for (int i = 0; i < 2; i++)
	{
		clipNodes.push_back(newNodes[i]);
	}
}
