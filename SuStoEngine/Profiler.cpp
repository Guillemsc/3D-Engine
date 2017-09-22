#include "Profiler.h"
#include "Globals.h"

Profiler::Profiler()
{
}

Profiler::~Profiler()
{
}

void Profiler::AwakeFinish()
{
	awake_total_time = SDL_GetTicks();
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
	}
	// -----------

	// Update time since startup
	update_time_since_startup = SDL_GetTicks() - update_start_time;
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
