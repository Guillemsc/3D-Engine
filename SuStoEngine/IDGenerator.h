#ifndef __IDGENERATOR_H__
#define __IDGENERATOR_H__

#include <iostream>
#include <list>

struct id_item
{
	std::string name;
	int max_id = 0;
};

class IDGen
{
public:
	IDGen();
	~IDGen();

	int NewId(const char* id_name);
	void ClearIds();

private:
	std::list<id_item> ids;

};

#endif //__IDGENERATOR_H__