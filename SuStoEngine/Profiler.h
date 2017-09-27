#ifndef __PROFILER_H__
#define __PROFILER_H__

#include <vector>
#include <string>
#include <iostream>
#include "PerfTimer.h"

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
	PerfTimer   timer;
	bool        startup = false;

private:
	int total_frames_ms = 0.0f;

	std::vector<int> ms;
	std::vector<int> ticks;
	int list_ticks = 0;
	int list_ms = 0;
};

struct GraphicsDeviceInfo
{
	unsigned int	   vendor_id = 0;
	unsigned int	   device_id = 0;
	std::string		   brand;
	unsigned int	   vram_budget_mb = 0;
	unsigned int       vram_usage_mb = 0;
	unsigned int       vram_avaliable_mb = 0;
	unsigned int       vram_reserved_mb = 0;
};

class Profiler
{
public:
	Profiler();
	~Profiler();
	void CleanUp();

	void SetEnabled(bool set);

	void UpdateStart();
	void UpdateFinish();

	void SetMaxFps(int fps);
	int GetMaxFps();
	float GetFrameTime();
	int GetFPS();
	float GetAvgFPS();
	int GetFramesSinceStartup();
	int GetTimeSinceStartup();

	void StartProfile(bool startup, const char* name, ...);
	void AddToProfile(const char* name, ...);
	void FinishProfile(const char * name, ...);
	Profile* GetProfile(const char* name, ...);

	int GetProfilesCount();
	std::vector<float> GetFramesVector();
	std::vector<Profile*> GetProfilesList();
	std::vector<float> GetMemoryVector();

	int GetCpuCount();
	int GetCpuCacheLineSize();
	int GetSystemRam();
	bool Has3DNow();
	bool HasAVX();
	bool HasAVX2();
	bool HasAltiVec();
	bool HasMMX();
	bool HasRDTSC();
	bool HasSSE();
	bool HasSSE2();
	bool HasSSE3();
	bool HasSSE41();
	bool HasSSE42();
	GraphicsDeviceInfo GetGraphicsDeviceInfo();

private:
	bool enabled = true;
	bool to_enable = false;

	// Start
	float cration_time = 0.0f;

	// Update ----
	PerfTimer frame_time;
	PerfTimer time_since_startup;

	int   frames_since_startup = 0;
	float update_ms = 0.0f;
	float avg_fps = 0.0f;
	int	  max_fps = 0;
	float capped_ms = -1;
	std::vector<float> frames;
	std::vector<float> memory;

	int frame_counter = 0;
	float frame_counter_ms = 0.0f;
	int last_second_frames = 0;
	// -----------

	// Items -----
	std::vector<Profile*> profiles;
	// -----------
};

#endif //__PROFILER_H__