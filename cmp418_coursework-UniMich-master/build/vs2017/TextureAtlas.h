#pragma once
#include <string>
#include <vector>
#include "load_texture.h"
#include "load_json.h"
#include "rapidjson\document.h"
#include "SubTextureStruct.h"
#include "Skin.h"
#include <graphics/sprite.h>
#include <unordered_map>


//Sprite texture and animation atlas
class TextureAtlas
{
private:
	//JSON DATA

	char* texJSON;

	std::string imagePath, name;
	int width, height;
	std::vector <std::string> partNames;

	gef::Sprite sprite;
	gef::Texture* sprite_texture;
	
public:

	//Texture initialisation
	void initTex(gef::Platform& platform);

	//File Read/Write functions
	void LoadJSONFromFile(const char* fileName);
	void LoadSkelJSONFromFile(const char* fileName);
	void ReadJSONAndPopulate(std::vector <std::string> &partNames);
	void ReadSkelJSONAndPopulate();
	SubTexture* ReadSubTextureAndPopulate(const rapidjson::Value& subTextureArrayMemeber);
	Skin* ReadSkinAndPopulate(const rapidjson::Value& skinArrayMemeber);
	Bone* ReadBoneAndPopulate(const rapidjson::Value& boneArrayMemeber);

	//Sprite update function
	void UpdateSprite(gef::Platform& platform, std::string part);

	//Sprite bone transform functions
	void BuildBoneTransforms();

	//Getter
	gef::Sprite GetSprite();

	//Maps
	std::unordered_map<std::string, SubTexture> subTexMap;
	std::unordered_map<std::string, Skin> skinMap;
	std::unordered_map<std::string, Bone> boneMap;

	//JSon files
	rapidjson::Document texJSONDocument, skelJSONDocument;
};

