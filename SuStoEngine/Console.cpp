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

void Console::Draw(uint flags)
{
	flags |= ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

	AddLogs();
	
	if (App->input->GetKeyBindingDown("console"))
	{
		visible = !visible;
	}

	ImGui::BeginDock("Console", false, &visible, false, flags);
		
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
	for (list<ConsoleText>::iterator it = console_items.begin(); it != console_items.end(); it++)
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
		| ImGuiInputTextFlags_CallbackHistory))
	{
		send_text_input = true;
	}

	// Keep focus on text input
	//if (ImGui::IsItemHovered() || (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)))
	//	ImGui::SetKeyboardFocusHere(-1);

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
	
	ImGui::EndDock();
}

void Console::AddLog(const char * txt, ConsoleTextType type)
{
	AddLog(ConsoleText(txt, type));
}

void Console::AddLog(ConsoleText console_text)
{
	console_items.push_back(console_text);

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

ImColor Console::GetColorByTextType(ConsoleTextType type)
{
	ImColor ret(255, 255, 255, 255);

	switch (type)
	{
	case ConsoleTextType::C_T_T_DEFAULT:
		ret = { 255, 255, 255, 255 };
		break;

	case ConsoleTextType::C_T_T_INFO:
		ret = { 255, 255, 255, 255 };
		break;

	case ConsoleTextType::C_T_T_ERROR:
		ret = { 255, 80, 80, 255 };
		break;

	case ConsoleTextType::C_T_T_SUCCES:
		ret = { 255, 255, 255, 255 };
		break;

	case ConsoleTextType::C_T_T_WARNING:
		ret = { 255, 255, 255, 255 };
		break;
	default:
		break;
	}

	return ret;
}

void Console::AddLogs()
{
	list<ConsoleText> logs = App->GetLogs();

	for (list<ConsoleText>::iterator it = logs.begin(); it != logs.end(); it++)
	{
		AddLog((*it));
	}

	App->ClearLogs();
}

ConsoleText::ConsoleText(std::string _txt, ConsoleTextType _type)
{
	txt = _txt;
	type = _type;
}
