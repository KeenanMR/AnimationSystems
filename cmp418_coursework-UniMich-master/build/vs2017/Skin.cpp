#include "Skin.h"


//Build skin transform based on key data
void Skin::BuildTransform()
{
	skinRotation.SetIdentity();
	skinTransform.SetIdentity();
	skinTranslation.SetIdentity();

	skinRotation.Rotate(gef::DegToRad(skX));
	skinTranslation.SetTranslation(gef::Vector2(x, y));

	skinTransform = skinRotation * skinTranslation;

};

//Builds the local bone based on skeleton data
void Bone::BuildLocalTransform()
{
	boneRotate.SetIdentity();
	localTransform.SetIdentity();
	boneTranslate.SetIdentity();

	boneRotate.Rotate(gef::DegToRad(skX));
	boneTranslate.SetTranslation(gef::Vector2(x, y));

	localTransform = boneRotate * boneTranslate;


}