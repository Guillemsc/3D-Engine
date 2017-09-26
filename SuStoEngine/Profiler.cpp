#include "Profiler.h"
#include "Globals.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

Profiler::Profiler()
{
	cration_time = SDL_GetTicks();
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

void Profiler::AwakeFinish()
{
	awake_total_time = SDL_GetTicks() - cration_time;
}

void Profiler::StartFinish()
{
	start_total_time = SDL_GetTicks() - awake_total_time;

	update_start_time = SDL_GetTicks();
	update_time_since_startup = 0.0f;
}

void Profiler::UpdateFinish()
{
	frames_since_startup++;
	frame_ms = SDL_GetTicks() - (update_start_time + update_time_since_startup);
	avg_fps = float(frames_since_startup) / update_time_since_startup;

	// Frames / s
	frame_counter++;
	frame_counter_ms += frame_ms;
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

	if (memory.size() > MAX_MEMORY_LOGGED) {
		memory.erase(memory.begin());
	}
	// -----------

	// Update time since startup
	update_time_since_startup = SDL_GetTicks() - update_start_time;
}

float Profiler::GetAwakeTime()
{
	return awake_total_time;
}

float Profiler::GetStartTime()
{
	return start_total_time;
}

float Profiler::GetFrameTime()
{
	return frame_ms;
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

void Profiler::StartProfile(const char * name)
{
	bool found = false;

	for (std::vector<Profile*>::iterator it = profiles.begin(); it != profiles.end(); it++)
	{
		if ((*it)->name == name)
		{
			(*it)->frame_start = SDL_GetTicks();
			current_profile = (*it);

			found = true;
			break;
		}
	}

	if (!found)
	{
		Profile* prof = new Profile();
		prof->name = name;
		prof->frame_start = SDL_GetTicks();
		profiles.push_back(prof);

		current_profile = prof;
	}
}

void Profiler::FinishProfile()
{
	if (current_profile != nullptr)
	{
		current_profile->last_frame_ms = SDL_GetTicks() - current_profile->frame_start;
		current_profile->total_frames_ms += current_profile->last_frame_ms;
		current_profile = nullptr;
	}
}

Profile * Profiler::GetProfile(const char * name)
{
	Profile* ret = nullptr;

	for (std::vector<Profile*>::iterator it = profiles.begin(); it != profiles.end(); it++)
	{
		if ((*it)->name == name)
		{
			ret = (*it);
			break;
		}
	}

	return ret;
}

std::vector<float> Profiler::GetFramesVector()
{
	return frames;
}

std::vector<float> Profiler::GetMemoryVector()
{
	return memory;
}
