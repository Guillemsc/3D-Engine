#include "Console.h"
#include "App.h"
#include "imgui.h"

Console::Console(bool start_enabled)
{
}

Console::~Console()
{
}

bool Console::Awake()
{
	bool ret = true;

	memset(input_buffer, 0, sizeof(input_buffer));

	return ret;
}

bool Console::Update()
{
	bool ret = true;

	AddLogs();

	ImGui::SetNextWindowSize(ImVec2(520, 600), 2);

	if (ImGui::Begin("Console"))
	{
		ImGui::Text("Type help to see common comands");

		ImGui::Separator();

		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

		for (list<console_text>::iterator it = console_items.begin(); it != console_items.end(); it++)
		{
			ImVec4 col = GetColorByTextType((*it).type);
			ImGui::PushStyleColor(ImGuiCol_Text, col);
			ImGui::TextUnformatted((*it).txt.c_str());
			ImGui::PopStyleColor();
		}

		ImGui::PopStyleVar();
		ImGui::EndChild();

		ImGui::Separator();

		if (ImGui::InputText("Input", input_buffer, 255, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			LOG_OUTPUT(input_buffer);
			input_buffer[0] = '\0';
		}
		/*if (ImGui::InputText("Input", input_buffer, 255, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory), &TextChangeCallback, (void*)this)
		{

		}*/
	}
	ImGui::End();

	return ret;
}

bool Console::CleanUp()
{
	bool ret = true;

	return ret;
}

void Console::AddLog(const char * txt, console_text_type type)
{
	console_text ct;
	ct.txt = txt;
	ct.type = type;

	console_items.push_back(ct);
}

ImColor Console::GetColorByTextType(console_text_type type)
{
	ImColor ret(255, 255, 255, 255);

	switch (type)
	{
	case console_text_type::console_text_type_default:
		ret = { 255, 255, 255, 255 };
		break;

	case console_text_type::console_text_type_info:
		ret = { 255, 255, 255, 255 };
		break;

	case console_text_type::console_text_type_error:
		ret = { 255, 255, 255, 255 };
		break;

	case console_text_type::console_text_type_succes:
		ret = { 255, 255, 255, 255 };
		break;

	case console_text_type::console_text_type_warning:
		ret = { 255, 255, 255, 255 };
		break;
	default:
		break;
	}

	return ret;
}

void Console::AddLogs()
{
	for (list<string>::iterator it = App->logs.begin(); it != App->logs.end(); it++)
	{
		AddLog((*it).c_str());
	}

	App->logs.clear();
}

static int TextChangeCallback(ImGuiTextEditCallbackData * data)
{
	return 0;
}
