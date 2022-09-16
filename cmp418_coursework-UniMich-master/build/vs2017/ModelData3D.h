#pragma once


#include <graphics/skinned_mesh_instance.h>
#include <graphics/renderer_3d.h>
#include <graphics/sprite_renderer.h>
#include <graphics/scene.h>
#include <maths/math_utils.h>
#include <system/platform.h>
#include "motion_clip_player.h"
#include "CameraDataStruct.h"
#include <vector>
#include <unordered_map>
#include "AnimationData.h"
#include <input/input_manager.h>
#include <input/keyboard.h>

//Forward delcarations
namespace gef
{
	class Renderer3D;
	class Mesh;
	class Scene;
	class Skeleton;
}

//Class to hold 3d model data
class ModelData3D
{

public:

	//Constructor
	ModelData3D();

	//Public functions
	void SetupLights();
	void SetupCamera();
	void Init(gef::Platform& platform);// std::vector<AnimationData*> animData);
	void Input();
	void Cleanup();
	void Update(float frame_time, gef::SkeletonPose treeOutput);
	void Render(gef::Platform& platform, bool render = true);


	//Getters
	gef::Skeleton* GetFirstSkeleton(gef::Scene* scene);
	gef::Mesh* GetFirstMesh(gef::Scene* scene, gef::Platform& platform);
	gef::Scene* GetScene();
	gef::Mesh* GetMesh();
	gef::SkeletonPose GetPose();
	gef::SkinnedMeshInstance* GetSkinnedInstance();
	



private:

	//Gef functions for storage of animation and model data

	gef::InputManager* manager;
	gef::Mesh* mesh_;
	gef::SkinnedMeshInstance* player_;
	gef::Scene* model_scene_;
	gef::Renderer3D* renderer_3d_;
	gef::SkeletonPose finalPose;
	std::vector<MotionClipPlayer> animPlayers;
	//MotionClipPlayer currentPlay, nextPlay;

	//Blend alpha vvalue
	float alpha = 0, fps_;

	//Animation iterator value
	int animIter = 0;

	//Blending bool
	bool blending = false;

	//Camera data
	CameraData camData;



};

