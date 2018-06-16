#ifndef __RESOURCE_SHADER_LOADER_H__
#define __RESOURCE_SHADER_LOADER_H__

#include "ResourceLoader.h"
#include "Resource.h"
#include "ResourceShader.h"
#include <vector>

class ResourceShader;

class ResourceShaderLoader : public ResourceLoader
{
public:
	ResourceShaderLoader();
	virtual ~ResourceShaderLoader();

	void Start();

	Resource* CreateResource(std::string new_uid);

	bool LoadFileToEngine(DecomposedFilePath decomposed_file_path, std::vector<Resource*>& resources);

	bool RemoveAssetInfoFromEngine(DecomposedFilePath decomposed_file_path);
	void ClearFromGameObject(Resource* resource, GameObject* go);

	bool ExportResourceToLibrary(Resource* resource);
	bool ExportResourceToAssets(Resource* resource);
	bool ImportResourceFromLibrary(DecomposedFilePath decomposed_file_path);

	bool LoadAssetIntoScene(DecomposedFilePath decomposed_file_path);

	bool IsAssetOnLibrary(DecomposedFilePath d_filepath, std::vector<std::string>& library_files_used);
	bool RenameAsset(DecomposedFilePath decomposed_file_path, const char* new_name);

	void CreateDefaultShaders();
	ResourceShader* CreateShader(const char* vertex_code, const char* fragment_code);

	ResourceShader* GetDefaultShader() const;

private:

private:
	ResourceShader* default_shader = nullptr;

};

#endif // !__RESOURCE_SHADER_LOADER_H__