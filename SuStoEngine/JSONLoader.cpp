#include "JSONLoader.h"
#include "Globals.h"
#include "Functions.h"

JSONLoader::JSONLoader(bool enabled) : Module(enabled)
{
	SetName("JSONLoader");
}

JSONLoader::~JSONLoader()
{
}

bool JSONLoader::Awake()
{
	bool ret = true;

	LOG_OUTPUT("Loading JSON Module");

	return ret;
}

JSON_Doc* JSONLoader::LoadJSON(const char * path)
{
	JSON_Doc* ret = nullptr;

	bool exists = false;
	for (list<JSON_Doc*>::iterator it = jsons.begin(); it != jsons.end(); it++)
	{
		if (TextCmp(path, (*it)->GetPath()))
		{
			ret = (*it);
			exists = true;
			break;
		}
	}

	if (!exists)
	{
		JSON_Value *user_data = json_parse_file(path);
		JSON_Object *root_object = json_value_get_object(user_data);

		if (user_data == nullptr && root_object == nullptr)
		{
			LOG_OUTPUT("Error loading %s", path);
		}
		else
		{
			LOG_OUTPUT("Succes loading %s", path);

			JSON_Doc* new_doc = new JSON_Doc(user_data, root_object, path);
			jsons.push_back(new_doc);

			ret = new_doc;
		}
	}

	return ret;
}


JSON_Doc* JSONLoader::CreateJSON(const char * path)
{
	JSON_Doc* ret = nullptr;

	bool exists = false;
	for (list<JSON_Doc*>::iterator it = jsons.begin(); it != jsons.end(); it++)
	{
		if (TextCmp(path, (*it)->GetPath()))
		{
			exists = true;
			break;
		}
	}

	if (exists)
	{
		LOG_OUTPUT("Error creating %s. There is already a file with this path/name", path);
	}
	else
	{
		JSON_Value* root_value = json_value_init_object();

		if (root_value == nullptr)
		{
			LOG_OUTPUT("Error creating %s. Wrong path?", path);
		}
		else
		{
			JSON_Object* root_object = json_value_get_object(root_value);

			JSON_Doc* new_doc = new JSON_Doc(root_value, root_object, path);
			jsons.push_back(new_doc);

			new_doc->Save();

			ret = new_doc;
		}
	}

	return ret;
}

bool JSONLoader::CleanUp()
{
	bool ret = true;

	LOG_OUTPUT("Unloading JSON Module");

	for (list<JSON_Doc*>::iterator it = jsons.begin(); it != jsons.end();)
	{
		(*it)->CleanUp();
		delete (*it);

		it = jsons.erase(it);
	}

	return ret;
}

JSON_Doc::JSON_Doc(JSON_Value * _value, JSON_Object * _object, const char* _path)
{
	value = _value;
	object = _object;
	root = _object;
	path = _path;
}

JSON_Doc::~JSON_Doc()
{
}

void JSON_Doc::SetString(string set, const char * str)
{
	json_object_dotset_string(object, set.c_str(), str);
}

void JSON_Doc::SetBool(string set, bool bo)
{
	json_object_dotset_boolean(object, set.c_str(), bo);
}

void JSON_Doc::SetNumber(string set, double nu)
{
	json_object_dotset_number(object, set.c_str(), nu);
}

void JSON_Doc::SetArray(string set)
{
	JSON_Value* va = json_value_init_array();
	JSON_Array* arr = json_value_get_array(va);

	json_object_dotset_value(object, set.c_str(), va);
}

void JSON_Doc::ClearArray(string arr)
{
	JSON_Array* array = json_object_get_array(root, arr.c_str());

	if (array != nullptr)
	{
		json_array_clear(array);
	}
}

const int JSON_Doc::GetArrayCount(string set) const
{
	int ret = 0;

	JSON_Array* array = json_object_get_array(root, set.c_str());

	if (array != nullptr)
	{
		ret = json_array_get_count(array);
	}

	return ret;
}

const char * JSON_Doc::GetStringFromArray(string arr, int index)
{
	const char* ret = nullptr;

	JSON_Array* array = json_object_get_array(root, arr.c_str());

	if (array != nullptr)
	{
		if (FindArrayValue(arr.c_str(), index, json_value_type::JSONString))
		{
			ret = json_array_get_string(array, index);
		}
	}

	return ret;
}

