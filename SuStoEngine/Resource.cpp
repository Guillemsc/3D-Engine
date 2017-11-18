#include "Resource.h"

Resource::Resource(std::string _unique_id, ResourceType _type)
{
	unique_id = _unique_id;
	type = _type;
	count_ref = 0;
}

Resource::~Resource()
{
}

ResourceType Resource::GetType()
{
	return type;
}

std::string Resource::GetUniqueId()
{
	return unique_id;
}

bool Resource::IsUsed()
{
	return count_ref > 0 ? true : false;
}

int Resource::UsedCount()
{
	return count_ref;
}

void Resource::LoadMem()
{
	if (!IsUsed())
		LoadToMemory();

	count_ref++;
}

void Resource::UnloadMem()
{
	count_ref--;

	if (count_ref < 0)
		count_ref = 0;

	if (!IsUsed())
		UnloadFromMemory();
}

