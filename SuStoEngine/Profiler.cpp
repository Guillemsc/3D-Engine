#include "Profiler.h"
#include "Globals.h"
#include "Functions.h"
#include "SDL/include/SDL.h"
#include "gpudetect\DeviceId.h"
#include "mmgr\mmgr.h"

#define MAX_FRAMES_LOGGED 50
#define MAX_MEMORY_LOGGED 50
#define MAX_PROFILE_LOGGED 50

Profiler::Profiler()
{
	enabled = true;
	to_enable = true;
	time_since_startup.Start();

	std::wstring brand_ws;
	Uint64 vram_b, vram_u, vram_a, vram_r;

	if (getGraphicsDeviceInfo(&graphics_info.vendor_id, &graphics_info.device_id, &brand_ws, &vram_b, &vram_u, &vram_a, &vram_r))
	{
		sprintf_s(graphics_info.brand, 250, "%S", brand_ws.c_str());
		graphics_info.vram_budget_mb = float(vram_b) / (1024.f * 1024.f * 1024.f);
		graphics_info.vram_usage_mb = float(vram_u) / (1024.f * 1024.f * 1024.f);
		graphics_info.vram_avaliable_mb = float(vram_a) / (1024.f * 1024.f * 1024.f);
		graphics_info.vram_reserved_mb = float(vram_r) / (1024.f * 1024.f * 1024.f);
	}
}

Profiler::~Profiler()
{
}

void Profiler::CleanUp()
{
	for (std::vector<Profile*>::iterator it = profiles.begin(); it != profiles.end(); it++)
	{
		delete (*it);
	}

	profiles.clear();
}

void Profiler::SetEnabled(bool set)
{
	to_enable = set;
}

void Profiler::UpdateStart()
{
	frame_time.Start();
	enabled = to_enable;
}

void Profiler::UpdateFinish()
{
	// Frame time
	update_ms = frame_time.ReadMs();

	// Cap fps
	if (capped_ms > 0 && update_ms < capped_ms)
	{
		SDL_Delay(capped_ms - update_ms);
	}

	// Frames since start
	frames_since_startup++;

	// Avg fps
	avg_fps = float(frames_since_startup) / time_since_startup.ReadMs();

	// Frames / s
	frame_counter++;
	frame_counter_ms += frame_time.ReadMs();

	if (frame_counter_ms > 1000)
	{
		last_second_frames = frame_counter;
		frame_counter = 0;
		frame_counter_ms = frame_counter_ms - 1000;

		frames.push_back(last_second_frames);
		if (frames.size() > MAX_FRAMES_LOGGED) 
			frames.erase(frames.begin());
		
	}
	// -----------

	// Memory Statistics
	memory.push_back(m_getMemoryStatistics().totalActualMemory);

	if (memory.size() > MAX_MEMORY_LOGGED) 
		memory.erase(memory.begin());
	
	// -----------
}

void Profiler::SetMaxFps(int const &fps)
{
	if (fps > 0)
	{
		max_fps = fps;
		capped_ms = (float)(1000 / (float)fps);
	}
}

const int Profiler::GetMaxFps() const
{
	return max_fps;
}

const float Profiler::GetFrameTime() const
{
	return update_ms;
}

const int Profiler::GetFPS() const
{
	return last_second_frames;
}

const float Profiler::GetAvgFPS() const
{
	return avg_fps*1000;
}

const int Profiler::GetFramesSinceStartup() const
{
	return frames_since_startup;
}

const int Profiler::GetTimeSinceStartup() const
{
	return SDL_GetTicks();
}

void Profiler::StartProfile(bool startup, const char * name, ...)
{
	bool found = false;

	if (!enabled)
		return;

	char str[200];
	va_list args;
	va_start(args, name);
	vsprintf_s(str, 200, name, args);
	va_end(args);

	for (std::vector<Profile*>::iterator it = profiles.begin(); it != profiles.end(); it++)
	{
		if (TextCmp((*it)->name.c_str(), str))
		{
			(*it)->Start();

			found = true;
			break;
		}
	}

	if (!found)
	{
		Profile* prof = new Profile();
		prof->name = str;
		prof->Start();
		prof->startup = startup;
		profiles.push_back(prof);
	}
}

