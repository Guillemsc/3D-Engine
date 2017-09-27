#ifndef __Configuration_H__
#define __Configuration_H__

#include "EditorUI.h"
#include "Module.h"

struct ImGuiTextEditCallbackData;

struct hardware_info {
	char sdl_version[25] = "";
	float ram_gb = 0.f;
	uint cpu_count = 0;
	uint l1_cachekb = 0;
	bool rdtsc = false;
	bool altivec = false;
	bool now3d = false;
	bool mmx = false;
	bool sse = false;
	bool sse2 = false;
	bool sse3 = false;
	bool sse41 = false;
	bool sse42 = false;
	bool avx = false;
	bool avx2 = false;
	uint gpu_vendor = 0;
	uint gpu_device = 0;
	char gpu_brand[250] = "";
	float vram_mb_budget = 0.f;
	float vram_mb_usage = 0.f;
	float vram_mb_available = 0.f;
	float vram_mb_reserved = 0.f;
};

class Configuration : public EditorElement
{
public:
	Configuration(bool start_enabled = true);
	virtual ~Configuration();

	void Start();
	void Draw();

public:
	void UpdateInfo();

private:
	char name_input_buffer[255];
	char organization_input_buffer[255];
	char version_input_buffer[255];
	int  max_fps = 0;
	int  window_width = 0;
	int  window_height = 0;
	float brightness = 0.0f;
	bool fullscreen	= false;
	bool resizable = false;
	bool borderless	= false;
	bool fulldekstop = false;
	bool maximized = false;
	int  display_size_width = 0;
	int  display_size_height = 0;

	hardware_info info;
};

#endif