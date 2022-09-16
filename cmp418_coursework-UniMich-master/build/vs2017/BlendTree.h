#pragma once
#include "OutputNode.h"
#include <input/input_manager.h>
#include <input/keyboard.h>
#include <graphics/skinned_mesh_instance.h>
#include <graphics/scene.h>
#include <vector>
#include <unordered_map>




class BlendTree
{

public:

	//Variables that get passed into 
	struct TreeVariables
	{
		float blendValue = 0;
		std::unordered_map<const char*, float> animPlaybacks;
		std::unordered_map<const char*, bool> animLoops;
		
	};

	//Punlic functions
	void Init(gef::SkinnedMeshInstance* skinnedMesh, ClipNode* clip);
	void Init(gef::SkinnedMeshInstance* skinnedMesh, LinearBlendNode* blendNode);
	void InitInput(gef::Platform & platform); 
	void InitTreeVariables(std::vector<ClipNode*> allClips);
	void UpdateInputNode(ClipNode* clip);
	void UpdateInputNode(LinearBlendNode* blendNode);
	void Update(float deltaTime);
	void Input(const char* currentAnimName);
	gef::SkeletonPose Output();

	//Getters
	OutputNode GetOutputNode();
	TreeVariables GetVariableTable();

private:

	
	//Tree variable strct
	TreeVariables variables;

	//Output
	OutputNode output;

	//Gef variables
	gef::InputManager* manager;
	gef::SkinnedMeshInstance* mesh;


};

