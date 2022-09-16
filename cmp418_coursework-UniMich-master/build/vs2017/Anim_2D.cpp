#include "Anim_2D.h"

//Constructer
Anim_2D::Anim_2D() :
	renderer(NULL),
	inputManager(NULL)
{
}

//Initialiser 
void Anim_2D::init(gef::Platform& platform)
{
	
	//Initialise input manager
	inputManager = gef::InputManager::Create(platform);


	renderer = gef::SpriteRenderer::Create(platform);
	sprite_pos_ = gef::Vector2(platform.width() * 0.5f, platform.height() * 0.5f);

	//Load tex and skelton JSONs and save the data
	tex.LoadJSONFromFile("Dragon_tex.json");
	tex.ReadJSONAndPopulate(partNames);
	tex.LoadSkelJSONFromFile("Dragon_ske.json");
	tex.ReadSkelJSONAndPopulate();

	//Reads and Populates animation keys
	ReadKeys();

	//Initialises texture
	tex.initTex(platform);
}


//Function to update sprite transforms
void Anim_2D::SpriteTransform(gef::Platform& platform)
{
	//Rebuilds bone transforms before update
	tex.BuildBoneTransforms();

	timer += frameTimeLocal;
	for (int i = 0; i < partNames.size(); i++)
	{

		//Set bone rotate and translation for animation
		if (timer >= 1.f / frameRate)
		{
			tex.boneMap.at(partNames[i]).animRot = AnimRotate(partNames[i]);
			tex.boneMap.at(partNames[i]).animTrans = AnimTranslate(partNames[i]);
		}



		//Initialise transform
		fullTransform.SetIdentity();
		fullTransform.Scale(gef::Vector2(0.5, 0.5));
		fullTransform.SetTranslation(gef::Vector2(sprite_pos_.x, sprite_pos_.y));

		//Get sprite part transforms
		gef::Matrix33 subTexTransform = tex.subTexMap.at(partNames.at(i)).subTransform;
		gef::Matrix33 skinTransform = tex.skinMap.at(partNames.at(i)).skinTransform;
		gef::Matrix33 boneTransform = tex.boneMap.at(partNames.at(i)).worldTransform;

		//Calculate and apply final transforms
		fullTransform = subTexTransform * skinTransform * boneTransform * fullTransform;
		tex.UpdateSprite(platform, partNames.at(i));
		renderer->DrawSprite(tex.GetSprite(), fullTransform);
	}

}

//Reset anims to default positions when changed
void Anim_2D::ResetAnims()
{
	for (int i = 0; i < partNames.size(); i++)
	{
			tex.boneMap.at(partNames[i]).animRot = 0;
			tex.boneMap.at(partNames[i]).animTrans = gef::Vector2(0,0);
	}
}


//Translates the bones based on key data
gef::Vector2 Anim_2D::AnimTranslate(std::string currentBone)
{
	//Erases the "parts/" section of the bone name to align with the boneKeys 
	std::string currentBoneName = currentBone;
	currentBoneName.erase(0, 6);

	//Iterates and updates bone transformations
	for (auto& boneKey : animMap.at(currentAnim).boneKeys)
	{
		if (boneKey->name == currentBoneName)
		{

			for (int currTran = 0; currTran < boneKey->translationKeys.size(); currTran++)
			{
				int nextKey;
				if (currTran >= boneKey->translationKeys.size() - 1)
				{
					nextKey = 0;
				}
				else
				{
					nextKey = currTran + 1;
				}

				if (currentFrame >= boneKey->translationKeys.at(currTran)->startTimer && currentFrame < boneKey->translationKeys.at(nextKey)->startTimer)
				{
					float time = (currentFrame - boneKey->translationKeys.at(currTran)->startTimer) / (boneKey->translationKeys.at(nextKey)->startTimer - boneKey->translationKeys.at(currTran)->startTimer);
					gef::Vector2 animPos = posLerp(boneKey->translationKeys.at(currTran)->translationVector, boneKey->translationKeys.at(nextKey)->translationVector, time);

					return animPos;
				}
				else if (currTran == nextKey)
				{
					gef::Vector2 animPos;
					animPos.x = boneKey->translationKeys.at(currTran)->translationVector.x;
					animPos.y = boneKey->translationKeys.at(currTran)->translationVector.y;

					return animPos;
				}

			}

		}
	}

	//Default return value if no change is made
	return tex.boneMap.at(currentBone).animTrans;

}

