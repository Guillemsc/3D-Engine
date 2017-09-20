#ifndef __Console_H__
#define __Console_H__

#include "Module.h"

class Application;

struct ImColor;
struct ImGuiTextEditCallbackData;

enum console_text_type
{
	console_text_type_default,
	console_text_type_info,
	console_text_type_succes,
	console_text_type_warning,
	console_text_type_error,
};

struct console_text
{
	string txt;
	console_text_type type = console_text_type_default;
};

class Console : public Module
{
public:
	Console(bool start_enabled = true);

	// Destructor
	virtual ~Console();

	bool Awake();
	bool Update();
	bool CleanUp();

	void AddLog(const char* txt, console_text_type type = console_text_type_default);

private:
	ImColor GetColorByTextType(console_text_type type);
	void AddLogs();

private:
	list<console_text> console_items;
	int max_items = 0;
	bool enabled = false;
	char input_buffer[255];
};

static int TextChangeCallback(ImGuiTextEditCallbackData* data);

#endif // __Console_H__