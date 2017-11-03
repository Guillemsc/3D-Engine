#ifndef __TEXTURELOADER_H__
#define __TEXTURELOADER_H__

#include "Module.h"
#include "GeometryMath.h"

class Texture
{
public:
	Texture(uint id, uint width, uint height, const char* filename);
	
	bool operator == (Texture* text);

	void CleanUp();

	uint GetId();
	string GetFileName();
	float2 GetSize();

	void AddUser();
	void DeleteUser();
	int	 UsedBy();
	bool IsUsed();

private:
	string file_name;
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

	Texture* LoadTexture(const char* full_path);
	void UnloadTexture(Texture* text);

private:
	vector<Texture*> textures;

public:
	
};

#include "Importer.h"

class TextureImporter : public Importer
{
public:
	bool Import(const char* file, const char* path, std::string& output_file);
	bool Import(const void* buffer, uint size, std::string& output_file);
	bool Load(const char * exported_file, Texture* texture);
	bool Save(const char* path, vector<Texture*> textures);

};

#endif // __TEXTURELOADER_H__