//Rotates the animation bones based on key data
float Anim_2D::AnimRotate(std::string currentBone)
{
	//Erases the "parts/" section of the bone name to align with the boneKeys 
	std::string currentBoneName = currentBone;
	currentBoneName.erase(0, 6);

	//Iterates and updates bone rotations
	for (auto& boneKey : animMap.at(currentAnim).boneKeys)
	{
		if (boneKey->name == currentBoneName)
		{

			if (animIterator > 0)
			{
				animIterator = animIterator;
			}
			for (int currRot = 0; currRot < boneKey->rotationKeys.size(); currRot++)
			{
				int nextKey;
				if (currRot >= boneKey->rotationKeys.size() - 1)
				{
					nextKey = 0;
				}
				else
				{
					nextKey = currRot + 1;
				}

				if (currentFrame >= boneKey->rotationKeys.at(currRot)->startTimer && currentFrame < boneKey->rotationKeys.at(nextKey)->startTimer)
				{
					float time = (currentFrame - boneKey->rotationKeys.at(currRot)->startTimer) / (boneKey->rotationKeys.at(nextKey)->startTimer - boneKey->rotationKeys.at(currRot)->startTimer);
					float animRot = rotLerp(boneKey->rotationKeys.at(currRot)->rotation, boneKey->rotationKeys.at(nextKey)->rotation, time);

					return animRot;
				}
				else if (currRot == nextKey)
				{
					float animRot = boneKey->rotationKeys.at(currRot)->rotation;

					return animRot;
				}

			}

		}
	}

	//Default return value
	return tex.boneMap.at(currentBone).animRot;

}
	
//Internal class renderer
void Anim_2D::Render2D(gef::Platform& platform, bool render)
{
	renderer->Begin();

	if(render)
	SpriteTransform(platform);

	
	renderer->End();
}

//Reads the animation keys
void Anim_2D::ReadKeys()
{

	//Read animations frameRate
	const rapidjson::Value& frameDoc = tex.skelJSONDocument["armature"];

	if (frameDoc[0].HasMember("frameRate"))
		frameRate = frameDoc[0]["frameRate"].GetInt();

	const rapidjson::Value& keyDoc = tex.skelJSONDocument["armature"][0]["animation"];

	//Loops through all animations and inserts them into a map
	for (int i = 0; i < keyDoc.Size(); i++)
	{
		Anim* tempAnim = PopulateKeys(keyDoc[i]);

		std::pair<std::string, Anim> pair;
		pair.first = tempAnim->name;
		pair.second = *tempAnim;
		animNames.push_back(pair.first);

		animMap.insert(pair);
	}

	//Sets the current animation to the first loaded animation
	currentAnim = animNames.at(0);


}

