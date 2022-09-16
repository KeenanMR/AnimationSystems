#include "scene_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <system/debug_log.h>
#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>
#include <maths/math_utils.h>
#include <input/sony_controller_input_manager.h>
#include <graphics/sprite.h>
#include "load_texture.h"
#include "load_json.h"


#include "rapidjson\document.h"



SceneApp::SceneApp(gef::Platform& platform) :
	Application(platform),
	sprite_renderer_(NULL),
	font_(NULL)
{
}

void SceneApp::Init()
{
	//Initialise sprite renderer
	spriteClass.init(platform_);
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	input_manager_ = gef::InputManager::Create(platform_);
	InitFont();

	animNames.push_back("xbot/xbot@running_inplace.scn");
	animNames.push_back("xbot/xbot@walking_inplace.scn");
	animNames.push_back("xbot/xbot@idle.scn");
	animNames.push_back("xbot/xbot@jump.scn");

	/*
	for (int i = 0; i < animNames.size(); i++)
	{
		AnimationData* anim = new AnimationData(animNames.at(i), platform_);
		animationDataVector.push_back(anim);
	}
	*/

	
	//Initialise animation class
	animationClass.init(platform_);

	//Init 3D animations
	modelClass.Init(platform_);
	treeManager.Init(platform_, animNames, modelClass.GetSkinnedInstance());
	iKClass.Init(platform_, modelClass.GetPose(), modelClass.GetSkinnedInstance());

	

}

void SceneApp::CleanUp()
{

	spriteClass.CleanUp();
	CleanUpFont();
	animationClass.CleanUp();
	iKClass.CleanUp();
	modelClass.Cleanup();

	delete sprite_renderer_;
	sprite_renderer_ = NULL;
}




bool SceneApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;



	//Update input and animation classes
	
	
		// read input devices
		if (input_manager_)
		{
			input_manager_->Update();

			// keyboard input
			gef::Keyboard* keyboard = input_manager_->keyboard();
			if (keyboard)
			{
				if (keyboard->IsKeyDown(gef::Keyboard::KC_ESCAPE))
				{
					exit(0);
				}

				if (keyboard->IsKeyDown(gef::Keyboard::KC_0))
				{
					currentState = Spritesheet;
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_1))
				{
					currentState = Mesh;
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_2))
				{
					currentState = Bones;
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_3))
				{
					iKClass.ResetPose();
					currentState = InverseKinematics;
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_4))
				{

					currentState = Spritesheet;
				}
			}
		}
	

		if (currentState == Mesh)
		{
			treeManager.Input();
			treeManager.Update(frame_time);
			modelClass.Update(frame_time,treeManager.GetOutputPose());
		}

		if (currentState == InverseKinematics)
		{
			iKClass.Update(frame_time,platform_);
		}

		if (currentState == Bones)
		{
			animationClass.Input();
			animationClass.Update(frame_time);
		}

		if (currentState == Spritesheet)
		{
			spriteClass.Input();
			spriteClass.Update(platform_,frame_time);
		}



	return true;
}




void SceneApp::Render()
{


	
	if(currentState == Mesh)
		modelClass.Render(platform_);

	if(currentState == Bones)
		animationClass.Render2D(platform_);

	if (currentState == InverseKinematics)
		iKClass.Render(platform_);

	//Render animation
	if (currentState == Spritesheet)
	{
		sprite_renderer_->Begin();
		sprite_renderer_->DrawSprite(spriteClass.GetSprite());
	}
	else
	{
		sprite_renderer_->Begin(false);
	}
	DrawHUD();
	sprite_renderer_->End();


}

void SceneApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void SceneApp::CleanUpFont()
{
	animationClass.CleanUp();
	modelClass.Cleanup();


	for (auto it : animNames)
	{
		delete it;
		it = NULL;
	}

	animNames.clear();

	delete font_;
	font_ = NULL;
}

void SceneApp::DrawHUD()
{
	if(font_)
	{
		// display frame rate
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);

		font_->RenderText(sprite_renderer_, gef::Vector4(0.0f, 0.0f, -0.9f), 0.75f, 0xffffffff, gef::TJ_LEFT, "Keys 1-4 Switch Animation Type");
		font_->RenderText(sprite_renderer_, gef::Vector4(0.0f, 30.0f, -0.9f), 0.75f, 0xffffffff, gef::TJ_LEFT, "Spacebar Iterates Loaded Animations");
		font_->RenderText(sprite_renderer_, gef::Vector4(0.0f, 90.0f, -0.9f), 0.75f, 0xffffffff, gef::TJ_LEFT, "When on 3D Animation Type:");
		font_->RenderText(sprite_renderer_, gef::Vector4(0.0f, 120.0f, -0.9f), 0.75f, 0xffffffff, gef::TJ_LEFT, "Numpad 4 & 7 Control Blend Value");
		font_->RenderText(sprite_renderer_, gef::Vector4(0.0f, 150.0f, -0.9f), 0.75f, 0xffffffff, gef::TJ_LEFT, "Numpad 5 & 8 Control Playback Speed");
		font_->RenderText(sprite_renderer_, gef::Vector4(0.0f, 180.0f, -0.9f), 0.75f, 0xffffffff, gef::TJ_LEFT, "Numpad 6 & 9 Control Looping");

		font_->RenderText(sprite_renderer_, gef::Vector4(0.0f, 240.0f, -0.9f), 0.75f, 0xffffffff, gef::TJ_LEFT, "When on Inverse Kinematics Type:");
		font_->RenderText(sprite_renderer_, gef::Vector4(0.0f, 270.0f, -0.9f), 0.75f, 0xffffffff, gef::TJ_LEFT, "Numpad 4 & 7 Control Number of Calculated Limbs");
	}
}

