#include "XMLLoader.h"
#include "App.h"
#include "PugiXml\src\pugixml.hpp"

XMLLoader::XMLLoader(bool start_enabled) : Module(start_enabled)
{
	SetName("XMLLoader");
}

XMLLoader::~XMLLoader()
{
}

bool XMLLoader::Awake()
{
	bool ret = true;

	CONSOLE_LOG("Loading XML Module");

	return ret;
}

void XMLLoader::LoadXML(const char * path, pugi::xml_document & doc)
{
	bool exist = false;

	for (std::list<xml_doc*>::iterator it = xmls.begin(); it != xmls.end(); ++it)
	{
		if (path == (*it)->path)
		{
			doc.reset((*it)->doc);
			exist = true;
			break;
		}
	}

	if(!exist)
	{
		pugi::xml_parse_result result = doc.load_file(path);

		if (!result)
		{
			CONSOLE_LOG("Error loading '%s', probably wrong XML file name", path);
		}
		else
		{
			CONSOLE_LOG("Succes loading '%s'", path);
			xml_doc* new_doc = new xml_doc();
			new_doc->path = path;
			new_doc->doc.reset(doc);

			xmls.push_back(new_doc);
		}
	}
}

bool XMLLoader::CleanUp()
{
	bool ret = true;

	CONSOLE_LOG("Unloading XML Module")

	for (std::list<xml_doc*>::iterator it = xmls.begin(); it != xmls.end();)
	{
		RELEASE(*it);
		it = xmls.erase(it);
	}
	
	return ret;
}
