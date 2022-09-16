#pragma once
#include "BlendNode.h"
#include "ClipNode.h"

//Forward declarations
class BlendTree;

class LinearBlendNode :  public BlendNode
{
public:

	//Public functions 
	void SetInput(ClipNode* inputNode);
	bool Update(float deltaTime, BlendTree* tree) override;
	bool Process(float deltaTime, float blendValue);
	void UpdateInputNodes(ClipNode* newNodes[]);

private:

	//Clip node input vector
	std::vector<ClipNode*> clipNodes;

	//Alpha blend value 
	float alpha = 0;

};

