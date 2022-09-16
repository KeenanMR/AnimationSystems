#include "TextureAtlas.h"

//Initialises texture
void TextureAtlas::initTex(gef::Platform& platform)
{
		sprite_texture = CreateTextureFromPNG("Dragon_tex.png", platform);
		sprite.set_texture(sprite_texture);
		
}

//Load JSON tex file
void TextureAtlas::LoadJSONFromFile(const char* fileName)
{
	texJSON = LoadJSON(fileName);

	texJSONDocument.Parse(texJSON);
	
}

//Load JSON ske file
void TextureAtlas::LoadSkelJSONFromFile(const char* fileName)
{
	texJSON = LoadJSON(fileName);

	skelJSONDocument.Parse(texJSON);
}

void TextureAtlas::ReadJSONAndPopulate(std::vector <std::string> &partNames)
{
	//Populate String Values
	name = texJSONDocument["name"].GetString();
	imagePath = texJSONDocument["imagePath"].GetString();


	//Populate Integer Values
	width = texJSONDocument["width"].GetInt();
	height = texJSONDocument["height"].GetInt();

	//Populate Subtextures
	const rapidjson::Value& subtextureArray = texJSONDocument["SubTexture"];

	for (int i = 0; i < subtextureArray.Size(); i++)
	{
		SubTexture* subTextureTemp = ReadSubTextureAndPopulate(subtextureArray[i]);


		std::pair<std::string, SubTexture> pair;
		pair.first = subTextureTemp->name;
		pair.second = *subTextureTemp;
		partNames.push_back(pair.first);
		subTexMap.insert(pair);
		delete subTextureTemp;
	}

}

void TextureAtlas::ReadSkelJSONAndPopulate()
{
	//Iterate and populate the skin data
	const rapidjson::Value& skinDoc = skelJSONDocument["armature"][0]["skin"][0]["slot"];

	for (int i = 0; i < skinDoc.Size(); i++)
	{
		Skin* skinTemp = ReadSkinAndPopulate(skinDoc[i]);
		std::pair<std::string, Skin> pair;
		pair.first = skinTemp->name;
		pair.second = *skinTemp;

		skinMap.insert(pair);
		delete skinTemp;
	}

	//Iterate and populate the bone data
	const rapidjson::Value& boneDoc = skelJSONDocument["armature"][0]["bone"];

	for (int i = 0; i < boneDoc.Size(); i++)
	{
		Bone* boneTemp = ReadBoneAndPopulate(boneDoc[i]);
		std::pair<std::string, Bone> pair;
		pair.first = "parts/" + boneTemp->name;
		pair.second = *boneTemp;

		boneMap.insert(pair);
		delete boneTemp;
	}

	//Set parent bone based on key data
	std::unordered_map<std::string, Bone>::iterator it = boneMap.begin();

	while (it != boneMap.end())
	{
		if (it->second.parent != "")
		{
			it->second.parentBone = &boneMap.at("parts/" + it->second.parent);
		}

		it++;

	}



}

//Iterate and update world bone transforms
void TextureAtlas::BuildBoneTransforms()
{
	std::unordered_map<std::string, Bone>::iterator it = boneMap.begin();

	while (it != boneMap.end())
	{
		gef::Matrix33 worldTransform;
		worldTransform.SetIdentity();
		it->second.localTransform.Rotate(gef::DegToRad(it->second.skX + it->second.animRot));
		it->second.localTransform.SetTranslation(gef::Vector2(it->second.x, it->second.y) + it->second.animTrans);
		worldTransform = it->second.localTransform;

		Bone* parentPointer = it->second.parentBone;

		while (parentPointer != NULL)
		{
			worldTransform = worldTransform * parentPointer->localTransform;
			parentPointer = parentPointer->parentBone;
		}

		it->second.worldTransform = worldTransform;

		it++;

	}
}

