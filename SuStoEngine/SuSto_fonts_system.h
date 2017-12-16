#ifndef SUSTO_FONTS_SYSTEM
#define SUSTO_FONTS_SYSTEM

#include "SuStoUI.h"
#include "SuSto_event_system.h"
#include "susto_truetype.h"

class UIFont
{
public:
	UIFont();
	~UIFont();

private:
	const char* name;
};

class UIFontsSystem
{
public:
	UIFontsSystem(SuStoUIMain* main);
	~UIFontsSystem();

	void CleanUp();

	void LoadFont(const char* filepath);

	std::function<uint(unsigned char * buffer, uint buffer_size, uint width, uint height)> LoadTexture;
	std::function<void(uint)> UnloadTexture;

private:
	SuStoUIMain* ui_main = nullptr;
	std::vector<UIFont> fonts;
};

#endif