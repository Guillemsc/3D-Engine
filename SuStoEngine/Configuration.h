#ifndef __Configuration_H__
#define __Configuration_H__

#include "EditorUI.h"
#include "Module.h"

class Configuration : public EditorElement
{
public:
	Configuration(bool start_enabled = true);
	virtual ~Configuration();

	void Start();
	void Draw();

public:
};

#endif