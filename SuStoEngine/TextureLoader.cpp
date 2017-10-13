#include "TextureLoader.h"
#include "App.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "ModuleGameObject.h"
#include "ComponentTexture.h"

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
	
	ILuint id;				
	GLuint textureID;								

	if (ilLoad(IL_TYPE_UNKNOWN, full_path))
	{
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

		textureID = App->renderer3D->LoadTextureBuffer(ilGetData(), 1, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
			GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);

		Texture* texture = new Texture(textureID, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));

		ilDeleteImages(1, &id);

		// CUSTOM GAME OBJECT TEXTURE LOADING FOR THIS ASSIGNMENT
		vector<GameObject*> go = App->gameobj->GetListGameObjects();

		for (vector<GameObject*>::iterator it = go.begin(); it != go.end(); it++)
		{
			(*it)->AddComponent(TEXTURE);
			ComponentTexture* comp = (ComponentTexture*)(*it)->FindComponentByType(TEXTURE);

			comp->SetTexture(texture);
		}

		// ------------------------------------------------------
	}
	else
	{
		LOG_OUTPUT("Cannot load image %s. Error: %s", full_path, iluErrorString(ilGetError()));
	}

	return ret;
}

Texture::Texture(uint _id, uint _width, uint _height)
{
	id = _id;
	width = _width;
	height = _height;
}

uint Texture::GetId()
{
	return id;
}
