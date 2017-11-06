#ifndef _IMPORTER_H_
#define _IMPORTER_H_

#include "Globals.h"

#include <string>

class Importer
{
public:
	virtual bool Load(const char* exported_file) { return false; };
	virtual bool Save(const char* exported_file) { return false; }
};


#endif // !_IMPORTER_H_