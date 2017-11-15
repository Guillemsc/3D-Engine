#ifndef __RESOURCE_TEXTURE_H__
#define __RESOURCE_TEXTURE_H__

#include "Resource.h"
#include <string>
#include "Globals.h"
#include "GeometryMath.h"

class ResourceTexture : public Resource
{
public:
	ResourceTexture(std::string unique_id);
	virtual ~ResourceTexture();

	void CleanUp();

	void SetData(byte* texture_data, uint texture_data_lenght, uint width, uint height, int format, uint wrap_s, uint wrap_t, uint mag, uint min);
	void SetFlipped(bool set);

	uint GetTextureId();
	float2 GetSize();
	byte* GetTextureData();
	uint GetTextureDataSize();
	bool GetFlipped();

private:
	void LoadToMemory();
	void UnloadFromMemory();

private:
	byte*  texture_data = nullptr;
	uint   texture_data_size = 0;
	uint   texture_id = 0;
	int	   format = 0;
	float2 size = float2(0, 0);
	uint   wrap_s = 0;
	uint   wrap_t = 0;
	uint   mag = 0;
	uint   min = 0;
	bool   flipped = false;
};

#endif