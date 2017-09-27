#include "Hardware.h"
#include "imgui.h"
#include "App.h"
#include "Globals.h"

Hardware::Hardware(bool start_enabled) : EditorElement(start_enabled)
{

}

Hardware::~Hardware()
{
}

void Hardware::Start()
{
	sdl_ver = std::to_string(App->GetSDLVersion().major) + "." + std::to_string(App->GetSDLVersion().minor) + "." + std::to_string(App->GetSDLVersion().patch);
	cpus = std::to_string(App->profiler->GetCpuCount()) + (" (Cache: ") + std::to_string(App->profiler->GetCpuCacheLineSize()) + ")";
	sys_ram = "System RAM: " + std::to_string((int)(App->profiler->GetSystemRam() * 0.001)) + "Gb";

	if (App->profiler->Has3DNow())
		caps += "3DNow, ";
	if (App->profiler->HasAltiVec())
		caps += "AltiVec, ";
	if (App->profiler->HasAVX())
		caps += "AVX, ";
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
		caps += "SSE42, ";
}

void Hardware::Draw()
{
	if (!visible)
		return;

	if (ImGui::Begin("Hardware", &visible, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("SDL_Version:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), sdl_ver.c_str());
		ImGui::Separator();
		ImGui::Text("CPUs:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), cpus.c_str());
		ImGui::Text("System RAM:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), sys_ram.c_str());
		ImGui::Text("Caps:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), caps.c_str());
	}

	ImGui::End();
}
