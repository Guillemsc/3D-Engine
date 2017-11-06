#ifndef __TEXTURELOADER_H__
#define __TEXTURELOADER_H__

#include "Module.h"
#include "GeometryMath.h"

class TextureImporter;

class Texture
{
public:
	Texture(byte* texture_data, uint texture_data_lenght, uint width, uint height, int format, const char* filename, uint wrap_s, uint wrap_t, uint mag, uint min);
	
	bool operator == (Texture* text);

	void CleanUp();

	uint GetId();

	string GetFileName();
	float2 GetSize();
 
	void AddUser();
	void DeleteUser();
	int	 UsedBy();
	bool IsUsed();

	double GetUniqueId();
	void SetUniqueId(double set);

	void LoadToMemory();
	void UnloadFromMemory();

private:
	byte*  texture_data = nullptr;
	int	   format = 0;
	float2 size = float2(0, 0);
	uint   wrap_s = 0;
	uint   wrap_t = 0;
	uint   mag = 0;
	uint   min = 0;

	string file_name;
	uint   id = 0;

	int  used_by = 0;

	double unique_id = 0;
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

	void UnloadAllFiles();

private:
	vector<Texture*> textures;

	TextureImporter* texture_importer = nullptr;

public:
	
};

#include "Importer.h"

class TextureImporter : public Importer
{
public:
	bool Load(const char * exported_file);
	bool Save(const char* path, Texture* textures);
	void ImportAllTextures();
};

#endif // __TEXTURELOADER_H__