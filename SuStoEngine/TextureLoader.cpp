#include "TextureLoader.h"
#include "App.h"
#include "ModuleRenderer3D.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

TextureLoader::TextureLoader(bool start_enabled)
{
}

TextureLoader::~TextureLoader()
{
}

bool TextureLoader::Awake()
{
	bool ret = true;

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	return ret;
}

bool TextureLoader::Start()
{
	bool ret = true;

	//LoadTexture("Baker_house.png");

	return ret;
}

bool TextureLoader::Update()
{
	bool ret = true;

	return ret;
}

bool TextureLoader::CleanUp()
{
	bool ret = true;

	return ret;
}

bool TextureLoader::LoadTexture(const char * full_path)
{
	bool ret = true;
	
	uint* id = nullptr;

	ILuint image_id;
	ilGenImages(1, &image_id);
	ilBindImage(image_id);

	if (ilLoadImage(full_path))
	{
		App->renderer3D->LoadTextureBuffer(id, 1, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_MAG_FILTER, GL_TEXTURE_MIN_FILTER);

		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		ret = ilutGLBindTexImage();
		ilDeleteImages(1, &image_id);
	}
	else
	{
		LOG_OUTPUT("Cannot load image %s. Error: %s", full_path, iluErrorString(ilGetError()));
	}

	return ret;
}
