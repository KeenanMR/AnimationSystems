#include "IKManager.h"
#include "picking.h"
#include "ccd.h"

IKManager::IKManager() :
	renderer(NULL),
	effectorPosition(gef::Vector4::kZero)
{
}

void IKManager::Init(gef::Platform& platform, gef::SkeletonPose ikReferencePose, gef::SkinnedMeshInstance* player)
{
	ikPose = ikReferencePose;
	localPlayer = player;

	renderer = gef::Renderer3D::Create(platform);
	inputManager = gef::InputManager::Create(platform);

	SetupCamera(platform);
	SetupLights();
}

void IKManager::Update(float deltaTime, gef::Platform& platform)
{
	bool mb_down = false;
	gef::Vector2 mouse_position(gef::Vector2::kZero);

	// read input devices
	if (inputManager)
	{
		inputManager->Update();

		// keyboard input
		gef::Keyboard* keyboard = inputManager->keyboard();
		if (keyboard)
		{
			if (keyboard->IsKeyPressed(gef::Keyboard::KC_SPACE))
			{
				ResetPose();
			}

			if (keyboard->IsKeyPressed(gef::Keyboard::KC_NUMPAD7))
			{
				if (boneNumber != 4)
					boneNumber++;
			}

			if (keyboard->IsKeyPressed(gef::Keyboard::KC_NUMPAD4))
			{
				if (boneNumber != 0)
					boneNumber--;

			}

		}

		// mouse
		mouse_position = inputManager->touch_manager()->mouse_position();
		mb_down = inputManager->touch_manager()->is_button_down(1);
	}


	if (mb_down)
	{
		gef::Vector4 mouse_ray_start_point, mouse_ray_direction;
		GetScreenPosRay(mouse_position, renderer->projection_matrix(), renderer->view_matrix(), mouse_ray_start_point, mouse_ray_direction, (float)platform.width(), (float)platform.height(), ndc_zmin_);

		if (RayPlaneIntersect(mouse_ray_start_point, mouse_ray_direction, gef::Vector4(0.0f, 0.0f, 0.0f), gef::Vector4(0.0f, 0.0f, 1.0f), effectorPosition))
		{

			switch (boneNumber)
			{
			case 1:
				CalculateCCD(ikPose, *localPlayer, effectorPosition, bones.boneArray.rightArm);
				break;
			case 2:
				CalculateCCD(ikPose, *localPlayer, effectorPosition, bones.boneArray.rightArm);
				CalculateCCD(ikPose, *localPlayer, effectorPosition, bones.boneArray.leftArm);
				break;
			case 3:
				CalculateCCD(ikPose, *localPlayer, effectorPosition, bones.boneArray.rightArm);
				CalculateCCD(ikPose, *localPlayer, effectorPosition, bones.boneArray.leftArm);
				CalculateCCD(ikPose, *localPlayer, effectorPosition, bones.boneArray.leftLeg);
				break;
			case 4:
				CalculateCCD(ikPose, *localPlayer, effectorPosition, bones.boneArray.rightArm);
				CalculateCCD(ikPose, *localPlayer, effectorPosition, bones.boneArray.leftArm);
				CalculateCCD(ikPose, *localPlayer, effectorPosition, bones.boneArray.leftLeg);
				CalculateCCD(ikPose, *localPlayer, effectorPosition, bones.boneArray.rightLeg);
				break;
			}
			localPlayer->UpdateBoneMatrices(ikPose);
		}
	}
}

void IKManager::CleanUp()
{

	delete inputManager;
	inputManager = NULL;

	delete renderer;
	renderer = NULL;
}

void IKManager::Render(gef::Platform& platform)
{
	SetCameraMatrices(platform);

	// draw meshes here
	renderer->Begin();

	// draw the player, the pose is defined by the bone matrices
	if (localPlayer)
	{
		renderer->DrawSkinnedMesh(*localPlayer, localPlayer->bone_matrices());
	}
	renderer->End();
}

void IKManager::SetCameraMatrices(gef::Platform& platform)
{
	// setup view and projection matrices
	gef::Matrix44 projection_matrix;
	gef::Matrix44 view_matrix;
	projection_matrix = platform.PerspectiveProjectionFov(camData.camera_fov_, (float)platform.width() / (float)platform.height(), camData.near_plane_, camData.far_plane_);
	view_matrix.LookAt(camData.camera_eye_, camData.camera_lookat_, camData.camera_up_);

	if (renderer)
	{
		renderer->set_projection_matrix(projection_matrix);
		renderer->set_view_matrix(view_matrix);
	}
}

void IKManager::ResetPose()
{
	ikPose = localPlayer->bind_pose();
	localPlayer->UpdateBoneMatrices(ikPose);
}


void IKManager::SetupLights()
{
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-300.0f, -500.0f, 100.0f));

	gef::Default3DShaderData& default_shader_data = renderer->default_shader_data();
	default_shader_data.set_ambient_light_colour(gef::Colour(0.5f, 0.5f, 0.5f, 1.0f));
	default_shader_data.AddPointLight(default_point_light);
}

void IKManager::SetupCamera(gef::Platform& platform)
{
	// initialise the camera settings
	camData.camera_eye_ = gef::Vector4(0.0f, 100.0f, 500.0f);
	camData.camera_lookat_ = gef::Vector4(0.0f, 100.0f, 0.0f);
	camData.camera_up_ = gef::Vector4(0.0f, 1.0f, 0.0f);
	camData.camera_fov_ = gef::DegToRad(45.0f);
	camData.near_plane_ = 0.01f;
	camData.far_plane_ = 1000.f;

	SetCameraMatrices(platform);
}