//Populates the animation keys
Anim* Anim_2D::PopulateKeys(const rapidjson::Value& keyMember)
{

	Anim* tempAnim = new Anim;

	//Get animation data
	if (keyMember.HasMember("duration"))
		tempAnim->duration = keyMember["duration"].GetFloat();

	if (keyMember.HasMember("name"))
		tempAnim->name = keyMember["name"].GetString();


	for (int boneIndex = 0; boneIndex < keyMember["bone"].Size(); boneIndex++)
	{
		BoneKey* tempBone = new BoneKey;

		//Get bone name
		if (keyMember["bone"][boneIndex].HasMember("name"))
			tempBone->name = keyMember["bone"][boneIndex]["name"].GetString();

		//Get translate frames
		if (keyMember["bone"][boneIndex].HasMember("translateFrame"))
		{
			for (int transIndex = 0; transIndex < keyMember["bone"][boneIndex]["translateFrame"].Size(); transIndex++)
			{
				TranslationKey* tempTranslate = new TranslationKey;

				if (keyMember["bone"][boneIndex]["translateFrame"][transIndex].HasMember("duration"))
					tempTranslate->nextKeyTimer = keyMember["bone"][boneIndex]["translateFrame"][transIndex]["duration"].GetFloat();

				if (keyMember["bone"][boneIndex]["translateFrame"][transIndex].HasMember("x"))
					tempTranslate->translationVector.x = keyMember["bone"][boneIndex]["translateFrame"][transIndex]["x"].GetFloat();
				else
					tempTranslate->translationVector.x = 0;

				if (keyMember["bone"][boneIndex]["translateFrame"][transIndex].HasMember("y"))
					tempTranslate->translationVector.y = keyMember["bone"][boneIndex]["translateFrame"][transIndex]["y"].GetFloat();
				else
					tempTranslate->translationVector.y = 0;

				tempBone->translationKeys.push_back(tempTranslate);
			}

			float timeTotal = 0;

			for (int i = 0; i < tempBone->translationKeys.size(); i++)
			{


				if (i == 0)
				{
					tempBone->translationKeys.at(i)->startTimer = 0;
					timeTotal += tempBone->translationKeys.at(i)->nextKeyTimer;
				}
				else
				{
					tempBone->translationKeys.at(i)->startTimer = timeTotal;
					timeTotal += tempBone->translationKeys.at(i)->nextKeyTimer;
				}


			}



		}
		//Get rotate frames
		if (keyMember["bone"][boneIndex].HasMember("rotateFrame"))
		{
			for (int rotateIndex = 0; rotateIndex < keyMember["bone"][boneIndex]["rotateFrame"].Size(); rotateIndex++)
			{
				RotationKey* tempRotation = new RotationKey;

				if (keyMember["bone"][boneIndex]["rotateFrame"][rotateIndex].HasMember("duration"))
					tempRotation->nextKeyTimer = keyMember["bone"][boneIndex]["rotateFrame"][rotateIndex]["duration"].GetFloat();

				if (keyMember["bone"][boneIndex]["rotateFrame"][rotateIndex].HasMember("rotate"))
					tempRotation->rotation = keyMember["bone"][boneIndex]["rotateFrame"][rotateIndex]["rotate"].GetFloat();
				else
					tempRotation->rotation = 0;


				tempBone->rotationKeys.push_back(tempRotation);
			}
		}

		float timeTotal = 0;

		for (int i = 0; i < tempBone->rotationKeys.size(); i++)
		{

			if (i == 0)
			{
				tempBone->rotationKeys.at(i)->startTimer = 0;
				timeTotal += tempBone->rotationKeys.at(i)->nextKeyTimer;
			}
			else
			{
				tempBone->rotationKeys.at(i)->startTimer = timeTotal;
				timeTotal += tempBone->rotationKeys.at(i)->nextKeyTimer;
			}
		}

		tempAnim->boneKeys.push_back(tempBone);

	}
	return tempAnim;
}

//Updates animations based on durations 
void Anim_2D::Update(float frameTime)
{
	frameTimeLocal = frameTime;
	fps_ = 1.0f / frameTime;

	if (timer >= 1.f / frameRate)
	{
		int currentAnimDuration = animMap.at(currentAnim).duration;
		currentFrame = currentFrame + 1 % currentAnimDuration;

		//Resets frames
		if (currentFrame > animMap.at(currentAnim).duration)
			currentFrame = 0;

		timer = 0;
	}

	inputManager->Update();
	
}

//Handles user input
void Anim_2D::Input()
{
	gef::Keyboard* keyboard = inputManager->keyboard();
	if (keyboard)
	{
		if (keyboard->IsKeyDown(gef::Keyboard::KC_ESCAPE))
		{
			exit(0);
		}

		if (keyboard->IsKeyPressed(gef::Keyboard::KC_SPACE))
		{
			if (animIterator < 3)
				animIterator++;
			else
				animIterator = 0;


			currentAnim = animNames.at(animIterator);

			ResetAnims();
		}
	}
}

void Anim_2D::CleanUp()
{
	delete inputManager;
	inputManager = NULL;

	delete renderer;
	renderer = NULL;
}



//Lerps between 2 animation positions
gef::Vector2 Anim_2D::posLerp(gef::Vector2(startPos), gef::Vector2(endPos), float time)
{
	float x = (1.f - time) * startPos.x + time * endPos.x;
	float y = (1.f - time) * startPos.y + time * endPos.y;

	return gef::Vector2(x, y);
}
//Lerps between 2 animation rotations
float Anim_2D::rotLerp(float startAngle, float endAngle, float time)
{
	float angleDiff = endAngle - startAngle;

	float angle;

	if (angleDiff > 180)
	{
		angleDiff -= 360;
	}
	else if (angleDiff < -180)
	{
		angleDiff += 360;
	}

	angle = startAngle + time * angleDiff;
	return angle;
}

//Get frame rate
int Anim_2D::GetFrameRate()
{
	return frameRate;
}


