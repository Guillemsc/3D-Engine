#ifndef __Configuration_H__
#define __Configuration_H__

#include "Module.h"

class Configuration : public Module
{
public:
	Configuration(bool start_enabled = true);
	virtual ~Configuration();

	bool Awake();
	bool Start();
	bool Update();

public:
	bool visible = false;
};

#endif