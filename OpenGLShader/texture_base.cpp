#include "texture_base.h"
#include "utils.h"

LH_NAMESPACE_BEGIN

TextureBase::TextureBase() : mImageData(nullptr), mWidth(0), mHeight(0), mTexture(0)
{

}

TextureBase::~TextureBase()
{

}

void TextureBase::Init(const char* const& bmpFilePath)
{
	mImageData = Utils::LoadBMP(bmpFilePath, mWidth, mHeight);
	GenTexture_i();
	DEL_PTR(mImageData);
}

void TextureBase::GenTexture_i()
{

}

LH_NAMESPACE_END