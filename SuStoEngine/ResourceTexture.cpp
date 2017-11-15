#include "ResourceTexture.h"
#include "App.h"
#include "ModuleRenderer3D.h"

ResourceTexture::ResourceTexture(std::string unique_id) : Resource(unique_id, RT_TEXTURE)
{
}

ResourceTexture::~ResourceTexture()
{
}

void ResourceTexture::CleanUp()
{
	UnloadFromMemory();

	if (texture_data != nullptr)
		RELEASE_ARRAY(texture_data);
}

void ResourceTexture::SetData(byte * _texture_data, uint _texture_data_lenght, uint _width, uint _height, int _format, uint _wrap_s, uint _wrap_t, uint _mag, uint _min)
{
	if (_texture_data_lenght > 0)
	{
		texture_data = new byte[_texture_data_lenght];
		memcpy(texture_data, _texture_data, _texture_data_lenght * sizeof(byte));
	}

	texture_data_size = _texture_data_lenght;
	format = _format;
	size.x = _width;
	size.y = _height;
	wrap_s = _wrap_s;
	wrap_t = _wrap_t;
	mag = _mag;
	min = _min;
}

void ResourceTexture::SetFlipped(bool set)
{
	flipped = set;
}

uint ResourceTexture::GetTextureId()
{
	return texture_id;
}

float2 ResourceTexture::GetSize()
{
	return size;
}

byte * ResourceTexture::GetTextureData()
{
	return texture_data;
}

uint ResourceTexture::GetTextureDataSize()
{
	return texture_data_size;
}

bool ResourceTexture::GetFlipped()
{
	return flipped;
}

void ResourceTexture::LoadToMemory()
{
	if (texture_id == 0 && texture_data != nullptr && size.x > 0 && size.y > 0)
		texture_id = App->renderer3D->LoadTextureBuffer(texture_data, 1, format, size.x, size.y, wrap_s, wrap_t, mag, min);
}

void ResourceTexture::UnloadFromMemory()
{
	if (texture_id != 0)
	{
		App->renderer3D->UnloadTextureBuffer(texture_id, 1);
		texture_id = 0;
	}
}
