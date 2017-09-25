#include "About.h"
#include "App.h"
#include "imgui.h"

About::About(bool start_enabled) : EditorElement(start_enabled)
{
}

About::~About()
{
}

void About::Start()
{
}

void About::Draw()
{
	if (!visible)
		return;

	if (ImGui::Begin("About SuSto Engine", &visible, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("SuSto Engine %s", App->GetVersion());
		ImGui::Separator();
		ImGui::Text("By Guillem Sunyer and Simon Stoyanov.");
		ImGui::Text("3D Engine made for the 3D engines subject during the 3rd year of Game Design and Game Development degree at CITM,");
		if (ImGui::Button("Github Repository"))
		{
			App->GoToBrowser("https://github.com/Guillemsc/3D-Engine");
		}
		ImGui::SameLine();
		if (ImGui::Button("Download Latest Release"))
		{
			App->GoToBrowser("https://github.com/Guillemsc/3D-Engine/releases");
		}
		ImGui::SameLine();
		if (ImGui::Button("Report Issue"))
		{
			App->GoToBrowser("https://github.com/Guillemsc/3D-Engine/issues");
		}
		ImGui::Separator();
		if (ImGui::CollapsingHeader("Libraries"))
		{
			ImGui::Columns(3, "Name");
			ImGui::Separator();
			ImGui::Text("Use"); ImGui::NextColumn();
			ImGui::Text("Name"); ImGui::NextColumn();
			ImGui::Text("Version"); ImGui::NextColumn();
			ImGui::Separator();
			const char* use[3] = { "Math", "UI", "File System" };
			const char* name[3] = { "MathGeoLib", "ImGui", "Parson" };
			const char* version[3] = { "v1.5", "v1.52", "---" };
			static int selected = -1;
			for (int i = 0; i < 3; i++)
			{
				ImGui::Text(use[i]); ImGui::NextColumn();
				ImGui::Text(name[i]); ImGui::NextColumn();
				ImGui::Text(version[i]); ImGui::NextColumn();
			}
			ImGui::Columns(1);
			ImGui::Separator();
		}
		if (ImGui::CollapsingHeader("License"))
		{
			ImGui::Text("MIT License");
			ImGui::Text("Copyright(c) 2017 Guillem Sunyer Caldu and Simon Stoyanov Beltran");
			ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy");
			ImGui::Text("of this software and associated documentation files(the 'Software'), to deal");
			ImGui::Text("in the Software without restriction, including without limitation the rights");
			ImGui::Text("to use, copy, modify, merge, publish, distribute, sublicense, and / or sell");
			ImGui::Text("copies of the Software, and to permit persons to whom the Software is");
			ImGui::Text("furnished to do so, subject to the following conditions :");

			ImGui::TextColored({ 1, 0.2f, 0.2f, 1 }, "The above copyright notice and this permission notice shall be included in all");
			ImGui::TextColored({ 1, 0.2f, 0.2f, 1 }, "copies or substantial portions of the Software.");

			ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
			ImGui::Text("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
			ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE");
			ImGui::Text("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
			ImGui::Text("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
			ImGui::Text("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE");
			ImGui::Text("SOFTWARE.");
		}

	}
	ImGui::End();
}
