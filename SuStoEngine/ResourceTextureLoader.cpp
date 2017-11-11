#include "ResourceTextureLoader.h"
#include "App.h"
#include "Functions.h"
#include "ResourceTexture.h"
#include "ResourceManager.h"
#include "Globals.h"
#include "ModuleFileSystem.h"
#include "JSONLoader.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

ResourceTextureLoader::ResourceTextureLoader()
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
 
}

ResourceTextureLoader::~ResourceTextureLoader()
{
}

ResourceTexture* ResourceTextureLoader::Load(const char * filepath)
{
	ResourceTexture* ret = nullptr;

	// Load texture
	if (ilLoad(IL_TYPE_UNKNOWN, filepath))
	{
		// Get file name
		string file_name = GetFileNameFromFilePath(filepath);

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

		// Create texture
		ret = (ResourceTexture*)App->resource_manager->CreateNewResource(RT_TEXTURE);
		ret->SetData(ilGetData(), ilGetInteger(IL_IMAGE_SIZE_OF_DATA), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT),
			GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
		ret->SetName(file_name);

		ilDeleteImages(1, &ImageInfo.Id);

		// Export it to Library
		App->resource_manager->SaveResourceIntoFile(ret);
	}
	else
	{
		LOG_OUTPUT("Cannot load image %s. Error: %s", filepath, iluErrorString(ilGetError()));
	}

	return ret;
}

void ResourceTextureLoader::ImportAllTextures()
{
	vector<string> files = App->file_system->GetFilesInPath(App->file_system->GetLibraryTexturePath().c_str(), "dds");

	for (vector<string>::iterator it = files.begin(); it != files.end(); it++)
	{
		Import((*it).c_str());
	}
}

void ResourceTextureLoader::Import(const char * filepath)
{
	string path = GetPathFromFilePath(filepath);
	string filename = GetFileNameFromFilePath(filepath);
	string name = GetFilenameWithoutExtension(filename.c_str());

	// -------------------------------------
	// META --------------------------------
	// -------------------------------------
	string meta_name = path + name + ".meta";
	JSON_Doc* doc = App->json->LoadJSON(meta_name.c_str());
	string uid = doc->GetString("uid", "no_uid");

	// -------------------------------------
	// FILE --------------------------------
	// -------------------------------------
	// Load texture
	if (ilLoad(IL_TYPE_UNKNOWN, filepath))
	{
		// Get file name
		string file_name = GetFileNameFromFilePath(filepath);

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

		// Create texture
		ResourceTexture* ret = (ResourceTexture*)App->resource_manager->CreateNewResource(RT_TEXTURE, uid.c_str());
		ret->SetData(ilGetData(), ilGetInteger(IL_IMAGE_SIZE_OF_DATA), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT),
			GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
		ret->SetName(file_name);

		ilDeleteImages(1, &ImageInfo.Id);
	}
	else
	{
		LOG_OUTPUT("Cannot load image %s. Error: %s", filepath, iluErrorString(ilGetError()));
	}

	App->json->UnloadJSON(doc);
}

bool ResourceTextureLoader::Export(const char * path, ResourceTexture* resource)
{
	bool ret = true;

	string name = resource->GetUniqueId();

	// -------------------------------------
	// FILE --------------------------------
	// -------------------------------------
	uint size = 0;
	byte* data = nullptr;

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

	// -------------------------------------
	// META --------------------------------
	// -------------------------------------
	string meta_name = path + name + ".meta";

	JSON_Doc* doc = App->json->LoadJSON(meta_name.c_str());
	if (doc == nullptr)
		doc = App->json->CreateJSON(meta_name.c_str());

	if (doc != nullptr)
	{
		doc->Clear();

		doc->SetString("uid", resource->GetUniqueId().c_str());

		doc->Save();
	}

	App->json->UnloadJSON(doc);

	return ret;
}
