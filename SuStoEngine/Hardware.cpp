#include "Hardware.h"
#include "imgui.h"
#include "App.h"
#include "Globals.h"
#include "Profiler.h"
#include "ModuleWindow.h"
#include "gpudetect\DeviceId.h"

Hardware::Hardware(bool start_enabled) : EditorElement(start_enabled)
{

}

Hardware::~Hardware()
{
}

void Hardware::Start()
{
	sdl_ver = std::to_string(App->GetSDLVersion().major) + "." + std::to_string(App->GetSDLVersion().minor) + "." + std::to_string(App->GetSDLVersion().patch);
	cpus = std::to_string(App->profiler->GetCpuCount()) + (" (Cache: ") + std::to_string(App->profiler->GetCpuCacheLineSize()) + "kb)";
	sys_ram = "System RAM: " + std::to_string((int)(App->profiler->GetSystemRam() / 1024)) + "Gb";

	if (App->profiler->Has3DNow())
		caps += "3DNow, ";
	if (App->profiler->HasAltiVec())
		caps += "AltiVec, ";
	if (App->profiler->HasAVX())
		caps += "AVX, ";
	if (App->profiler->HasAVX2())
		caps += "AVX2, ";
	if (App->profiler->HasMMX())
		caps += "MMX, ";
	if (App->profiler->HasRDTSC())
		caps += "RDTSC, ";
	if (App->profiler->HasSSE())
		caps += "SSE, ";
	if (App->profiler->HasSSE2())
		caps += "SSE2, ";
	if (App->profiler->HasSSE3())
		caps += "SSE3, ";
	if (App->profiler->HasSSE41())
		caps += "SSE41, ";
	if (App->profiler->HasSSE42())
		caps += "SSE42";

	App->window->GetDisplaySize(window_w, window_h);
}

void Hardware::Draw()
{
	if (!visible)
		return;

	ImGuiStyle * style = &ImGui::GetStyle();
	ImVec4 sec_colour = style->Colors[ImGuiCol_ComboBg];
	sec_colour.x += +0.2f;
	sec_colour.y += +0.2f;
	sec_colour.z += +0.2f;

	if (ImGui::Begin("Hardware", &visible, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("SDL_Version:"); 
		ImGui::SameLine(); 
		ImGui::TextColored(sec_colour, sdl_ver.c_str());

		ImGui::Separator();

		ImGui::Text("CPUs: "); 
		ImGui::SameLine(); 
		ImGui::TextColored(sec_colour, cpus.c_str());

		ImGui::Text("System RAM:"); 
		ImGui::SameLine(); 
		ImGui::TextColored(sec_colour, sys_ram.c_str());

		ImGui::Text("Caps:"); 
		ImGui::SameLine(); 
		ImGui::TextColored(sec_colour, caps.c_str());

		ImGui::Separator();

		GraphicsDeviceInfo info = App->profiler->GetInfo();

		ImGui::Text("GPU:");  
		ImGui::SameLine(); 
		ImGui::TextColored(sec_colour, "Vendor %d Device %d", info.vendor_id, info.device_id);

		ImGui::Text("Brand:"); 
		ImGui::SameLine(); 
		ImGui::TextColored(sec_colour, info.brand);

		ImGui::Separator();

		ImGui::Text("Display resolution:");
		ImGui::SameLine();
		ImGui::TextColored(sec_colour, "%d x %d", window_w, window_h);
	}

	ImGui::End();
}
