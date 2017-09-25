#ifndef _JSONLOADER_H_
#define _JSONLOADER_H_

#include "Module.h"
#include "Parson\parson.h"

class JSON_Doc
{
public:
	JSON_Doc(JSON_Value* value, JSON_Object* object, const char* path);
	~JSON_Doc();

	void SetString(const char* set, const char* str);
	void SetBool(const char* set, bool bo);
	void SetNumber(const char* set, double nu);
	const char* GetString(const char* str);
	bool GetBool(const char* bo);
	double GetNumber(const char* nu);

	const char* GetPath();
	void Save();
	void CleanUp();

private:
	JSON_Value* value = nullptr;
	JSON_Object* object = nullptr;
	const char* path;
};

class JSONLoader : public Module
{
public:
	JSONLoader(bool enabled = true);
	~JSONLoader();

	bool Awake();
	bool CleanUp();

	JSON_Doc* LoadJSON(const char* path);
	JSON_Doc* CreateJSON(const char* path);

private:
	std::list<JSON_Doc*> jsons;
};

#endif // !_JSONLOADER_H_
