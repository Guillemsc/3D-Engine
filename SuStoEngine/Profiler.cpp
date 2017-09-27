#include "Profiler.h"
#include "Globals.h"
#include "Functions.h"
#include "SDL/include/SDL.h"
#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"
#include "gpudetect\DeviceId.h"

#define MAX_FRAMES_LOGGED 50
#define MAX_MEMORY_LOGGED 50
#define MAX_PROFILE_LOGGED 50

Profiler::Profiler()
{
	enabled = true;
	to_enable = true;
	time_since_startup.Start();
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

void Profiler::SetMaxFps(int fps)
{
	if (fps > 0)
	{
		max_fps = fps;
		capped_ms = (float)(1000 / (float)fps);
	}
}

int Profiler::GetMaxFps()
{
	return max_fps;
}

float Profiler::GetFrameTime()
{
	return update_ms;
}

int Profiler::GetFPS()
{
	return last_second_frames;
}

float Profiler::GetAvgFPS()
{
	return avg_fps*1000;
}

int Profiler::GetFramesSinceStartup()
{
	return frames_since_startup;
}

int Profiler::GetTimeSinceStartup()
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

int Profiler::GetProfilesCount()
{
	return profiles.size();
}

std::vector<float> Profiler::GetFramesVector()
{
	return frames;
}

std::vector<Profile*> Profiler::GetProfilesList()
{
	return profiles;
}

std::vector<float> Profiler::GetMemoryVector()
{
	return memory;
}

int Profiler::GetCpuCount()
{
	return SDL_GetCPUCount();
}

int Profiler::GetCpuCacheLineSize()
{
	return SDL_GetCPUCacheLineSize();
}

int Profiler::GetSystemRam()
{
	return SDL_GetSystemRAM();
}

bool Profiler::Has3DNow()
{
	return SDL_Has3DNow();
}

bool Profiler::HasAVX()
{
	return SDL_HasAVX();
}

bool Profiler::HasAVX2()
{
	return SDL_HasAVX2();
}

bool Profiler::HasAltiVec()
{
	return SDL_HasAltiVec();
}

bool Profiler::HasMMX()
{
	return SDL_HasMMX();
}

bool Profiler::HasRDTSC()
{
	return SDL_HasRDTSC();
}

bool Profiler::HasSSE()
{
	return SDL_HasSSE();
}

bool Profiler::HasSSE2()
{
	return SDL_HasSSE2();
}

bool Profiler::HasSSE3()
{
	return SDL_HasSSE3();
}

bool Profiler::HasSSE41()
{
	return SDL_HasSSE41();
}

bool Profiler::HasSSE42()
{
	return SDL_HasSSE42();
}

GraphicsDeviceInfo Profiler::GetGraphicsDeviceInfo()
{
	GraphicsDeviceInfo graphics_info;

	std::wstring brand_ws;
	if (getGraphicsDeviceInfo(&graphics_info.vendor_id, &graphics_info.device_id, &brand_ws, &graphics_info.vram_budget_mb,
		&graphics_info.vram_usage_mb, &graphics_info.vram_avaliable_mb, &graphics_info.vram_reserved_mb))
	{
		graphics_info.brand.assign(brand_ws.begin(), brand_ws.end());
		graphics_info.vram_budget_mb /= (1024.f * 1024.f);
		graphics_info.vram_usage_mb /= (1024.f * 1024.f);
		graphics_info.vram_avaliable_mb /= (1024.f * 1024.f);
		graphics_info.vram_reserved_mb /= (1024.f * 1024.f);
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

int Profile::GetLastFrameTick()
{
	int ret = 0;

	if (!ticks.empty())
	{
		ret = ticks.front();
	}

	return ret;
}

int Profile::GetLastFrameMs()
{
	int ret = 0;

	if (!ms.empty())
	{
		ret = ms.front();
	}

	return ret;
}

std::vector<int> Profile::GetTicksList()
{
	return ticks;
}

std::vector<int> Profile::GetMsList()
{
	return ms;
}
