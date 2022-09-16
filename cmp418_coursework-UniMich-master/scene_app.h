#ifndef _SCENE_APP_H
#define _SCENE_APP_H

#include <system/application.h>
#include <maths/vector2.h>
#include <graphics/sprite.h>
#include <input/input_manager.h>
#include <vector>
#include "Anim_2D.h"
#include "motion_clip_player.h"
#include "ModelData3D.h"
#include "AnimationData.h"
#include "BlendTreeManager.h"
#include "TextureAtlasSpriteSheet.h"
#include "IKManager.h"
#include "primitive_renderer.h"



// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class InputManager;
}

class SceneApp : public gef::Application
{
public:
	SceneApp(gef::Platform& platform);
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();
private:
	void InitFont();
	void CleanUpFont();
	void DrawHUD();
    
	gef::SpriteRenderer* sprite_renderer_;
	gef::InputManager* input_manager_;
	gef::Font* font_;
	float fps_;

	Anim_2D animationClass;

	TextureAtlasSpriteSheet spriteClass;

	std::vector<const char*> animNames;
	std::vector<AnimationData*> animationDataVector;

	BlendTreeManager treeManager;
	ModelData3D modelClass;

	IKManager iKClass;


	enum AnimState { Mesh, Bones, Spritesheet, InverseKinematics };

	AnimState currentState = Mesh;
};

#endif // _SCENE_APP_H
