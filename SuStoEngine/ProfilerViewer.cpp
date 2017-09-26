#include "ProfilerViewer.h"
#include "imgui.h"
#include "App.h"

ProfilerViewer::ProfilerViewer(bool start_enabled)
{
}

ProfilerViewer::~ProfilerViewer()
{
}

void ProfilerViewer::Start()
{
}

void ProfilerViewer::Draw()
{
	if (!visible)
		return;

	ImGui::SetNextWindowSize(ImVec2(400, 800), 2);

	if (ImGui::Begin("Profiler", &visible))
	{
		ImGui::SetNextTreeNodeOpen(true);
		if (ImGui::CollapsingHeader("General"))
		{
			ImGui::Text("Profiles: %d", App->profiler->GetProfilesCount());

			vector<Profile*> pr = App->profiler->GetProfilesList();

			for (vector<Profile*>::iterator it = pr.begin(); it != pr.end(); it++)
			{
				ImGui::Text("%s: %d ms" , (*it)->name.c_str(), (*it)->last_frame_ms);
			}
		}
	}
	ImGui::End();

}