const bool JSON_Doc::GetBoolFromArray(string arr, int index)
{
	bool ret = false;

	JSON_Array* array = json_object_get_array(root, arr.c_str());

	if (array != nullptr)
	{
		if (FindArrayValue(arr.c_str(), index, json_value_type::JSONBoolean))
		{
			ret = json_array_get_boolean(array, index);
		}
	}

	return ret;
}

const double JSON_Doc::GetNumberFromArray(string arr, int index)
{
	double ret = 0;

	JSON_Array* array = json_object_get_array(root, arr.c_str());

	if (array != nullptr)
	{
		if (FindArrayValue(arr.c_str(), index, json_value_type::JSONNumber))
		{
			ret = json_array_get_number(array, index);
		}
	}

	return ret;
}

void JSON_Doc::AddStringToArray(string arr, const char * str)
{
	JSON_Array* array = json_object_get_array(root, arr.c_str());

	if (array != nullptr)
	{
		json_array_append_string(array, str);
	}
}

void JSON_Doc::AddBoolToArray(string arr, bool bo)
{
	JSON_Array* array = json_object_get_array(root, arr.c_str());

	if (array != nullptr)
	{
		json_array_append_boolean(array, bo);
	}
}

void JSON_Doc::AddNumberToArray(string arr, double set)
{
	JSON_Array* array = json_object_get_array(root, arr.c_str());

	if (array != nullptr)
	{
		json_array_append_number(array, set);
	}
}

void JSON_Doc::AddSectionToArray(string arr)
{
	JSON_Array* array = json_object_get_array(root, arr.c_str());

	if (array != nullptr)
	{
		json_array_append_value(array, json_value_init_object());
	}
}

bool JSON_Doc::MoveToSectionFromArray(string arr, int index)
{
	bool ret = false;

	JSON_Array* array = json_object_get_array(root, arr.c_str());

	if (array != nullptr)
	{
		JSON_Object* obj = json_array_get_object(array, index);

		object = obj;
		ret = true;
	}

	return ret;
}

const char * JSON_Doc::GetString(string str, const char* defaul)
{
	const char* ret = defaul;

	if(FindValue(str.c_str(), json_value_type::JSONString))
		ret = json_object_dotget_string(object, str.c_str());

	return ret;
}

const bool JSON_Doc::GetBool(string str, bool defaul)
{
	bool ret = defaul;

	if (FindValue(str.c_str(), json_value_type::JSONBoolean))
		ret = json_object_dotget_boolean(object, str.c_str());

	return ret;
}

const double JSON_Doc::GetNumber(string str, double defaul)
{
	double ret = defaul;

	if (FindValue(str.c_str(), json_value_type::JSONNumber))
		ret = json_object_dotget_number(object, str.c_str());

	return ret;
}

bool JSON_Doc::MoveToSection(string set)
{
	bool ret = false;

	JSON_Object* obj = json_object_get_object(object, set.c_str());

	if (obj != nullptr)
	{
		object = obj;
		ret = true;
	}

	return ret;
}

void JSON_Doc::RemoveSection(string set)
{
	json_object_remove(object, set.c_str());
}

void JSON_Doc::MoveToRoot()
{
	object = root;
}

void JSON_Doc::AddSection(string set)
{
	json_object_set_value(object, set.c_str(), json_value_init_object());
}

const char * JSON_Doc::GetPath() const
{
	return path;
}

void JSON_Doc::Save()
{
	json_serialize_to_file_pretty(value, path);
}

void JSON_Doc::CleanUp()
{
	json_value_free(value);
}

bool JSON_Doc::FindValue(const char * str, json_value_type type)
{
	bool ret = false;

	JSON_Value* val = json_object_dotget_value(object, str);

	if (val != nullptr && json_value_get_type(val) == type)
		ret = true;

	return ret;
}

bool JSON_Doc::FindArrayValue(const char * arr, int index, json_value_type type)
{
	bool ret = false;

	JSON_Array* array = json_object_get_array(root, arr);

	if (array != nullptr)
	{
		JSON_Value* val = json_array_get_value(array, index);

		if (val != nullptr && json_value_get_type(val) == type)
			ret = true;
	}
	
	return ret;
}
