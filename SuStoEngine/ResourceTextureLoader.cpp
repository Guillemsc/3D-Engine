#include "ResourceTextureLoader.h"
#include "App.h"
#include "Functions.h"
#include "ResourceTexture.h"
#include "ResourceManager.h"
#include "Globals.h"
#include "ModuleFileSystem.h"
#include "JSONLoader.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "ModuleGameObject.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

ResourceTextureLoader::ResourceTextureLoader() : 
ResourceLoader(ResourceType::RT_TEXTURE, App->file_system->GetLibraryTexturePath())
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

ResourceTextureLoader::~ResourceTextureLoader()
{
}

Resource * ResourceTextureLoader::CreateResource(std::string new_uid)
{
	Resource* ret = nullptr;

	ret = new ResourceTexture(new_uid);

	return ret;
}

bool ResourceTextureLoader::LoadFileToEngine(DecomposedFilePath decomposed_file_path, std::vector<Resource*>& resources)
{
	bool ret = false;

	bool ret = ilLoad(IL_TYPE_UNKNOWN, decomposed_file_path.file_path.c_str());
	
	if (ret)
	{
		// Get texture info
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		// Convert image to rgb and a byte chain
		ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

		ILubyte* data = ilGetData();
		uint data_size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
		uint image_width = ilGetInteger(IL_IMAGE_WIDTH);
		uint image_height = ilGetInteger(IL_IMAGE_HEIGHT);
		uint type = ilGetInteger(IL_IMAGE_TYPE);

		if (data_size > 0)
		{

			ResourceTexture* rtex = (ResourceTexture*)App->resource_manager->CreateNewResource(RT_TEXTURE);



			App->resource_manager->ExportResourceToLibrary(rtex);


			resources.push_back(rtex);

			ret = true;
		}

		ilDeleteImages(1, &ImageInfo.Id);
	}

	return ret;
}

