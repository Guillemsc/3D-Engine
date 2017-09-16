#ifndef __j1FILESYSTEM_H__
#define __j1FILESYSTEM_H__

#include "Module.h"

struct SDL_RWops;

int close_sdl_rwops(SDL_RWops *rw);

class FileSystem : public Module
{
public:

	FileSystem(bool enabled = true);

	// Destructor
	virtual ~FileSystem();

	// Called before render is available
	bool Awake();

	// Called before quitting
	bool CleanUp();

	// Utility functions
	bool AddPath(const char* path_or_zip, const char* mount_point = NULL);
	bool Exists(const char* file) const;
	bool IsDirectory(const char* file) const;
	bool ChangeWriteDir(const char* file) const;
	const char* GetSaveDirectory() const
	{
		return "save/";
	}

	// Open for Read/Write
	unsigned int Load(const char* file, char** buffer) const;
	SDL_RWops* Load(const char* file) const;

	unsigned int Save(const char* file, const char* buffer, unsigned int size) const;

public:
	char* base_path = NULL;
	char* write_path = NULL;

private:

};

#endif // __j1FILESYSTEM_H__