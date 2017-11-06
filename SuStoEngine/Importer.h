#ifndef _IMPORTER_H_
#define _IMPORTER_H_

#include "Globals.h"

#include <string>

template <class TYPE>
class Importer
{
public:
	virtual TYPE* Load(const char* exported_file) { return false; };
	virtual bool Save(const char* exported_file) { return false; }
};


#endif // !_IMPORTER_H_