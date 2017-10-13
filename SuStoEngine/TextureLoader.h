#ifndef __TEXTURELOADER_H__
#define __TEXTURELOADER_H__

#include "Module.h"

class Texture
{
public:
	Texture(uint id, uint width, uint height);
	
	uint GetId();

private:
	uint id = 0;
	uint width = 0;
	uint height = 0;
};

class TextureLoader : public Module
{
public:
	TextureLoader(bool start_enabled = true);
	~TextureLoader();

	bool Awake();
	bool Start();
	bool Update();
	bool CleanUp();

	bool LoadTexture(const char* full_path);

private:
	vector<Texture*> textures;

public:

};

#endif // __TEXTURELOADER_H__