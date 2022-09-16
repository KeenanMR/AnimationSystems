#pragma once
#include <string>
#include "load_texture.h"
#include "load_json.h"
#include "AnimKeys.h"
#include "rapidjson\document.h"
#include "TextureAtlas.h"
#include "graphics/sprite_renderer.h"
#include "input/keyboard.h"
#include "input/input_manager.h"
#include <unordered_map>
#include <graphics/font.h>


//Class for handling all 2D animation controls
#pragma once
class Anim_2D
{
private:

	TextureAtlas tex;
	std::vector<std::string> partNames;
	std::vector<std::string> animNames;
	gef::Matrix33 fullTransform;
	gef::Vector2 sprite_pos_;
	gef::SpriteRenderer* renderer;
	RotationKey rotKey;
	TranslationKey transKey;
	BoneKey boneKey;
	Anim animationData;
	std::unordered_map<std::string,Anim> animMap;
	int currentFrame = 0;
	std::string currentAnim;
	gef::InputManager* inputManager;
	float animIterator = 0, frameTimeLocal, timer = 0, fps_;
	int frameRate;

public:

	//File Read/Write functions
	void ReadKeys();
	Anim* PopulateKeys(const rapidjson::Value& keyMember);

	//Sprite Update functions
	void SpriteTransform(gef::Platform& platform);

	//Animation transform functions
	gef::Vector2 AnimTranslate(std::string currentBone);
	float AnimRotate(std::string currentAnim);

	//Reset anims to default positions when changed
	void ResetAnims();

	//Basic Run Functions
	Anim_2D();
	void init(gef::Platform& platform);
	void Input();
	void Render2D(gef::Platform& platform, bool render = true);
	void Update(float frameTime);
	void CleanUp();

	//Getters
	int GetFrameRate();
	
	//Lerp funcs
	gef::Vector2 posLerp(gef::Vector2(startPos), gef::Vector2(endPos), float time);
	float rotLerp(float startAngle, float endAngle, float time);

};