void Profiler::AddToProfile(const char * name, ...)
{
	//if (!enabled)
	//	return;

	//char str[200];
	//va_list args;
	//va_start(args, name);
	//vsprintf_s(str, 200, name, args);
	//va_end(args);

	//for (std::vector<Profile*>::iterator it = profiles.begin(); it != profiles.end(); it++)
	//{
	//	if ((*it)->name == str)
	//	{
	//		(*it)->timer.AddTime((*it)->timer.ReadMs());
	//		break;
	//	}
	//}
}

void Profiler::FinishProfile(const char* name, ...)
{
	if (!enabled)
		return;

	char str[200];
	va_list args;
	va_start(args, name);
	vsprintf_s(str, 200, name, args);
	va_end(args);

	for (std::vector<Profile*>::iterator it = profiles.begin(); it != profiles.end(); it++)
	{
		if (TextCmp((*it)->name.c_str(), str))
		{
			(*it)->Finish();
		}
	}
}

Profile* Profiler::GetProfile(const char * name, ...)
{
	char str[200];
	va_list args;
	va_start(args, name);
	vsprintf_s(str, 200, name, args);
	va_end(args);

	Profile* ret = nullptr;

	for (std::vector<Profile*>::iterator it = profiles.begin(); it != profiles.end(); it++)
	{
		if (TextCmp((*it)->name.c_str(), str))
		{
			ret = (*it);
			break;
		}
	}

	return nullptr;
}

int Profiler::GetProfilesCount() const
{
	return profiles.size();
}

std::vector<float> Profiler::GetFramesVector() const
{
	return frames;
}

std::vector<Profile*> Profiler::GetProfilesList() const
{
	return profiles;
}

std::vector<float> Profiler::GetMemoryVector() const
{
	return memory;
}

const int Profiler::GetCpuCount() const
{
	return SDL_GetCPUCount();
}

const int Profiler::GetCpuCacheLineSize() const
{
	return SDL_GetCPUCacheLineSize();
}

const int Profiler::GetSystemRam() const
{
	return SDL_GetSystemRAM();
}

const bool Profiler::Has3DNow() const
{
	return SDL_Has3DNow();
}

const bool Profiler::HasAVX() const
{
	return SDL_HasAVX();
}

const bool Profiler::HasAVX2() const
{
	return SDL_HasAVX2();
}

const bool Profiler::HasAltiVec() const
{
	return SDL_HasAltiVec();
}

const bool Profiler::HasMMX() const
{
	return SDL_HasMMX();
}

const bool Profiler::HasRDTSC() const
{
	return SDL_HasRDTSC();
}

const bool Profiler::HasSSE() const
{
	return SDL_HasSSE();
}

const bool Profiler::HasSSE2() const
{
	return SDL_HasSSE2();
}

const bool Profiler::HasSSE3() const
{
	return SDL_HasSSE3();
}

const bool Profiler::HasSSE41() const
{
	return SDL_HasSSE41();
}

const bool Profiler::HasSSE42() const
{
	return SDL_HasSSE42();
}

GraphicsDeviceInfo Profiler::GetInfo()
{
	Uint64 vram_b, vram_u, vram_a, vram_r;
	if (getGraphicsDeviceInfo(nullptr, nullptr, nullptr, &vram_b, &vram_u, &vram_a, &vram_r)) 
	{
		graphics_info.vram_budget_mb = float(vram_b) / (1024.f * 1024.f);
		graphics_info.vram_usage_mb = float(vram_u) / (1024.f * 1024.f);
		graphics_info.vram_avaliable_mb = float(vram_a) / (1024.f * 1024.f);
		graphics_info.vram_reserved_mb = float(vram_r) / (1024.f * 1024.f);
	}

	return graphics_info;
}

void Profile::Start()
{
	timer.Start();
}

void Profile::Finish()
{
	total_frames_ms += timer.ReadMs();

	ticks.push_back(timer.ReadTicks());

	ms.push_back(timer.ReadMs());

	if (ticks.size() > MAX_PROFILE_LOGGED)
	{
		ticks.erase(ticks.begin());
		ms.erase(ms.begin());
	}
}

const int Profile::GetLastFrameTick() const
{
	int ret = 0;

	if (!ticks.empty())
	{
		ret = ticks.front();
	}

	return ret;
}

const int Profile::GetLastFrameMs() const
{
	int ret = 0;

	if (!ms.empty())
	{
		ret = ms.front();
	}

	return ret;
}

const std::vector<int> Profile::GetTicksList() const
{
	return ticks;
}

const std::vector<int> Profile::GetMsList() const
{
	return ms;
}
