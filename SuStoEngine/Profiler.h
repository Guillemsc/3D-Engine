#ifndef __PROFILER_H__
#define __PROFILER_H__

#include "SDL\include\SDL.h"
#include <vector>
#include <string>
#include <iostream>
#include "PerfTimer.h"
#define MAX_FRAMES_LOGGED 50
#define MAX_MEMORY_LOGGED 50
#define MAX_PROFILE_LOGGED 5

class Profile
{
public:
	void Start();
	void Finish();

	int GetLastFrameTick();
	int GetLastFrameMs();

	std::vector<int> GetTicksList();
	std::vector<int> GetMsList();

public:
	std::string name;

private:
	PerfTimer timer;
	std::vector<int> ms;
	std::vector<int> ticks;

	int last_frame_ms				= 0.0f;
	int total_frames_ms				= 0.0f;
};

class Profiler
{
public:
	Profiler();
	~Profiler();
	void CleanUp();

	void Start();
	void UpdateFinish();

	float GetFrameTime();
	int GetFPS();
	float GetAvgFPS();
	int GetFramesSinceStartup();
	int GetTimeSinceStartup();

	void StartProfile(const char* name, ...);
	void AddToProfile(const char* name, ...);
	void FinishProfile(const char * name, ...);
	Profile* GetProfile(const char* name, ...);

	int GetProfilesCount();
	std::vector<float> GetFramesVector();
	std::vector<Profile*> GetProfilesList();
	std::vector<float> GetMemoryVector();

private:
	float cration_time				= 0.0f;

	// Update ----
	float update_start_time			= 0.0f;
	float update_time_since_startup = 0.0f;

	int   frames_since_startup		= 0;
	float frame_ms					= 0.0f;
	float avg_fps					= 0.0f;
	std::vector<float> frames;
	std::vector<float> memory;

	int frame_counter				= 0;
	float frame_counter_ms			= 0.0f;
	int last_second_frames			= 0;
	// -----------

	// Items -----
	std::vector<Profile*> profiles;
	// -----------
};

#endif //__PROFILER_H__