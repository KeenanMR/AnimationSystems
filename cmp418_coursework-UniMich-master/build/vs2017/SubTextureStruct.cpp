#include "SubTextureStruct.h"


//Build subtexture transform based on key data
void SubTexture::BuildTransform()
{
	subTransform.SetIdentity();
	subTranslate.SetIdentity();
	subScale.SetIdentity();

	float x = width * 0.5f - (frameWidth * 0.5f + frameX);
	float y = height * 0.5f - (frameHeight * 0.5f + frameY);

	subTranslate.SetTranslation(gef::Vector2(x, y));
	subScale.Scale(gef::Vector2(width, height));

	subTransform = subScale * subTranslate;
}