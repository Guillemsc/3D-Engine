#include "TextureLoader.h"
#include "App.h"
#include "ModuleRenderer3D.h"
#include "ModuleFileSystem.h"
#include "GameObject.h"
#include "ModuleGameObject.h"
#include "ComponentMaterial.h"
#include "Functions.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

TextureLoader::TextureLoader(bool start_enabled)
{
	SetName("TextureLoader");
}

TextureLoader::~TextureLoader()
{
}

bool TextureLoader::Awake()
{
	bool ret = true;

	LOG_OUTPUT("Loading TextureLoader Module");

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	return ret;
}

bool TextureLoader::Start()
{
	bool ret = true;

	TextureImporter importer;
	importer.Load(App->file_system->library_texture_path.c_str());

	return ret;
}

bool TextureLoader::Update()
{
	bool ret = true;

	textures;

	return ret;
}

bool TextureLoader::CleanUp()
{
	bool ret = true;

	TextureImporter importer;
	importer.Save(App->file_system->library_texture_path.c_str(), textures);

	for (vector<Texture*>::iterator it = textures.begin(); it != textures.end();)
	{
		if ((*it)->GetId() != 0)
			App->renderer3D->UnloadTextureBuffer((*it)->GetId(), 1);

		(*it)->CleanUp();
		delete (*it);

		it = textures.erase(it);
	}

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
		string file_name = GetFileNameFromFilePath(full_path);

		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

		textureID = App->renderer3D->LoadTextureBuffer(ilGetData(), 1, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
			GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);

		Texture* texture = new Texture(textureID, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), file_name.c_str());
		textures.push_back(texture);
		ilDeleteImages(1, &id);

		textures.push_back(texture);

		// CUSTOM GAME OBJECT TEXTURE LOADING FOR THIS ASSIGNMENT
		vector<GameObject*> go = App->gameobj->GetListGameObjects();

		for (vector<GameObject*>::iterator it = go.begin(); it != go.end(); it++)
		{
			(*it)->RemoveComponent(MATERIAL);
			(*it)->AddComponent(MATERIAL);
			ComponentMaterial* comp = (ComponentMaterial*)(*it)->FindComponentByType(MATERIAL);

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
			if((*it)->GetId() != 0)
				App->renderer3D->UnloadTextureBuffer((*it)->GetId(), 1);

			(*it)->CleanUp();

			delete(*it);
			textures.erase(it);
			break;
		}
	}
}

Texture::Texture(uint _id, uint _width, uint _height, const char * filename)
{
	file_name = filename;
	id = _id;
	size.x = _width;
	size.y = _height;
}

bool Texture::operator==(Texture* text)
{
	bool ret = false;

	if (id == text->id && size.x == text->size.x && size.y == text->size.y)
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

string Texture::GetFileName()
{
	return file_name;
}

float2 Texture::GetSize()
{
	return size;
}

void Texture::AddUser()
{
	used_by++;
}

void Texture::DeleteUser()
{
	used_by--;
}

int Texture::UsedBy()
{
	return used_by;
}

bool Texture::IsUsed()
{
	return used_by != 0 ? true : false;
}

bool TextureImporter::Import(const char * file, const char * path, std::string & output_file)
{
	bool ret = true;

	return ret;
}

bool TextureImporter::Import(const void * buffer, uint size, std::string & output_file)
{
	bool ret = true;

	return ret;
}

bool TextureImporter::Load(const char * exported_file)
{
	bool ret = true;

	return ret;
}

bool TextureImporter::Save(const char * path, vector<Texture*> textures)
{
	bool ret = true;

	for (vector<Texture*>::iterator texture = textures.begin(); texture != textures.end(); ++texture)
	{
		string name = GetFilenameWithoutExtension((*texture)->GetFileName().c_str(), false);
		ILuint size;
		ILubyte *data;
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
		size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
		if (size > 0) {
			data = new ILubyte[size]; // allocate data buffer
			if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
				ret = App->file_system->SaveFile(path, (char*)data, name.c_str(), "DDS", size);
			RELEASE_ARRAY(data);
		}
	}
	return ret;
}
