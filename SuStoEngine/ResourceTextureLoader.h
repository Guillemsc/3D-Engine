#ifndef __RESOURCE_TEXTURE_LOADER_H__
#define __RESOURCE_TEXTURE_LOADER_H__

#include "Resource.h"
#include <vector>

class ResourceTexture;

class ResourceTextureLoader
{
public:
	ResourceTextureLoader();
	virtual ~ResourceTextureLoader();

	bool Load(const char* filepath, std::vector<Resource*>& resources);
	void Import(const char* filepath);
	bool Export(const char* filepath, ResourceTexture* resource);
	void ImportAllTextures();

	void DeImport(const char* filepath);

	unsigned int LoadTexture(const char * filename);
	void UnloadTexture(unsigned int id);


private:
	

};

#endif