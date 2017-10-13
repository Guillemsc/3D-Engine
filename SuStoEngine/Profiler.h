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

	const int GetLastFrameTick() const;
	const int GetLastFrameMs() const;

	const std::vector<int> GetTicksList() const;
	const std::vector<int> GetMsList() const;

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
	unsigned int	vendor_id = 0;
	unsigned int	device_id = 0;
	char			brand[250] = "";
	float			vram_budget_mb = 0.f;
	float			vram_usage_mb = 0.f;
	float			vram_avaliable_mb = 0.f;
	float			vram_reserved_mb = 0.f;
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

	void SetMaxFps(int const &fps);
	const int GetMaxFps() const;
	const float GetFrameTime() const;
	const int GetFPS() const;
	const float GetAvgFPS() const;
	const int GetFramesSinceStartup() const;
	const int GetTimeSinceStartup() const;

	void StartProfile(bool startup, const char* name, ...);
	void AddToProfile(const char* name, ...);
	void FinishProfile(const char * name, ...);
	Profile* GetProfile(const char* name, ...);

	int GetProfilesCount() const;
	std::vector<float> GetFramesVector() const;
	std::vector<Profile*> GetProfilesList() const;
	std::vector<float> GetMemoryVector() const;

	const int GetCpuCount() const;
	const int GetCpuCacheLineSize() const;
	const int GetSystemRam() const;
	const bool Has3DNow() const;
	const bool HasAVX() const;
	const bool HasAVX2() const;
	const bool HasAltiVec() const;
	const bool HasMMX() const;
	const bool HasRDTSC() const;
	const bool HasSSE() const;
	const bool HasSSE2() const;
	const bool HasSSE3() const;
	const bool HasSSE41()  const;
	const bool HasSSE42() const;
	GraphicsDeviceInfo GetInfo();

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
	GraphicsDeviceInfo graphics_info;
};

#endif //__PROFILER_H__