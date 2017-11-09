#ifndef __TEXTURELOADER_H__
#define __TEXTURELOADER_H__

#include "Module.h"
#include "GeometryMath.h"

class TextureImporter;
class ResourceTexture;


class TextureLoader : public Module
{
public:
	TextureLoader(bool start_enabled = true);
	~TextureLoader();

	bool Awake();
	bool Start();
	bool Update();
	bool CleanUp();
	void OnLoadFile(const char* file_path, const char* file_name, const char* file_extension);

	ResourceTexture* LoadTexture(const char* full_path);

	TextureImporter* GetTextureImporter();

private:
	TextureImporter* texture_importer = nullptr;

public:
	
};

class TextureImporter
{
public:
	ResourceTexture* Load(const char * exported_file);
	bool Save(const char* path, ResourceTexture* textures);
};

#endif // __TEXTURELOADER_H__