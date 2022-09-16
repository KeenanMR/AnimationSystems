#pragma once
#include <unordered_map>
#include <string>
#include "SubTexDataSprite.h"
#include <graphics/sprite.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <input/input_manager.h>
#include <input/keyboard.h>
#include <rapidjson/document.h>
#include <graphics/texture.h>
#include "load_texture.h"
#include "load_json.h"

class TextureAtlasSpriteSheet
{
	//Struct for the sprite sheet
	struct SheetData
	{
		int width, height;
		const char* name;
	};

public:

	//Constructor
	TextureAtlasSpriteSheet();

	//Public functions
	void init(gef::Platform& platform);
	void Update(gef::Platform& platform, float frame_time);
	void Render();
	void CleanUp();
	void Input();
	void ReadPopJSON(rapidjson::Document& texDoc, SheetData& sheetData);
	void ReadPopSkel(rapidjson::Document& skelDoc, SkelData& dataStruct);
	const char* PopAnimOrder(const rapidjson::Value& subTextureArrayMemeber);
	SpriteData* PopSubTex(const rapidjson::Value& subTextureArrayMemeber);
	gef::Sprite GetSprite();

private:

	//Map to hold the sub textures
	std::unordered_map<std::string,SpriteData> subTextures;

	//Animation data for the two animation
	SkelData boyAnimData, boyFallAnimData;

	//Gef variables
	gef::SpriteRenderer* renderer;
	gef::Font* font;
	gef::InputManager* inputManager;

	//Sprite textures and data
	gef::Texture* spriteTex, *fallSpriteTex;
	gef::Sprite sprite;


	//Documents to hold JSON document 
	rapidjson::Document boyAnimJSON, boySkelJSON, boyFallAnimJSON, boyFallSkelJSON;
	const char *texJSONStorage;

	//Timer for the frame updates
	float  timer = 0;

	//Animation frame counter
	short boyFrameNumber, fallFrameNumber;
	int  currentFrame = 0;

	//bool to control the animation
	bool falling = false;

	//sheet data for the sprite sheets
	SheetData attackData, fallData;





};

