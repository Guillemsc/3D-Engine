#ifndef __PROFILER_H__
#define __PROFILER_H__

#include "SDL\include\SDL.h"
#include <vector>
#include <iostream>
#define MAX_FRAMES_LOGGED 100
#define MAX_MEMORY_LOGGED 100

struct Profile
{
	const char* name;

	float last_frame_ms = 0.0f;
	float frame_start = 0.0f;
	float total_frames_ms = 0.0f;
};

class Profiler
{
public:
	Profiler();
	~Profiler();
	void CleanUp();

	void AwakeFinish();
	void StartFinish();
	void UpdateFinish();

	float GetAwakeTime();
	float GetStartTime();
	float GetFrameTime();
	int GetFPS();
	float GetAvgFPS();
	int GetFramesSinceStartup();
	int GetTimeSinceStartup();

	void StartProfile(const char* name);
	void FinishProfile();
	Profile* GetProfile(const char* name);
	std::vector<float> GetFramesVector();
	std::vector<float> GetMemoryVector();

private:
	float cration_time = 0.0f;

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
	std::vector<float> frames;
	std::vector<float> memory;

	int frame_counter = 0;
	float frame_counter_ms = 0.0f;
	int last_second_frames = 0;
	// -----------

	// Items -----
	Profile* current_profile = nullptr;
	std::vector<Profile*> profiles;
	// -----------
};

#endif //__PROFILER_H__