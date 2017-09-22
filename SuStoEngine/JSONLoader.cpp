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
