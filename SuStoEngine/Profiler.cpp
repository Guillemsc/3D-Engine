#include "Profiler.h"
#include "Globals.h"
#include "Functions.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

#define MAX_FRAMES_LOGGED 50
#define MAX_MEMORY_LOGGED 50
#define MAX_PROFILE_LOGGED 100

Profiler::Profiler()
{
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

void Profiler::UpdateStart()
{
	frame_time.Start();
}

void Profiler::UpdateFinish()
{
	// Frames since start
	frames_since_startup++;

	// Frame time
	update_ms = frame_time.ReadMs();

	// Avg fps
	avg_fps = float(frames_since_startup) / time_since_startup.ReadMs();

	// Cap fps
	if (capped_ms > 0 && update_ms < capped_ms)
	{ 
		SDL_Delay(capped_ms - update_ms);
	}

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
	return avg_fps;
}

int Profiler::GetFramesSinceStartup()
{
	return frames_since_startup;
}

int Profiler::GetTimeSinceStartup()
{
	return SDL_GetTicks();
}

void Profiler::StartProfile(const char * name, ...)
{
	bool found = false;

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
		profiles.push_back(prof);
	}
}

void Profiler::AddToProfile(const char * name, ...)
{
	char str[200];
	va_list args;
	va_start(args, name);
	vsprintf_s(str, 200, name, args);
	va_end(args);

	//for (std::vector<Profile*>::iterator it = profiles.begin(); it != profiles.end(); it++)
	//{
	//	if ((*it)->name == str)
	//	{
	//		(*it)->frame_start -= (SDL_GetTicks() - (*it)->frame_start);
	//		break;
	//	}
	//}
}

void Profiler::FinishProfile(const char* name, ...)
{
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

	return ret;
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

void Profile::Start()
{
	timer.Start();
}

void Profile::Finish()
{
	list_ticks += timer.ReadTicks();
	ticks.push_back(timer.ReadTicks());

	list_ms += timer.ReadMs();
	ms.push_back(timer.ReadMs());

	total_frames_ms += timer.ReadMs();

	if (ticks.size() > MAX_PROFILE_LOGGED)
	{
		list_ticks -= (*ticks.begin());
		ticks.erase(ticks.begin());

		list_ms -= (*ms.begin());
		ms.erase(ms.begin());
	}
}

int Profile::GetLastFrameTick()
{
	int ret = 0;

	if (!ticks.empty())
	{
		ret = list_ticks / ticks.size();

		if (ret < 0)
			ret = 0;
	}

	return ret;
}

int Profile::GetLastFrameMs()
{
	int ret = 0;

	if (!ms.empty())
	{
		ret = list_ms / ms.size();

		if (ret < 0)
			ret = 0;
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
