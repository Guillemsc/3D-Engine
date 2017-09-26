#ifndef _XMLLOADER_H_
#define _XMLLOADER_H_

#include "Module.h"
#include "PugiXml\src\pugixml.hpp"

struct xml_doc
{
	pugi::xml_document	doc;
	string				path;
};

class XMLLoader : public Module
{
public:
	XMLLoader(bool enabled = true);
	~XMLLoader();

	bool Awake();
	void LoadXML(const char* path, pugi::xml_document& doc);

	// Called before quitting
	bool CleanUp();

private:

public:

private:
	std::list<xml_doc*>	xmls;
};

#endif // !_J1XMLLOADER_H_

