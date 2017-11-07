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

	texture_importer = new TextureImporter();

	return ret;
}

bool TextureLoader::Start()
{
	bool ret = true;

	//texture_importer->ImportAllTextures();

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

	// Unload all textures
	UnloadAllFiles();

	return ret;
}

void TextureLoader::OnLoadFile(const char * file_path, const char * file_name, const char * file_extension)
{
	if (TextCmp("png", file_extension) || TextCmp("dds", file_extension))
	{
		LoadTexture(file_path);
	}
}

Texture* TextureLoader::LoadTexture(const char * full_path)
{
	Texture* ret = nullptr;											

	// Load texture
	if (ilLoad(IL_TYPE_UNKNOWN, full_path))
	{
		// Get file name
		string file_name = GetFileNameFromFilePath(full_path);

		// Get texture info
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		// Rotate if origin is upper left
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		// Convert image to rgb and a byte chain
		ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

		ILuint size = 0;
		size = ilSaveL(IL_RGB, NULL, 0);
		uint s = ImageInfo.SizeOfData;

		// Create texture
		Texture* texture = new Texture(ImageInfo.Data, size, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT),
			file_name.c_str(), GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);

		textures.push_back(texture);
		ret = texture;

		// Load texture to memory (temp?)
		texture->LoadToMemory();

		ilDeleteImages(1, &ImageInfo.Id);

		// Export it to Library
		texture_importer->Save(App->file_system->GetLibraryTexturePath().c_str(), texture);
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
			(*it)->CleanUp();

			delete(*it);
			textures.erase(it);
			break;
		}
	}
}

void TextureLoader::UnloadAllFiles()
{
	for (vector<Texture*>::iterator it = textures.begin(); it != textures.end(); it++)
	{	
		(*it)->CleanUp();

		delete(*it);
		it = textures.erase(it);	
	}
}

Texture::Texture(byte* _texture_data, uint _texture_data_lenght, uint _width, uint _height, int _format, const char * filename, uint _wrap_s, uint _wrap_t, uint _mag, uint _min)
{
	if (_texture_data_lenght > 0)
	{
		texture_data = new byte[_texture_data_lenght];
		memcpy(texture_data, _texture_data, _texture_data_lenght * sizeof(byte));
	}
	
	format = _format;
	size.x = _width;
	size.y = _height;
	wrap_s = _wrap_s;
	wrap_t = _wrap_t;
	mag = _mag;
	min = _min;

	file_name = filename;
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
	// Unload from memory
	UnloadFromMemory();

	// Unload texture data
	RELEASE_ARRAY(texture_data);
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

double Texture::GetUniqueId()
{
	return unique_id;
}

void Texture::SetUniqueId(double set)
{
	unique_id = set;
}

void Texture::LoadToMemory()
{
	if(id == 0 && texture_data != nullptr && size.x > 0 && size.y > 0)
		id = App->renderer3D->LoadTextureBuffer(texture_data, 1, format, size.x, size.y, wrap_s, wrap_t, mag, min);
}

void Texture::UnloadFromMemory()
{
	if(id != 0)
		App->renderer3D->UnloadTextureBuffer(id, 1);
}

Texture* TextureImporter::Load(const char * exported_file)
{
	Texture* ret = nullptr;

	App->LoadFile(exported_file);

	return ret;
}

bool TextureImporter::Save(const char * path, Texture* texture)
{
	bool ret = true;

	uint size = 0;
	byte* data = nullptr;

	string name = GetFilenameWithoutExtension(texture->GetFileName().c_str(), false);

	// To pick a specific DXT compression use
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);

	// Get the size of the data buffer
	size = ilSaveL(IL_DDS, NULL, 0);

	if (size > 0) 
	{
		// Allocate data buffer
		data = new byte[size]; 

		// Save to buffer with the ilSaveIL function
		if (ilSaveL(IL_DDS, data, size) > 0)
			ret = App->file_system->FileSave(path, (char*)data, name.c_str(), "dds", size);

		RELEASE_ARRAY(data);
	}
	
	return ret;
}

void TextureImporter::ImportAllTextures()
{
	vector<string> paths = App->file_system->GetFilesInPath(App->file_system->GetLibraryTexturePath().c_str(), "dds");

	for (vector<string>::iterator it = paths.begin(); it != paths.end(); it++)
	{
		App->LoadFile((*it).c_str());
	}
}
