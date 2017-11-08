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
	const char* GetString(string str, const char* defaul = "");
	const bool GetBool(string bo, bool defaul = false);
	const double GetNumber(string nu, double defaul = 0);

	void SetArray(string set);
	void ClearArray(string arr);
	void AddStringToArray(string arr, const char* str);
	void AddBoolToArray(string arr, bool bo);
	void AddNumberToArray(string arr, double set);
	void AddSectionToArray(string arr);
	bool MoveToSectionFromArray(string arr, int index);
	const int GetArrayCount(string set) const;
	const char* GetStringFromArray(string arr, int index);
	const bool GetBoolFromArray(string arr, int index);
	const double GetNumberFromArray(string arr, int index);

	bool MoveToSection(string set);
	void RemoveSection(string set);
	void MoveToRoot();
	void AddSection(string set);

	void Clear();

	const char* GetPath() const;
	void Save();
	void CleanUp();

private:
	bool FindValue(const char* str, json_value_type type);
	bool FindArrayValue(const char* array, int index, json_value_type type);

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
