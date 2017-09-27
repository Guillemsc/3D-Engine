#include "ProfilerViewer.h"
#include "imgui.h"
#include "App.h"

ProfilerViewer::ProfilerViewer(bool start_enabled)
{
	visible = start_enabled;
}

ProfilerViewer::~ProfilerViewer()
{
}

void ProfilerViewer::Start()
{
	ms_ticks = 1;
	record = false;
}

void ProfilerViewer::Draw()
{
	if (!visible)
	{
		record = false;
		return;
	}

	ImGui::SetNextWindowSize(ImVec2(400, 800), 2);
	if (ImGui::Begin("Profiler", &visible))
	{
		ImGui::Text("Profiles: %d", App->profiler->GetProfilesCount());
		ImGui::RadioButton("ms", &ms_ticks, 1); ImGui::SameLine();
		ImGui::RadioButton("ticks", &ms_ticks, 2); 

		if (ImGui::Button("Record Runtime"))
		{
			record = !record;
			App->profiler->SetEnabled(record);
		}
		
		vector<Profile*> pr = App->profiler->GetProfilesList();

		// Update
		if (record)
		{
			if (ImGui::CollapsingHeader("Update", ImGuiTreeNodeFlags_DefaultOpen))
			{
				for (vector<Profile*>::iterator it = pr.begin(); it != pr.end(); it++)
				{
					if (!(*it)->startup)
					{
						if (ms_ticks == 1)
						{
							ImGui::Text("%s: %d ms", (*it)->name.c_str(), (*it)->GetLastFrameMs());
						}
						else
						{
							ImGui::Text("%s: %d ticks", (*it)->name.c_str(), (*it)->GetLastFrameTick());
						}
					}
				}
			}
		}

		// Start
		if (ImGui::CollapsingHeader("Startup", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (vector<Profile*>::iterator it = pr.begin(); it != pr.end(); it++)
			{
				if ((*it)->startup)
				{
					if (ms_ticks == 1)
					{
						ImGui::Text("%s: %d ms", (*it)->name.c_str(), (*it)->GetLastFrameMs());
					}
					else
					{
						ImGui::Text("%s: %d ticks", (*it)->name.c_str(), (*it)->GetLastFrameTick());
					}
				}
			}
		}
	}
	ImGui::End();
}
