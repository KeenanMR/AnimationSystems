#pragma once
#include <system/platform.h>
#include <system/application.h>
#include <maths/vector2.h>
#include <maths/vector4.h>
#include <maths/matrix44.h>
#include <maths/math_utils.h>
#include <vector>
#include <graphics/skinned_mesh_instance.h>
#include "primitive_builder.h"
#include "primitive_renderer.h"
#include "BoneStructs.h"
#include <input/input_manager.h>
#include <graphics/renderer_3d.h>
#include <input/keyboard.h>
#include<input/touch_input_manager.h>
#include "CameraDataStruct.h"

//Class to manage the inverse kinematics system

class IKManager
{
public:
	//Constructor
	IKManager();

	//Public functions
	void Init(gef::Platform& platform, gef::SkeletonPose ikReferencePose,gef::SkinnedMeshInstance* player);
	void Update(float deltaTime, gef::Platform& platform);
	void CleanUp();
	void Render(gef::Platform& platform);
	void ResetPose();

	//Setters
	void SetCameraMatrices(gef::Platform& platform);
	void set_ndc_zmin(float value) { ndc_zmin_ = value; }

private:

	//Private functions
	void SetupLights();
	void SetupCamera(gef::Platform& platform);

	//Gef variables for storing model and pose data
	gef::Vector4 effectorPosition;
	gef::Renderer3D* renderer;
	gef::SkeletonPose ikPose;
	gef::InputManager* inputManager;
	gef::SkinnedMeshInstance* localPlayer;

	//Bone structs access variable
	BoneStructs bones;

	//Counter to track the number of limbs to calculate for
	int boneNumber = 1;
	float ndc_zmin_;

	//Camera data
	CameraData camData;

};

