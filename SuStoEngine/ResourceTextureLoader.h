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

	Resource* CreateResource(std::string new_uid);

	bool LoadFileToEngine(DecomposedFilePath decomposed_file_path, std::vector<Resource*>& resources);

	bool RemoveAssetInfoFromEngine(DecomposedFilePath decomposed_file_path);
	void ClearFromGameObject(Resource* resource, GameObject* go);

	bool ExportResourceToLibrary(Resource* resource);
	bool ImportResourceFromLibrary(DecomposedFilePath decomposed_file_path);

	bool LoadAssetIntoScene(DecomposedFilePath decomposed_file_path);

	bool IsAssetOnLibrary(DecomposedFilePath d_filepath, std::vector<std::string>& library_files_used);
	bool RenameAsset(DecomposedFilePath decomposed_file_path, const char* new_name);


private:
	

};

#endif