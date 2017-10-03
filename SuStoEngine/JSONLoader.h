#ifndef _JSONLOADER_H_
#define _JSONLOADER_H_

#include "Module.h"
#include "Parson\parson.h"

class JSON_Doc
{
public:
	JSON_Doc(JSON_Value* value, JSON_Object* object, const char* path);
	~JSON_Doc();

	void SetString(string set, const char* str);
	void SetBool(string set, bool bo);
	void SetNumber(string set, double nu);
	void SetArray(const char* set);
	const char* GetString(const char* str, const char* defaul = "");
	bool GetBool(const char* bo, bool defaul = false);
	double GetNumber(const char* nu, double defaul = 0);

	bool MoveToSection(string set);
	void MoveToRoot();
	void AddSection(string set);

	const char* GetPath();
	void Save();
	void CleanUp();

private:
	bool FindValue(const char* str, json_value_type type);

private:
	JSON_Value*  value = nullptr;
	JSON_Object* object = nullptr;
	JSON_Object* root = nullptr;
	const char*  path;
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
