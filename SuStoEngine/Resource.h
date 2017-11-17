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

	void SetFilePath(std::string name);
	std::string GetFilePath();
	std::string GetName();

	ResourceType GetType();

	std::string GetUniqueId();
	bool IsUsed();
	int  UsedCount();

	void LoadMem();
	void UnloadMem();

	void SetOriginalFileUID(std::string set);
	std::string GetOriginalFileUID();

private:
	virtual void LoadToMemory() {};
	virtual void UnloadFromMemory() {};

private:
	ResourceType type = RT_NULL;
	std::string  unique_id;
	int			 count_ref = 0;

	std::string  filepath;
	std::string	 name;

	std::string original_file_unique_id;

};

#endif