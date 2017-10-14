#include "ModuleFileSystem.h"

FileSystem::FileSystem(bool start_enabled)
{
	SetName("File System");
}

FileSystem::~FileSystem()
{
}

bool FileSystem::Awake()
{
	return true;
}

bool FileSystem::Start()
{
	return true;
}

bool FileSystem::Update()
{
	return true;
}

bool FileSystem::CleanUp()
{
	return true;
}


bool FileSystem::CopyToExplorer(const char * full_path, const char * destination)
{
	return true;
}
