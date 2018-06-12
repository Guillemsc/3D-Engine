#ifndef __Console_H__
#define __Console_H__

#include "EditorUI.h"
#include "Module.h"

#define MAX_LINES 100	

class Application;

struct ImColor;
struct ImGuiTextEditCallbackData;

enum ConsoleTextType
{
	C_T_T_DEFAULT,
	C_T_T_INFO,
	C_T_T_SUCCES,
	C_T_T_WARNING,
	C_T_T_ERROR,
};

struct ConsoleText
{
	ConsoleText(std::string txt, ConsoleTextType type = ConsoleTextType::C_T_T_DEFAULT);

	std::string txt;
	ConsoleTextType type = ConsoleTextType::C_T_T_DEFAULT;
};

class Console : public EditorElement
{
public:
	Console(bool start_enabled = true);
	virtual ~Console();

	void Start();
	void Draw(uint flags);

	void AddLog(const char* txt, ConsoleTextType type = ConsoleTextType::C_T_T_DEFAULT);
	void AddLog(ConsoleText console_text);
	void Clear();

private:
	void ScrollBottom();
	void CommandInput(const char* txt);
	ImColor GetColorByTextType(ConsoleTextType type);
	void AddLogs();

private:
	list<ConsoleText> console_items;
	int  max_items = 0;
	bool enabled = false;
	char input_buffer[255];
	bool scroll_bottom = false;
	bool send_text_input = false;
};

#endif // __Console_H__