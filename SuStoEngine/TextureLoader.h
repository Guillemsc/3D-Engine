#ifndef __TEXTURELOADER_H__
#define __TEXTURELOADER_H__

#include "Module.h"
#include "GeometryMath.h"

class Texture
{
public:
	Texture(uint id, uint width, uint height);
	
	bool operator == (Texture* text);

	void CleanUp();

	uint GetId();
	float2 GetSize();

	void AddUser();
	void DeleteUser();
	int	 UsedBy();
	bool IsUsed();

private:
	uint   id = 0;
	float2 size = float2(0, 0);

	int  used_by = 0;
};

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

	bool LoadTexture(const char* full_path);
	void UnloadTexture(Texture* text);

private:
	vector<Texture*> textures;

public:

};

#endif // __TEXTURELOADER_H__