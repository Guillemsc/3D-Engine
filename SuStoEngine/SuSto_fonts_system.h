#ifndef SUSTO_FONTS_SYSTEM
#define SUSTO_FONTS_SYSTEM

#include "SuStoUI.h"
#include "SuSto_event_system.h"
#include "susto_truetype.h"

class UIFont
{
public:
	UIFont();
	UIFont(const char* filepath, const char* name, unsigned char* bitmap, uint butmap_size, int width, int height, int line_height, stbtt_fontinfo info);
	~UIFont();

	void CleanUp();

	const char* GetName();
	unsigned char* GetBitmap();
	uint GetBitmapSize();
	int GetWidth();
	int GetHeight();
	int GetLineHeight();
	stbtt_fontinfo GetInfo();
	const char* GetFilePath();

private:
	const char* name = nullptr;
	unsigned char* bitmap = nullptr;
	uint bitmap_size = 0;
	int width = 0;
	int height = 0;
	int line_height = 0;
	stbtt_fontinfo info;
	const char* filepath = nullptr;
};

class UIFontsSystem
{
public:
	UIFontsSystem(SuStoUIMain* main);
	~UIFontsSystem();

	void CleanUp();

	void LoadFont(const char* filepath, const char* set_name);
	void UnloadFont(const char* name);
	UIFont* GetFont(const char* name);
	SuStoTexture LoadText(const char* text, UIFont* font, uint size);
	void UnloadText(uint id);

	std::function<uint(unsigned char * buffer, uint buffer_size, uint width, uint height)> LoadTexture;
	std::function<void(uint)> UnloadTexture;

private:
	SuStoUIMain* ui_main = nullptr;
	std::vector<UIFont*> fonts;
};

#endif