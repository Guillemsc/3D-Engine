#ifndef __RESOURCE_TEXTURE_LOADER_H__
#define __RESOURCE_TEXTURE_LOADER_H__

#include "ResourceLoader.h"
#include "Resource.h"
#include <vector>

class ResourceTexture;

struct TextureInfo
{
	TextureInfo() {};
	TextureInfo(const TextureInfo& copy) { id = copy.id; size_x = copy.size_x; size_y = copy.size_y;
	};
	TextureInfo(unsigned int _id, unsigned int _size_x, unsigned int _size_y) { id = _id; size_x = _size_x; size_y = _size_y; };

	unsigned int id = 0;
	unsigned int size_x = 0;
	unsigned int size_y = 0;
};

class ResourceTextureLoader : public ResourceLoader
{
public:
	ResourceTextureLoader();
	virtual ~ResourceTextureLoader();

	// NEW
	Resource* CreateResource(std::string new_uid);

	bool LoadToEngine(const char* filepath, std::vector<Resource*>& resources);
	bool UnloadFromEngine(Resource* resource);

	void ClearFromGameObject(Resource* resource, GameObject* go);

	bool ExportToLibrary(Resource* resource);
	bool ImportFromLibrary(const char* uid);

	bool LoadIntoScene(Resource* resource);

	bool IsResourceOnLibrary(Resource* resource);
	bool IsResourceOnAssets(Resource* resource);

	void CreateResourcesMissingOnAssets();
	void RemoveResourcesMissingOnLibrary();
	// ---

	//bool Load(const char* filepath, std::vector<Resource*>& resources);
	//void Import(const char* filepath);
	//bool Export(const char* filepath, ResourceTexture* resource);
	//void ImportAllTextures();

	//void Unload(const char* filepath);
	
	TextureInfo LoadTexture(const char * filename, bool avoid_flip = false);
	void UnloadTexture(unsigned int id);


private:
	

};

#endif