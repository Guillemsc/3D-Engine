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
	cpus = "CPUs: " + std::to_string(App->profiler->GetCpuCount()) + (" (Cache: ") + std::to_string(App->profiler->GetCpuCacheLineSize()) + ")";
	sys_ram = "System RAM: " + std::to_string((int)(App->profiler->GetSystemRam() * 0.001)) + "Gb";
	caps += "Flags: ";

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

	ImGui::SetNextWindowSize(ImVec2(300, 300), 2);

	if (ImGui::Begin("Hardware", &visible))
	{
		ImGui::Text(cpus.c_str());
		ImGui::Text(sys_ram.c_str());
		ImGui::TextWrapped(caps.c_str());
	}

	ImGui::End();
}
