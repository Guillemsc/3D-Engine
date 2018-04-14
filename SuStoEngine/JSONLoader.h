#ifndef _JSONLOADER_H_
#define _JSONLOADER_H_

#include "Module.h"
#include "Parson\parson.h"

#include "GeometryMath.h"

class JSON_Doc
{
public:
	JSON_Doc();
	JSON_Doc(JSON_Value* value, JSON_Object* object, const char* path);
	JSON_Doc(JSON_Doc& doc);
	~JSON_Doc();

	void SetString(const string& set, const char* str);
	void SetBool(const string& set, bool bo);
	void SetNumber(const string& set, double nu);
	void SetNumber3(const string& set, float3 val);
	void SetNumber4(const string& set, float4 val);
	const char* GetString(const string& str, const char* defaul = "");
	const bool GetBool(const string& bo, bool defaul = false);
	const double GetNumber(const string& nu, double defaul = 0);
	const float3 GetNumber3(const string& fl, float3 defaul = float3(0, 0, 0));
	const float4 GetNumber4(const string& fl, float4 defaul = float4(0, 0, 0, 0));

	void SetArray(const string& set);
	void ClearArray(const string& arr);
	void AddStringToArray(const string& arr, const char* str);
	void AddBoolToArray(const string& arr, bool bo);
	void AddNumberToArray(const string& arr, double set);
	void AddSectionToArray(const string& arr);
	bool MoveToSectionFromArray(const string& arr, int index);
	const int GetArrayCount(const string& set) const;
	const char* GetStringFromArray(const string& arr, int index);
	const bool GetBoolFromArray(const string& arr, int index);
	const double GetNumberFromArray(const string& arr, int index);

	bool MoveToSection(const string& set);
	void RemoveSection(const string& set);
	void MoveToRoot();
	void AddSection(const string& set);

	JSON_Doc GetJsonNode();

	void Clear();

	string GetPath();
	void Save();
	void CleanUp();

private:
	bool FindValue(const char* str, json_value_type type);
	bool FindArrayValue(const char* array, int index, json_value_type type);

private:
	JSON_Value*  value = nullptr;
	JSON_Object* object = nullptr;
	JSON_Object* root = nullptr;
	string		 path;
};

class JSONLoader : public Module
{
public:
	JSONLoader(bool enabled = true);
	~JSONLoader();

	bool Awake();
	bool CleanUp();

	JSON_Doc* LoadJSON(const char* path);
	JSON_Doc* CreateJSON(const char * path, const char* name, const char* extension);
	void UnloadJSON(JSON_Doc* path);

private:
	std::list<JSON_Doc*> jsons;
};

#endif // !_JSONLOADER_H_
