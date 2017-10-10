#ifndef __TEXTURELOADER_H__
#define __TEXTURELOADER_H__

#include "Module.h"

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


public:

};

#endif // __TEXTURELOADER_H__