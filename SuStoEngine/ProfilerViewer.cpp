#include "ProfilerViewer.h"
#include "imgui.h"
#include "App.h"
#include "mmgr\mmgr.h"
#include "imgui_docking.h"

ProfilerViewer::ProfilerViewer(bool start_enabled)
{
	visible = start_enabled;
}

ProfilerViewer::~ProfilerViewer()
{
}

void ProfilerViewer::Start()
{
	profiler_mode = 1;
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
	if (igBeginDock("Profiler", &visible, 0))
	{
		ImGui::Text("Mode:");
		ImGui::RadioButton("Time", &profiler_mode, 1); ImGui::SameLine();
		ImGui::RadioButton("RAM Mem", &profiler_mode, 2); ImGui::SameLine();
		ImGui::RadioButton("VRAM Mem", &profiler_mode, 3);

		ImGui::Separator();
		ImGui::Separator();

		// Time profiler -----------------------------------
		if (profiler_mode == 1)
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
		// -------------------------------------------------
		else if (profiler_mode == 2)
		{
			ImGui::Text("Total Reported Mem: %d", m_getMemoryStatistics().totalReportedMemory);
			ImGui::Text("Peak Reported Mem: %d", m_getMemoryStatistics().peakReportedMemory);
			ImGui::Text("Total Actual Mem: %d", m_getMemoryStatistics().totalActualMemory);
			ImGui::Text("Peak Actual Mem: %d", m_getMemoryStatistics().peakActualMemory);
			ImGui::Text("Accumulated Reported Mem: %d", m_getMemoryStatistics().accumulatedReportedMemory);
			ImGui::Text("Accumulated Actual Mem: %d", m_getMemoryStatistics().accumulatedActualMemory);
			ImGui::Text("Accumulated Alloc Unit Count: %d", m_getMemoryStatistics().accumulatedAllocUnitCount);
			ImGui::Text("Total Alloc Unit Count: %d", m_getMemoryStatistics().totalAllocUnitCount);
			ImGui::Text("Peak Alloc Unit Count: %d", m_getMemoryStatistics().peakAllocUnitCount);
		}
		// Vram profiler ---------------------------------- -
		else if (profiler_mode == 3)
		{
			GraphicsDeviceInfo info = App->profiler->GetInfo();

			ImGui::Text("Video Memory: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%.1f Mb", info.vram_budget_mb);

			ImGui::Text("Video Memory On Use: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%.1f Mb", info.vram_usage_mb);

			ImGui::Text("Video Memory Available: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%.1f Mb", info.vram_avaliable_mb);

			ImGui::Text("Video Memory Reserved: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%.1f Mb", info.vram_reserved_mb);
		}
			
	}
	igEndDock();
}
