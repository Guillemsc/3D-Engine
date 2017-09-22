#ifndef __PROFILER_H__
#define __PROFILER_H__

#include "SDL\include\SDL.h"

struct Profile
{
	float last_frame_ms = 0.0f;
	float total_frames_ms = 0.0f;
};

class Profiler
{
public:
	Profiler();
	~Profiler();
	void NewFrame();

	float GetFrameTime();
	int GetFPS();
	int GetFramesSinceStartup();
	int GetTimeSinceStartup();

private:
	float startup_time = 0.0f;
	float time_since_startup = 0.0f;

	int   frames_since_startup = 0;
	float frame_ms = 0.0f;
	float avg_fps = 0.0f;

	int frame_counter = 0;
	float frame_counter_ms = 0.0f;
	int last_second_frames = 0;
};

#endif //__PROFILER_H__