#include "Configuration.h"
#include "App.h"
#include "ModuleWindow.h"
#include "imgui.h"
#include "gpudetect\DeviceId.h"
#include "mmgr\mmgr.h"
#include "SDL/include/SDL.h"

Configuration::Configuration(bool start_enabled) : EditorElement(start_enabled)
{
	memset(name_input_buffer, 0, sizeof(name_input_buffer));
	memset(organization_input_buffer, 0, sizeof(organization_input_buffer));
	memset(version_input_buffer, 0, sizeof(version_input_buffer));

	SDL_version version;
	SDL_GetVersion(&version);
	sprintf_s(info.sdl_version, 25, "%i.%i.%i", version.major, version.minor, version.patch);

	info.cpu_count = SDL_GetCPUCount();
	info.l1_cachekb = SDL_GetCPUCacheLineSize();
	info.ram_gb = App->profiler->GetSystemRam() / 1024; // Basic, 1024 MB = 1 GB

	info.altivec = SDL_HasAltiVec() == SDL_TRUE;
	info.avx = SDL_HasAVX() == SDL_TRUE;
	info.avx2 = SDL_HasAVX2() == SDL_TRUE;
	info.mmx = SDL_HasMMX() == SDL_TRUE;
	info.now3d = SDL_Has3DNow() == SDL_TRUE;
	info.rdtsc = SDL_HasRDTSC() == SDL_TRUE;
	info.sse = SDL_HasSSE() == SDL_TRUE;
	info.sse2 = SDL_HasSSE2() == SDL_TRUE;
	info.sse3 = SDL_HasSSE3() == SDL_TRUE;
	info.sse41 = SDL_HasSSE41() == SDL_TRUE;
	info.sse42 = SDL_HasSSE42() == SDL_TRUE;

	uint vendor_id, device_id;
	Uint64 vm, vm_curr, vm_a, vm_r;
	std::wstring brand;

	if (getGraphicsDeviceInfo(&vendor_id, &device_id, &brand, &vm, &vm_curr, &vm_a, &vm_curr))
	{
		info.gpu_vendor = vendor_id;
		info.gpu_device = device_id;
		sprintf_s(info.gpu_brand, 250, "%S", brand.c_str());
		info.vram_mb_budget = float(vm) / (1024.f * 1024.f);
		info.vram_mb_usage = float(vm_curr) / (1024.f * 1024.f);
		info.vram_mb_available = float(vm_a) / (1024.f * 1024.f);
		info.vram_mb_reserved = float(vm_curr) / (1024.f * 1024.f);
	}
	
}

Configuration::~Configuration()
{
}

void Configuration::Start()
{
	strcpy(name_input_buffer, App->GetAppName());
	strcpy(organization_input_buffer, App->GetAppOrganization());
	strcpy(version_input_buffer, App->GetVersion());
	max_fps = App->GetMaxFps();
	App->window->GetWindowSize(window_width, window_height);
	brightness = App->window->GetBrightness();
	fullscreen = App->window->GetFullscreen();
	resizable = App->window->GetResizable();
	borderless = App->window->GetBorderless();
	fulldekstop = App->window->GetFullDekstop();
	maximized = App->window->GetMaximized();
	App->window->GetWindowSize(display_size_width, display_size_height);
}

