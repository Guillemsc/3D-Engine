#include "JSONLoader.h"
#include "Globals.h"

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

	//JSON_Object* ob = LoadJSON("package.json");

	//const char *name = NULL;

	//name = json_object_get_string(ob, "name");

	//LOG_OUTPUT("%s", name);

	//json_object_set_string(ob, "name", "John Smith");

	//SaveJSON("package.json");

	//CreateJSON("test.json");

	return ret;
}

JSON_Doc* JSONLoader::LoadJSON(const char * path)
{
	JSON_Doc* ret = nullptr;

	bool exists = false;
	for (list<JSON_Doc*>::iterator it = jsons.begin(); it != jsons.end(); it++)
	{
		if (path == (*it)->GetPath())
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

		if (user_data == nullptr)
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
		if (path == (*it)->GetPath())
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
		JSON_Object* root_object = json_value_get_object(root_value);

		JSON_Doc* new_doc = new JSON_Doc(root_value, root_object, path);
		jsons.push_back(new_doc);
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
	path = _path;
}

JSON_Doc::~JSON_Doc()
{
}

void JSON_Doc::SetString(const char * set, const char * str)
{
	json_object_dotset_string(object, set, str);
}

void JSON_Doc::SetBool(const char * set, bool bo)
{
	json_object_dotset_boolean(object, set, bo);
}

void JSON_Doc::SetNumber(const char * set, double nu)
{
	json_object_dotset_number(object, set, nu);
}

const char * JSON_Doc::GetString(const char * str)
{
	return json_object_dotget_string(object, str);
}

bool JSON_Doc::GetBool(const char * bo)
{
	return json_object_dotget_boolean(object, bo);
}

double JSON_Doc::GetNumber(const char * nu)
{
	return json_object_dotget_number(object, nu);
}

const char * JSON_Doc::GetPath()
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
