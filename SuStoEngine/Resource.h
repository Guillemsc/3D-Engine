#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <string>

enum ResourceType
{
	RT_NULL,
	RT_TEXTURE,
	RT_MESH,
	RT_SCENE,
};

class Resource
{
public:
	Resource(std::string unique_id, ResourceType type);
	virtual ~Resource();

	virtual void CleanUp() {};

	void SetFileName(const char* file_name);
	std::string GetFileName();

	void SetFilePath(const char* file_path);
	std::string GetFilePath();

	ResourceType GetType();

	std::string GetUniqueId();

	bool IsUsed();
	int  UsedCount();

	void LoadMem();
	void UnloadMem();

private:
	virtual void LoadToMemory() {};
	virtual void UnloadFromMemory() {};

private:
	ResourceType type = RT_NULL;
	std::string  unique_id;
	int			 count_ref = 0;

	std::string file_name;
	std::string file_path;
};

#endif