void Configuration::Draw()
{
	if (!visible)
		return;

	App->window->GetWindowSize(window_width, window_height);

	ImGui::SetNextWindowSize(ImVec2(400, 800), 2);

	if (ImGui::Begin("Configuration", &visible))
	{
		// App
		if (ImGui::CollapsingHeader("App"))
		{
			if (ImGui::InputText("App Name", name_input_buffer, 254))
			{
				App->SetAppName(name_input_buffer);
				App->SaveConfig();
			}

			if (ImGui::InputText("Organization", organization_input_buffer, 254))
			{
				App->SetAppOrganization(organization_input_buffer);
				App->SaveConfig();
			}
			if (ImGui::InputText("Version", version_input_buffer, 254))
			{
				App->SetVersion(version_input_buffer);
				App->SaveConfig();
			}
			if (ImGui::SliderInt("Max FPS", &max_fps, 0, 999))
			{
				App->SetMaxFps(max_fps);
				App->SaveConfig();
			}
			ImGui::Text("Average fps: %d fps", (int)App->profiler->GetAvgFPS());
			std::vector<float> framerate = App->profiler->GetFramesVector();

			if (!framerate.empty())
			{
				char title[25];
				sprintf_s(title, 25, "%.1f", framerate[framerate.size() - 1]);
				ImGui::PlotHistogram("Framerate", &framerate[0], framerate.size(), 0, title, 0.0f, 500.0f, ImVec2(0, 100));
			}

			std::vector<float> memory = App->profiler->GetMemoryVector();

			if (!memory.empty())
			{
				char title[25];
				sprintf_s(title, 25, "%.1f", memory[memory.size() - 1]);
				ImGui::PlotHistogram("Memory", &memory[0], memory.size(), 0, title, 0.0f, 30000.0f, ImVec2(0, 100));
			}

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

		// Hardware
		if (ImGui::CollapsingHeader("Hardware", ImGuiTreeNodeFlags_DefaultOpen))
		{
			UpdateInfo();
			ImGui::Text("SDL Version: ");				ImGui::SameLine();		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), info.sdl_version);
			ImGui::Separator(); //----------------------------------------------------------------------------------------------------------------------------------------------------------------
			ImGui::Text("CPUs: ");						ImGui::SameLine();		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%u (Cache: %ukb)", info.cpu_count, info.l1_cachekb);
			ImGui::Text("System RAM: ");				ImGui::SameLine();		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%.1fGb", info.ram_gb);
			ImGui::Text("Caps");						ImGui::SameLine();		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%s%s%s%s%s%s%s\n%s%s%s%s",	
			/*||*/	info.altivec ? "AltiVec, " : "",	info.avx ? "AVX, " : "",	info.avx2 ? "AVX2, " : "",		info.mmx ? "MMX, " : "",		info.now3d ? "3DNow, " : "",	info.rdtsc ? "RDTSC, " : "",
			/*||*/	info.sse ? "SSE, " : "",			info.sse2 ? "SSE2, " : "",	info.sse3 ? "SSE3, " : "",		info.sse41 ? "SSE41, " : "",	info.sse42 ? "SSE42, " : "");																											
			ImGui::Separator(); //----------------------------------------------------------------------------------------------------------------------------------------------------------------
			ImGui::Text("GPU: ");						ImGui::SameLine();		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "vendor %u device %u", info.gpu_vendor, info.gpu_device);
			ImGui::Text("Brand: ");						ImGui::SameLine();		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), info.gpu_brand);
			ImGui::Text("Video Memory: ");				ImGui::SameLine();		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%.1f Mb", info.vram_mb_budget);
			ImGui::Text("Video Memory On Use: ");		ImGui::SameLine();		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%.1f Mb", info.vram_mb_usage);
			ImGui::Text("Video Memory Available: ");	ImGui::SameLine();		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%.1f Mb", info.vram_mb_available);
			ImGui::Text("Video Memory Reserved: ");		ImGui::SameLine();		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%.1f Mb", info.vram_mb_reserved);
		}

		// Window
		if (ImGui::CollapsingHeader("Window"))
		{
			if (ImGui::SliderFloat("Brightness", &brightness, 0, 1))
			{
				App->window->SetBrightness(brightness);
				App->SaveConfig(App->window);
			}
			if (ImGui::InputInt("Width", &window_width, 100))
			{
				if (window_width > display_size_width)
					window_width = display_size_width;

				App->window->SetWindowSize(window_width, window_height);
				App->SaveConfig(App->window);
			}
			if (ImGui::InputInt("Height", &window_height, 100))
			{
				if (window_height > display_size_height)
					window_height = display_size_height;

				App->window->SetWindowSize(window_width, window_height);
				App->SaveConfig(App->window);
			}
			if (ImGui::Checkbox("Fullscren", &fullscreen))
			{
				App->window->SetFullscreen(fullscreen);
				App->SaveConfig(App->window);
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Resizable", &resizable))
			{
				App->window->SetResizable(resizable);
				App->SaveConfig(App->window);
			}
			if (ImGui::Checkbox("Borderless", &borderless))
			{
				App->window->SetBorderless(borderless);
				App->SaveConfig(App->window);
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Full Dekstop", &fulldekstop))
			{
				App->window->SetFullDekstop(fulldekstop);
				App->SaveConfig(App->window);
			}

			maximized = App->window->GetMaximized();
			if (ImGui::Checkbox("Maximized", &maximized))
			{
				App->window->SetMaximized(maximized);
				App->SaveConfig(App->window);
			}
		}

	}

	ImGui::End();
}

void Configuration::UpdateInfo()
{
	Uint64 vm, vm_curr, vm_a, vm_r;

	if (getGraphicsDeviceInfo(nullptr, nullptr, nullptr, &vm, &vm_curr, &vm_a, &vm_curr))
	{
		info.vram_mb_budget = float(vm) / (1024.f * 1024.f);
		info.vram_mb_usage = float(vm_curr) / (1024.f * 1024.f);
		info.vram_mb_available = float(vm_a) / (1024.f * 1024.f);
		info.vram_mb_reserved = float(vm_curr) / (1024.f * 1024.f);
	}
}



