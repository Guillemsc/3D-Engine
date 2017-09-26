#ifndef __PROFILER_H__
#define __PROFILER_H__

#include "SDL\include\SDL.h"
#include <vector>
#include <string>
#include <iostream>
#define MAX_FRAMES_LOGGED 25

struct Profile
{
	std::string name;

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

	void Start();
	void UpdateFinish();

	float GetFrameTime();
	int GetFPS();
	float GetAvgFPS();
	int GetFramesSinceStartup();
	int GetTimeSinceStartup();

	void StartProfile(const char* name, ...);
	void FinishProfile(const char * name, ...);
	Profile* GetProfile(const char* name, ...);
	int GetProfilesCount();
	std::vector<float> GetFramesVector();
	std::vector<Profile*> GetProfilesList();

private:
	float cration_time = 0.0f;

	// Update ----
	float update_start_time = 0.0f;
	float update_time_since_startup = 0.0f;

	int   frames_since_startup = 0;
	float frame_ms = 0.0f;
	float avg_fps = 0.0f;
	std::vector<float> frames;

	int frame_counter = 0;
	float frame_counter_ms = 0.0f;
	int last_second_frames = 0;
	// -----------

	// Items -----
	std::vector<Profile*> profiles;
	// -----------
};

#endif //__PROFILER_H__