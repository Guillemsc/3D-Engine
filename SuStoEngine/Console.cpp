#include "Console.h"
#include "App.h"
#include "ModuleInput.h"
#include "imgui.h"
#include "Functions.h"
#include "imgui_docking.h"

Console::Console(bool start_enabled) : EditorElement(start_enabled)
{
	memset(input_buffer, 0, sizeof(input_buffer));
}

Console::~Console()
{
}

void Console::Start()
{
}

void Console::Draw()
{
	bool ret = true;

	AddLogs();


	if (App->input->GetKey("º") == KEY_DOWN)
	{
		visible = !visible;
		return;
	}

	if (!visible)
		return;

	ImGui::SetNextWindowSize(ImVec2(450, 500), 2);

	if (igBeginDock("Console", &visible, 1))
	{
		if (ImGui::SmallButton("Clear"))
			CommandInput(".Clear");

		ImGui::SameLine();

		if (ImGui::SmallButton("Help"))
			CommandInput(".Help");

		ImGui::Separator();

		// Scrollable
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

		// Scrollable texts
		for (list<console_text>::iterator it = console_items.begin(); it != console_items.end(); it++)
		{
			ImVec4 col = GetColorByTextType((*it).type);
			ImGui::PushStyleColor(ImGuiCol_Text, col);
			ImGui::TextUnformatted((*it).txt.c_str());
			ImGui::PopStyleColor();
		}

		// Scroll to bottom
		if (scroll_bottom)
		{
			ImGui::SetScrollHere();
			scroll_bottom = false;
		}

		ImGui::PopStyleVar();
		ImGui::EndChild();

		ImGui::Separator();

		// Text input
		if (ImGui::InputText("", input_buffer, 254, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion
			| ImGuiInputTextFlags_CallbackHistory, &TextChangeCallback, (void*)this))
		{
			send_text_input = true;
		}

		// Keep focus on text input
		if (ImGui::IsItemHovered() || (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)))
			ImGui::SetKeyboardFocusHere(-1);

		// Submit button
		ImGui::SameLine();
		if (ImGui::Button("Submit", { 100, 23 }))
		{
			send_text_input = true;
		}

		// Input text
		if (send_text_input)
		{
			if (input_buffer != "")
				CommandInput(input_buffer);

			strcpy(input_buffer, "");

			send_text_input = false;
		}
	}
	igEndDock();
}

void Console::AddLog(const char * txt, console_text_type type)
{
	console_text ct;
	ct.txt = txt;
	ct.type = type;

	console_items.push_back(ct);

	if (console_items.size() > MAX_LINES) {
		console_items.pop_front();
	}

	ScrollBottom();
}

void Console::Clear()
{
	console_items.clear();
}

void Console::ScrollBottom()
{
	scroll_bottom = true;
}

void Console::CommandInput(const char * txt)
{
	if (strcmp(txt, ".Clear") == 0) 
	{
		Clear();
	}
	else if (strcmp(txt, ".Help") == 0) 
	{
		AddLog("-------------------------HELP-------------------------");
		AddLog("||\t.Clear  \t||\tClears the console");
		AddLog("||\t.Help   \t||\tShows the list of commands");
	}
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
		ret = { 255, 80, 80, 255 };
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
	int i = 0;
	return 1;
}