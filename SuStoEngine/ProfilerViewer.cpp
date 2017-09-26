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
	ms_ticks = 1;
}

void ProfilerViewer::Draw()
{
	if (!visible)
		return;

	ImGui::SetNextWindowSize(ImVec2(400, 800), 2);
	if (ImGui::Begin("Profiler", &visible))
	{
		if (ImGui::CollapsingHeader("General", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::RadioButton("ms", &ms_ticks, 1); ImGui::SameLine();
			ImGui::RadioButton("ticks", &ms_ticks, 2);

			ImGui::Text("Profiles: %d", App->profiler->GetProfilesCount());

			ImGui::Separator();

			vector<Profile*> pr = App->profiler->GetProfilesList();

			for (vector<Profile*>::iterator it = pr.begin(); it != pr.end(); it++)
			{
				if(ms_ticks == 1)
					ImGui::Text("%s: %d ms" , (*it)->name.c_str(), (*it)->GetLastFrameMs());
				else
					ImGui::Text("%s: %d ticks", (*it)->name.c_str(), (*it)->GetLastFrameTick());
			}
		}
	}
	ImGui::End();
}
