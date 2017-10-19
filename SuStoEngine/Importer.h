#ifndef _IMPORTER_H_
#define _IMPORTER_H_

#include "Globals.h"

#include <string>

class Importer
{
public:
	virtual bool Import(const char* file, const char* path, std::string& output_file) { return false; };
	virtual bool Import(const void* buffer, uint size, std::string& output_file) { return false; };
	virtual bool Load(const char* exported_file) { return false; };

};


#endif // !_IMPORTER_H_