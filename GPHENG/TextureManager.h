#pragma once
#include "ResourceManager.h"
#include "Texture.h"

class TextureManager: public ResourceManager
{
public:
	TextureManager();
	~TextureManager();
	TexturePtr createTextureFromFile(const wchar_t* filePath);
protected:
	virtual Resource* createResourceFromFileConcrete(const wchar_t* filePath);
};