#include "IDGenerator.h"
#include "Functions.h"

IDGen::IDGen()
{
}

IDGen::~IDGen()
{
}

int IDGen::NewId(const char * id_name)
{
	int ret = 0;

	for (std::list<id_item>::iterator it = ids.begin(); it != ids.end(); it++)
	{
		if (TextCmp(id_name, (*it).name.c_str()))
		{
			return ++(*it).max_id;
		}
	}

	id_item id;
	id.name = id_name;
	id.max_id = 0;

	ids.push_back(id);

	return ret;
}

void IDGen::ClearIds()
{
	ids.clear();
}
