#ifndef __RESOURCES_MONITORING_H__
#define __RESOURCES_MONITORING_H__

#include "EditorUI.h"
#include "Module.h"
#include <vector>
#include "ModuleFileSystem.h"

class Resource;

class ResourcesMonitoring : public EditorElement
{
public:
	ResourcesMonitoring(bool start_enabled = true);
	virtual ~ResourcesMonitoring();

	void Start();
	void Draw(uint flags);
};

#endif