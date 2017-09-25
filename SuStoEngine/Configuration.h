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

private:
	char name_input_buffer[255];
	char organization_input_buffer[255];
	char version_input_buffer[255];
	int  max_fps = 0;
	int  window_width = 0;
	int  window_height = 0;
	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool fulldekstop = false;
	int  display_size_width = 0;
	int  display_size_height = 0;
};

#endif