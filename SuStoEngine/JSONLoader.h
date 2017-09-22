#ifndef _JSONLOADER_H_
#define _JSONLOADER_H_

#include "Module.h"
#include "Parson\parson.h"

struct json_doc
{
	JSON_Value* value = nullptr;
	JSON_Object* object = nullptr;
	string path;
};

class JSONLoader : public Module
{
public:
	JSONLoader(bool enabled = true);
	~JSONLoader();

	bool Awake();
	JSON_Object* LoadJSON(const char* path);
	void SaveJSON(const char* path);
	JSON_Object* CreateJSON(const char* path);

	// Called before quitting
	bool CleanUp();

private:

public:

private:
	std::list<json_doc>	jsons;
};

#endif // !_JSONLOADER_H_
