#ifndef __RESOURCE_TEXTURE_LOADER_H__
#define __RESOURCE_TEXTURE_LOADER_H__

class ResourceTexture;

class ResourceTextureLoader
{
public:
	ResourceTextureLoader();
	virtual ~ResourceTextureLoader();

	ResourceTexture* Load(const char* filepath);
	void Import(const char* filepath);
	bool Export(const char* filepath, ResourceTexture* resource);
	void ImportAllTextures();

private:
	

};

#endif