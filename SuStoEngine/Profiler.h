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
	void AwakeFinish();
	void StartFinish();
	void UpdateFinish();

	float GetFrameTime();
	int GetFPS();
	float GetAvgFPS();
	int GetFramesSinceStartup();
	int GetTimeSinceStartup();

private:
	float startup_time = 0.0f;

	// Awake -----
	float awake_total_time = 0.0f;
	// -----------

	// Start -----
	float start_total_time = 0.0f;
	// -----------

	// Update ----
	float update_start_time = 0.0f;
	float update_time_since_startup = 0.0f;

	int   frames_since_startup = 0;
	float frame_ms = 0.0f;
	float avg_fps = 0.0f;

	int frame_counter = 0;
	float frame_counter_ms = 0.0f;
	int last_second_frames = 0;
	// -----------
};

#endif //__PROFILER_H__