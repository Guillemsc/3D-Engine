#include "TextureLoader.h"
#include "App.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "ModuleGameObject.h"
#include "ComponentTexture.h"
#include "Functions.h"

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

void TextureLoader::OnLoadFile(const char * file_path, const char * file_name, const char * file_extension)
{
	if (TextCmp("png", file_extension))
	{
		LoadTexture(file_path);
	}
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

		textures.push_back(texture);

		// CUSTOM GAME OBJECT TEXTURE LOADING FOR THIS ASSIGNMENT
		vector<GameObject*> go = App->gameobj->GetListGameObjects();

		for (vector<GameObject*>::iterator it = go.begin(); it != go.end(); it++)
		{
			(*it)->RemoveComponent(TEXTURE);
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

void TextureLoader::UnloadTexture(Texture * text)
{
	for (vector<Texture*>::iterator it = textures.begin(); it != textures.end(); it++)
	{
		if ((*it) == text)
		{
			if(text->GetId() != 0)
				App->renderer3D->UnloadTextureBuffer(text->GetId(), 1);

			text->CleanUp();
			textures.erase(it);
			break;
		}
	}
}

Texture::Texture(uint _id, uint _width, uint _height)
{
	id = _id;
	width = _width;
	height = _height;
}

bool Texture::operator==(Texture * text)
{
	bool ret = false;

	if (id == text->id && width == text->width && height == text->height)
		ret = true;

	return ret;
}

void Texture::CleanUp()
{
}

uint Texture::GetId()
{
	return id;
}
