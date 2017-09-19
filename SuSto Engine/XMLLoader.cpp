#include "XMLLoader.h"
#include "App.h"
#include "FileSystem.h"
#include "PugiXml\src\pugixml.hpp"

XMLLoader::XMLLoader(bool start_enabled) : Module(start_enabled)
{

}

XMLLoader::~XMLLoader()
{
}

bool XMLLoader::Awake()
{
	bool ret = true;

	LOG_OUTPUT("Loading XML Module");
	SetName("XmlLoader");

	return ret;
}

void XMLLoader::LoadXML(const char * path, pugi::xml_document & doc)
{
	bool exist = false;

	pugi::xml_document daoc;

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
		char* buf = NULL;
		int size = App->fs->Load(path, &buf);
		pugi::xml_parse_result result = doc.load_buffer(buf, size);

		if (buf == NULL)
		{
			LOG_OUTPUT("Error loading '%s', probably wrong XML file name", path);
		}
		else
		{
			LOG_OUTPUT("Succes loading '%s'", path);
			xml_doc* new_doc = new xml_doc();
			new_doc->path = path;
			new_doc->doc.reset(doc);

			xmls.push_back(new_doc);
		}

		RELEASE(buf);
	}
}

bool XMLLoader::CleanUp()
{
	bool ret = true;

	LOG_OUTPUT("Unloading XML Module")

	for (std::list<xml_doc*>::iterator it = xmls.begin(); it != xmls.end();)
	{
		RELEASE(*it);
		it = xmls.erase(it);
	}
	
	return ret;
}