//Reads the subtexture data and returns a subtexture variable
SubTexture* TextureAtlas::ReadSubTextureAndPopulate(const rapidjson::Value& subTextureArrayMemeber)
{

	SubTexture* tempSubTexture = new SubTexture();

	

	if(subTextureArrayMemeber.HasMember("name"))
	tempSubTexture->name = subTextureArrayMemeber["name"].GetString();

	if (subTextureArrayMemeber.HasMember("frameY"))
		tempSubTexture->frameY = subTextureArrayMemeber["frameY"].GetInt();
	else
		tempSubTexture->frameY = 0;

	if (subTextureArrayMemeber.HasMember("frameX"))
	tempSubTexture->frameX = subTextureArrayMemeber["frameX"].GetInt();
	else
		tempSubTexture->frameX = 0;

	if (subTextureArrayMemeber.HasMember("y"))
		tempSubTexture->Y = subTextureArrayMemeber["y"].GetInt();
	else
		tempSubTexture->Y = 0;

	if (subTextureArrayMemeber.HasMember("x"))
		tempSubTexture->X = subTextureArrayMemeber["x"].GetInt();
	else
		tempSubTexture->X = 0;

	if (subTextureArrayMemeber.HasMember("frameHeight"))
		tempSubTexture->frameHeight = subTextureArrayMemeber["frameHeight"].GetInt();
	else
		tempSubTexture->frameHeight = subTextureArrayMemeber["height"].GetInt();

	if (subTextureArrayMemeber.HasMember("frameWidth"))
		tempSubTexture->frameWidth = subTextureArrayMemeber["frameWidth"].GetInt();
	else
		tempSubTexture->frameWidth = subTextureArrayMemeber["width"].GetInt();

	if (subTextureArrayMemeber.HasMember("width"))
		tempSubTexture->width = subTextureArrayMemeber["width"].GetInt();
	else
		tempSubTexture->width = 0;

	if (subTextureArrayMemeber.HasMember("height"))
		tempSubTexture->height = subTextureArrayMemeber["height"].GetInt();
	else
		tempSubTexture->height = 0;

	tempSubTexture->BuildTransform();

	return tempSubTexture;
}

//Reads the skin data and returns a skin variable
Skin* TextureAtlas::ReadSkinAndPopulate(const rapidjson::Value& skinArrayMemeber)
{
	Skin* tempSkin = new Skin();

	if (skinArrayMemeber["display"][0].HasMember("name"))
		tempSkin->name = skinArrayMemeber["display"][0]["name"].GetString();

	if (skinArrayMemeber["display"][0]["transform"].HasMember("x"))
		tempSkin->x = skinArrayMemeber["display"][0]["transform"]["x"].GetFloat();

	if (skinArrayMemeber["display"][0]["transform"].HasMember("y"))
		tempSkin->y = skinArrayMemeber["display"][0]["transform"]["y"].GetFloat();

	if (skinArrayMemeber["display"][0]["transform"].HasMember("skX"))
		tempSkin->skX = skinArrayMemeber["display"][0]["transform"]["skX"].GetFloat();
	else
		tempSkin->skX = 0;

	tempSkin->BuildTransform();

	return tempSkin;
}

//Reads the bone data and returns a bone variable
Bone* TextureAtlas::ReadBoneAndPopulate(const rapidjson::Value& boneArrayMemeber)
{
	Bone* tempBone = new Bone();

	if (boneArrayMemeber.HasMember("name"))
		tempBone->name = boneArrayMemeber["name"].GetString();

	if (boneArrayMemeber.HasMember("parent"))
		tempBone->parent = boneArrayMemeber["parent"].GetString();
	if (boneArrayMemeber.HasMember("transform"))
	{
		if (boneArrayMemeber["transform"].HasMember("x"))
			tempBone->x = boneArrayMemeber["transform"]["x"].GetFloat();

		if (boneArrayMemeber["transform"].HasMember("y"))
			tempBone->y = boneArrayMemeber["transform"]["y"].GetFloat();

		if (boneArrayMemeber["transform"].HasMember("skX"))
			tempBone->skX = boneArrayMemeber["transform"]["skX"].GetFloat();
	}
	tempBone->BuildLocalTransform();
	

	return tempBone;
}

//Updates the sprite data
void TextureAtlas::UpdateSprite(gef::Platform& platform,std::string part)
{

	sprite.set_width(subTexMap.at(part).width);
	sprite.set_height(subTexMap.at(part).height);
	sprite.set_uv_height(subTexMap.at(part).height/height);
	sprite.set_uv_width(subTexMap.at(part).width/width);

	float u = subTexMap.at(part).X / width;
	float v = subTexMap.at(part).Y / height;
	sprite.set_uv_position(gef::Vector2(u, v));

	float sprite_x = subTexMap.at(part).width * 0.5f - subTexMap.at(part).frameWidth * 0.5f + subTexMap.at(part).frameX;
	float sprite_y = subTexMap.at(part).height * 0.5f - subTexMap.at(part).frameHeight * 0.5f + subTexMap.at(part).frameY;

	sprite.set_position(gef::Vector4(platform.width() * 0.5f + sprite_x, platform.height() * 0.5f + sprite_y, 0.0f));


}

//Sprite getter
gef::Sprite TextureAtlas::GetSprite()
{
	return sprite;
}