//bool ResourceTextureLoader::Load(const char * filepath, std::vector<Resource*>& resources)
//{
//	bool ret = false;
//
//	// Load texture
//	if (ilLoad(IL_TYPE_UNKNOWN, filepath))
//	{
//		// Get file name
//		string file_name = App->file_system->GetFileNameFromFilePath(filepath);
//
//		// Create texture
//		ResourceTexture* rtex = (ResourceTexture*)App->resource_manager->CreateNewResource(RT_TEXTURE);
//
//		// Get texture info
//		ILinfo ImageInfo;
//		iluGetImageInfo(&ImageInfo);
//
//		// Rotate if origin is upper left
//		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
//		{
//			iluFlipImage();
//			rtex->SetFlipped(true);
//		}
//
//		// Convert image to rgb and a byte chain
//		ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
//		
//		// Save data
//		rtex->SetData(ilGetData(), ilGetInteger(IL_IMAGE_SIZE_OF_DATA), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT),
//			GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
//
//		// Set name
//		rtex->SetFileName(file_name.c_str());
//
//		// Export it to Library
//		App->resource_manager->SaveResourceIntoFile(rtex);
//
//		ilDeleteImages(1, &ImageInfo.Id);
//
//		resources.push_back(rtex);
//
//		ret = true;
//	}
//	else
//	{
//		CONSOLE_LOG("Cannot load image %s. Error: %s", filepath, iluErrorString(ilGetError()));
//	}
//
//	return ret;
//}
//
//TextureInfo ResourceTextureLoader::LoadTexture(const char* filename, bool avoid_flip)
//{
//	ILuint textureID;
//	ILenum error;
//	ILboolean success;
//
//	// Texture Generation
//	glGenTextures(1, &textureID);
//	glBindTexture(GL_TEXTURE_2D, textureID);
//
//	success = ilLoadImage(filename);
//
//	ILinfo ImageInfo;
//	if (success)
//	{
//		iluGetImageInfo(&ImageInfo);
//
//		//Flip the image into the right way
//		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT && !avoid_flip)
//		{
//			iluFlipImage();
//		}
//
//		// Convert the image into a suitable format to work with
//		if (!ilConvertImage(ilGetInteger(IL_IMAGE_FORMAT), IL_UNSIGNED_BYTE))
//		{
//			error = ilGetError();
//			CONSOLE_LOG("Image conversion failed - IL reportes error: %i, %s", error, iluErrorString(error));
//			exit(-1);
//		}
//
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
//		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT),
//			ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 
//			0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
//
//		CONSOLE_LOG("Texture Application Successful.");
//	}
//
//	else
//	{
//		error = ilGetError();
//		CONSOLE_LOG("Image Load failed - IL reportes error: %i, %s", error, iluErrorString(error));
//	}
//
//	//RELEASE MEMORY used by the image
//	ilDeleteImages(1, &textureID);
//
//	return TextureInfo(textureID, ImageInfo.Width, ImageInfo.Height);
//}
//
//void ResourceTextureLoader::UnloadTexture(unsigned int id)
//{
//	if(id != 0)
//		App->renderer3D->UnloadTextureBuffer(id, 1);
//}
//
//void ResourceTextureLoader::ImportAllTextures()
//{
//	vector<string> files = App->file_system->GetFilesInPath(App->file_system->GetLibraryTexturePath().c_str(), "dds");
//
//	for (vector<string>::iterator it = files.begin(); it != files.end(); it++)
//	{
//		Import((*it).c_str());
//	}
//}
//
//void ResourceTextureLoader::Unload(const char * filepath)
//{
//	string path = App->file_system->GetPathFromFilePath(filepath);
//	string filename = App->file_system->GetFileNameFromFilePath(filepath);
//	string extension = App->file_system->GetFileExtension(filename.c_str());
//	string name = App->file_system->GetFilenameWithoutExtension(filename.c_str(), false);
//
//	string meta_path = path + filename + ".meta";
//
//	JSON_Doc* meta = App->json->LoadJSON(meta_path.c_str());
//
//	if (meta != nullptr)
//	{
//		int resources_count = meta->GetArrayCount("resources");
//
//		for (int i = 0; i < resources_count; i++)
//		{
//			string res_uid = meta->GetStringFromArray("resources", i);
//
//			string resource_path = App->file_system->GetLibraryTexturePath() + res_uid + ".dss";
//			string resource_meta_path = App->file_system->GetLibraryTexturePath() + res_uid + ".meta";
//
//			App->gameobj->DeleteGameObjectsUsingResource(App->resource_manager->Get(res_uid));
//
//			App->file_system->FileDelete(resource_path.c_str());
//			App->file_system->FileDelete(resource_meta_path.c_str());
//		}
//	}
//
//	App->file_system->FileDelete(meta_path.c_str());
//	App->file_system->FileDelete(filepath);
//}
//
//void ResourceTextureLoader::Import(const char * filepath)
//{
//	string path = App->file_system->GetPathFromFilePath(filepath);
//	string filename = App->file_system->GetFileNameFromFilePath(filepath);
//	string name = App->file_system->GetFilenameWithoutExtension(filename.c_str());
//
//	// -------------------------------------
//	// META --------------------------------
//	// -------------------------------------
//	string meta_name = path + name + ".meta";
//	JSON_Doc* doc = App->json->LoadJSON(meta_name.c_str());
//
//	if (doc != nullptr)
//	{
//		string uid = doc->GetString("uid", "no_uid");
//		string resource_name = doc->GetString("name");
//		bool flipped = doc->GetBool("flipped");
//
//		// -------------------------------------
//		// FILE --------------------------------
//		// -------------------------------------
//		// Load texture
//		if (ilLoad(IL_TYPE_UNKNOWN, filepath))
//		{
//			// Get file name
//			string file_name = App->file_system->GetFileNameFromFilePath(filepath);
//
//			// Get texture info
//			ILinfo ImageInfo;
//			iluGetImageInfo(&ImageInfo);
//
//			if (!flipped)
//			{
//				iluFlipImage();
//			}
//
//			// Convert image to rgb and a byte chain
//			ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
//
//			// Create texture
//			ResourceTexture* ret = (ResourceTexture*)App->resource_manager->CreateNewResource(RT_TEXTURE, uid.c_str());
//			ret->SetData(ilGetData(), ilGetInteger(IL_IMAGE_SIZE_OF_DATA), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT),
//				GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
//			ret->SetFlipped(flipped);
//			ret->SetFileName(resource_name.c_str());
//
//			ilDeleteImages(1, &ImageInfo.Id);
//		}
//		else
//		{
//			CONSOLE_LOG("Cannot load image %s. Error: %s", filepath, iluErrorString(ilGetError()));
//		}
//
//		App->json->UnloadJSON(doc);
//	}
//}
//
//bool ResourceTextureLoader::Export(const char * path, ResourceTexture* resource)
//{
//	bool ret = true;
//
//	string name = resource->GetUniqueId();
//
//	// -------------------------------------
//	// FILE --------------------------------
//	// -------------------------------------
//	uint size = 0;
//	byte* data = nullptr;
//
//	// To pick a specific DXT compression use
//	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
//
//	// Get the size of the data buffer
//	size = ilSaveL(IL_DDS, NULL, 0);
//
//	if (size > 0)
//	{
//		ilEnable(IL_FILE_OVERWRITE);
//
//		// Allocate data buffer
//		data = new byte[size];
//
//		// Save to buffer with the ilSaveIL function
//		if (ilSaveL(IL_DDS, data, size) > 0)
//			ret = App->file_system->FileSave(path, (char*)data, name.c_str(), "dds", size);
//
//		RELEASE_ARRAY(data);
//	}
//
//	// -------------------------------------
//	// META --------------------------------
//	// -------------------------------------
//	string meta_name = path + name + ".meta";
//
//	JSON_Doc* doc = App->json->LoadJSON(meta_name.c_str());
//	if (doc == nullptr)
//		doc = App->json->CreateJSON(meta_name.c_str());
//
//	if (doc != nullptr)
//	{
//		doc->Clear();
//
//		doc->SetString("uid", resource->GetUniqueId().c_str());
//		doc->SetString("name", resource->GetFileName().c_str());
//		doc->SetBool("flipped", resource->GetFlipped());
//
//		doc->Save();
//	}
//
//	App->json->UnloadJSON(doc);
//
//	return ret;
//}

