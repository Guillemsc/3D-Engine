#include "JSONLoader.h"
#include "Globals.h"

JSONLoader::JSONLoader(bool enabled) : Module(enabled)
{
}

JSONLoader::~JSONLoader()
{
}

bool JSONLoader::Awake()
{
	bool ret = true;

	LOG_OUTPUT("Loading JSON Module");

	JSON_Object* ob = LoadJSON("package.json");

	const char *name = NULL;

	name = json_object_get_string(ob, "name");

	LOG_OUTPUT("%s", name);

	json_object_set_string(ob, "name", "John Smith");

	SaveJSON("package.json");

	CreateJSON("test.json");

	return ret;
}

JSON_Object* JSONLoader::LoadJSON(const char * path)
{
	JSON_Object* ret = nullptr;

	bool exists = false;
	for (list<json_doc>::iterator it = jsons.begin(); it != jsons.end(); it++)
	{
		if (path == (*it).path)
		{
			ret = (*it).object;
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
			ret = root_object;

			json_doc new_doc;

			new_doc.object = root_object;
			new_doc.value = user_data;
			new_doc.path = path;
			jsons.push_back(new_doc);
		}
	}

	return ret;
}

void JSONLoader::SaveJSON(const char * path)
{
	bool exists = false;
	for (list<json_doc>::iterator it = jsons.begin(); it != jsons.end(); it++)
	{
		if (path == (*it).path)
		{
			json_serialize_to_file((*it).value, path);
			exists = true;
			break;
		}
	}

	if (!exists)
	{
		LOG_OUTPUT("Error saving %s, probably wrong file path", path);
	}
}

JSON_Object* JSONLoader::CreateJSON(const char * path)
{
	JSON_Object* ret = nullptr;

	bool exists = false;
	for (list<json_doc>::iterator it = jsons.begin(); it != jsons.end(); it++)
	{
		if (path == (*it).path)
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

		json_doc new_doc;

		new_doc.object = root_object;
		new_doc.value = root_value;
		new_doc.path = path;
		jsons.push_back(new_doc);

		json_serialize_to_file(root_value, path);

		ret = root_object;
	}

	return ret;
}

bool JSONLoader::CleanUp()
{
	bool ret = true;

	LOG_OUTPUT("Unloading JSON Module");

	for (list<json_doc>::iterator it = jsons.begin(); it != jsons.end();)
	{
		json_value_free((*it).value);

		it = jsons.erase(it);
	}

	return ret;
}
