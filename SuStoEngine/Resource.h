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
	Resource(double unique_id);
	virtual ~Resource();

	virtual void CleanUp() {};

	void SetName(std::string name);
	std::string GetName();

	ResourceType GetType();

	double GetUniqueId();
	bool IsUsed();
	int  UsedCount();

	void LoadMem();
	void UnloadMem();

private:
	virtual void LoadToMemory() {};
	virtual void UnloadFromMemory() {};

private:
	ResourceType type = RT_NULL;
	double		 unique_id = 0;
	int			 count_ref = 0;
	std::string	 name;
};

#endif