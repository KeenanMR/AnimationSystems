#include "TextureAtlasSpriteSheet.h"


//Constructor
TextureAtlasSpriteSheet::TextureAtlasSpriteSheet() :
	renderer(NULL),
	inputManager(NULL),
	font(NULL),
	spriteTex(NULL)
{

}

//Setup and intitial values for thr sprite sheet texture atlas
void TextureAtlasSpriteSheet::init(gef::Platform& platform)
{

	renderer = gef::SpriteRenderer::Create(platform);
	inputManager = gef::InputManager::Create(platform);

	//Load sprite sheets
	spriteTex = CreateTextureFromPNG("boy-attack_tex.png", platform);
	sprite.set_texture(spriteTex);
	fallSpriteTex = CreateTextureFromPNG("boy-fall_tex.png", platform);
	sprite.set_texture(fallSpriteTex);

	//read tex jsons and populate
	texJSONStorage = LoadJSON("boy-fall_tex.json");
	boyFallAnimJSON.Parse(texJSONStorage);
	ReadPopJSON(boyFallAnimJSON,fallData);
	texJSONStorage = LoadJSON("boy-attack_tex.json");
	boyAnimJSON.Parse(texJSONStorage);
	ReadPopJSON(boyAnimJSON,attackData);


	//read ske jsons and populate
	texJSONStorage = LoadJSON("boy-fall_ske.json");
	boyFallSkelJSON.Parse(texJSONStorage);
	ReadPopSkel(boyFallSkelJSON,boyFallAnimData);
	texJSONStorage = LoadJSON("boy-attack_ske.json");
	boyAnimJSON.Parse(texJSONStorage);
	ReadPopSkel(boyAnimJSON, boyAnimData);

	//sets the number 
	boyFrameNumber = boyAnimData.frameOrder.size();
	fallFrameNumber= boyFallAnimData.frameOrder.size();
}


//Update the animation frames from the loaded data
void TextureAtlasSpriteSheet::Update(gef::Platform& platform, float frame_time)
{
	SkelData skelData;
	short frameSize = 0;
	SheetData sheet;

	//Sets the data to use based on the animation played
	if (falling)
	{
		skelData = boyFallAnimData;
		frameSize = fallFrameNumber;
		sprite.set_texture(fallSpriteTex);
		sheet = fallData;
	}
	else
	{
		skelData = boyAnimData;
		frameSize = boyFrameNumber;
		sprite.set_texture(spriteTex);
		sheet = attackData;
	}

	//Update the animation with the read framerate
	if (timer >= 1.f / skelData.frameRate)
	{

		SpriteData data = subTextures.at(skelData.frameOrder.at(currentFrame));

		sprite.set_width(data.width);
		sprite.set_height(data.height);
		sprite.set_uv_height(data.height / sheet.height);
		sprite.set_uv_width(data.width / sheet.width);

		float u = data.X / sheet.width;
		float v = data.Y / sheet.height;
		sprite.set_uv_position(gef::Vector2(u, v));

		float sprite_x = data.width * 0.5f - (data.frameWidth * 0.5f + data.frameX);
		float sprite_y = data.height * 0.5f - (data.frameHeight * 0.5f + data.frameY);

		sprite.set_position(gef::Vector4(platform.width() * 0.5f + sprite_x, platform.height() * 0.5f + sprite_y, 0.0f));

		currentFrame++;
		if (currentFrame > frameSize - 1)
		{
			currentFrame = 0;
		}

		timer = 0;
	}

	timer += frame_time;
	
}

//Render the sprite 
void TextureAtlasSpriteSheet::Render()
{
	renderer->Begin();

	renderer->DrawSprite(sprite);

	renderer->End();
}

//Cleanup
void TextureAtlasSpriteSheet::CleanUp()
{
	delete renderer;
	renderer = NULL;

	delete font;
	font = NULL;

	delete inputManager;
	inputManager = NULL;

	delete spriteTex;
	spriteTex = NULL;

	delete fallSpriteTex;
	fallSpriteTex = NULL;

	delete texJSONStorage;
	texJSONStorage = NULL;

}

//Input function to swap animations 
void TextureAtlasSpriteSheet::Input()
{
	if (inputManager)
	{
		inputManager->Update();
		
		gef::Keyboard* keyboard = inputManager->keyboard();
		if (keyboard)
		{
			if (keyboard->IsKeyPressed(gef::Keyboard::KC_SPACE))
			{
				falling = !falling;

				//resets frames to start
				currentFrame = 0;
			}
		}
	}
}

