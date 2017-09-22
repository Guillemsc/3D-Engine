#include "Profiler.h"

Profiler::Profiler()
{
	startup_time = SDL_GetTicks();
	time_since_startup = 0.0f;
}

Profiler::~Profiler()
{
}

void Profiler::NewFrame()
{
	frames_since_startup++;
	frame_ms = SDL_GetTicks() - (startup_time + time_since_startup);
	avg_fps = float(frames_since_startup) / time_since_startup;

	frame_counter++;
	frame_counter_ms += frame_ms;
	if (frame_counter_ms > 1000)
	{
		last_second_frames = frame_counter;
		frame_counter = 0;
		frame_counter_ms = frame_counter_ms - 1000;
	}

	time_since_startup = SDL_GetTicks() - startup_time;
}

float Profiler::GetFrameTime()
{
	return frame_ms;
}

int Profiler::GetFPS()
{
	return last_second_frames;
}

int Profiler::GetFramesSinceStartup()
{
	return frames_since_startup;
}

int Profiler::GetTimeSinceStartup()
{
	return time_since_startup;
}
