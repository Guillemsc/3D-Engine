#ifndef __Console_H__
#define __Console_H__

#include "EditorUI.h"
#include "Module.h"

#define MAX_LINES 100	

class Application;

struct ImColor;
struct ImGuiTextEditCallbackData;

enum console_text_type
{
	console_text_type_default,   // 0
	console_text_type_info,		 // 1
	console_text_type_succes,	 // 2
	console_text_type_warning,	 // 3
	console_text_type_error,	 // 4
};

struct console_text
{
	string txt;
	console_text_type type = console_text_type_default;
};

class Console : public EditorElement
{
public:
	Console(bool start_enabled = true);
	virtual ~Console();

	void Draw();

	void AddLog(const char* txt, console_text_type type = console_text_type_default);
	void Clear();

private:
	void ScrollBottom();
	void CommandInput(const char* txt);
	ImColor GetColorByTextType(console_text_type type);
	void AddLogs();

private:
	list<console_text> console_items;
	int max_items						= 0;
	bool enabled						= false;
	char input_buffer[255];
	bool scroll_bottom					= false;
	bool send_text_input				= false;
};

static int TextChangeCallback(ImGuiTextEditCallbackData* data);

#endif // __Console_H__