//Populate the tex json data structure
SpriteData* TextureAtlasSpriteSheet::PopSubTex(const rapidjson::Value& subTextureArrayMemeber)
{
	SpriteData* tempSpriteData = new SpriteData;

	if (subTextureArrayMemeber.HasMember("name"))
		tempSpriteData->name = subTextureArrayMemeber["name"].GetString();

	if (subTextureArrayMemeber.HasMember("frameY"))
		tempSpriteData->frameY = subTextureArrayMemeber["frameY"].GetInt();
	else
		tempSpriteData->frameY = 0;

	if (subTextureArrayMemeber.HasMember("frameX"))
		tempSpriteData->frameX = subTextureArrayMemeber["frameX"].GetInt();
	else
		tempSpriteData->frameX = 0;

	if (subTextureArrayMemeber.HasMember("y"))
		tempSpriteData->Y = subTextureArrayMemeber["y"].GetInt();
	else
		tempSpriteData->Y = 0;

	if (subTextureArrayMemeber.HasMember("x"))
		tempSpriteData->X = subTextureArrayMemeber["x"].GetInt();
	else
		tempSpriteData->X = 0;

	if (subTextureArrayMemeber.HasMember("frameHeight"))
		tempSpriteData->frameHeight = subTextureArrayMemeber["frameHeight"].GetInt();
	else
		tempSpriteData->frameHeight = subTextureArrayMemeber["height"].GetInt();

	if (subTextureArrayMemeber.HasMember("frameWidth"))
		tempSpriteData->frameWidth = subTextureArrayMemeber["frameWidth"].GetInt();
	else
		tempSpriteData->frameWidth = subTextureArrayMemeber["width"].GetInt();

	if (subTextureArrayMemeber.HasMember("width"))
		tempSpriteData->width = subTextureArrayMemeber["width"].GetInt();
	else
		tempSpriteData->width = 0;

	if (subTextureArrayMemeber.HasMember("height"))
		tempSpriteData->height = subTextureArrayMemeber["height"].GetInt();
	else
		tempSpriteData->height = 0;

	return tempSpriteData;
}

//Getter for sprite
gef::Sprite TextureAtlasSpriteSheet::GetSprite()
{
	return sprite;
}

//Read and populate the tex JSon file
void TextureAtlasSpriteSheet::ReadPopJSON(rapidjson::Document& texDoc, SheetData& sheetData)
{

	//Populate the rapidJSON document from the boy attack JSON
	if (texDoc.HasMember("name"))
		sheetData.name = texDoc["name"].GetString();

	if (texDoc.HasMember("width"))
		sheetData.width = texDoc["width"].GetInt();

	if (texDoc.HasMember("height"))
		sheetData.height = texDoc["height"].GetInt();

	//Populate Subtextures
	const rapidjson::Value& subtextureArray = texDoc["SubTexture"];
	
		for (int i = 0; i < subtextureArray.Size(); i++)
		{
			SpriteData* newSpriteData = PopSubTex(subtextureArray[i]);

			std::pair<const char*, SpriteData> pair;
			pair.first = newSpriteData->name;
			pair.second = *newSpriteData;
			subTextures.insert(pair);
			delete newSpriteData;
		}

}

//Read and populate the ske Json file
void TextureAtlasSpriteSheet::ReadPopSkel(rapidjson::Document& skelDoc, SkelData& dataStruct)
{
	if (skelDoc["armature"][0].HasMember("frameRate"))
		dataStruct.frameRate = skelDoc["armature"][0]["frameRate"].GetInt();

	const rapidjson::Value& subtextureArray = skelDoc["armature"][0]["skin"][0] ["slot"][0]["display"] ;
	for (int i = 0; i < subtextureArray.Size(); i++)
	{
		dataStruct.frameOrder.push_back(PopAnimOrder(subtextureArray[i]));
	}
}


//Get the order of anmimations from the ske json file
const char* TextureAtlasSpriteSheet::PopAnimOrder(const rapidjson::Value& subTextureArrayMemeber)
{
	const char* orderName = "";

	if (subTextureArrayMemeber.HasMember("name"))
		orderName = subTextureArrayMemeber["name"].GetString();

	return orderName;
}
