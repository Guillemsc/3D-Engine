#define STB_TRUETYPE_IMPLEMENTATION

#include "SuStoUI.h"
#include "SuSto_fonts_system.h"

UIFontsSystem::UIFontsSystem(SuStoUIMain * _ui_main)
{
	ui_main = _ui_main;
}

UIFontsSystem::~UIFontsSystem()
{
}

void UIFontsSystem::CleanUp()
{
}

void UIFontsSystem::LoadFont(const char * filepath, const char* set_name)
{
	long size = 0;
	unsigned char* fontBuffer = nullptr;

	FILE* fontFile = fopen(filepath, "rb");
	fseek(fontFile, 0, SEEK_END);
	size = ftell(fontFile); /* how long is the file ? */
	fseek(fontFile, 0, SEEK_SET); /* reset */

	fontBuffer = new unsigned char[size];

	fread(fontBuffer, size, 1, fontFile);
	fclose(fontFile);

	stbtt_fontinfo info;
	if (!stbtt_InitFont(&info, fontBuffer, 0))
	{
		LOG_OUTPUT("Loading font failed");
	}

	int b_w = 512; /* bitmap width */
	int b_h = 128; /* bitmap height */
	int l_h = 64; /* line height */


	UIFont* font = new UIFont(filepath, set_name, fontBuffer, size, b_w, b_h, l_h, info);
	fonts.push_back(font);
}

void UIFontsSystem::UnloadFont(const char * name)
{
	for (std::vector<UIFont*>::iterator it = fonts.begin(); it != fonts.end(); ++it)
	{
		if (SuStoUI::TextCmp((*it)->GetName(), name))
		{
			(*it)->CleanUp();
			RELEASE(*it);
			fonts.erase(it);
			break;
		}
	}
}

UIFont* UIFontsSystem::GetFont(const char * name)
{
	UIFont* ret = nullptr;

	for (std::vector<UIFont*>::iterator it = fonts.begin(); it != fonts.end(); ++it)
	{
		if (SuStoUI::TextCmp((*it)->GetName(), name))
		{
			return (*it);
		}
	}

	return ret;
}

SuStoTexture UIFontsSystem::LoadText(const char * text, UIFont * font, uint size)
{
	stbtt_fontinfo info = font->GetInfo();

	float scale = stbtt_ScaleForPixelHeight(&info, font->GetLineHeight());

	const char* word = text;

	int ascent, descent, lineGap;
	stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);

	ascent *= scale;
	descent *= scale;

	int x = 0;
	int y = 0;

	int bitmap_x = 0;
	int bitmap_y = ascent - descent;

	// Get size first
	for (int i = 0; i < strlen(word); ++i)
	{
		if (word[i] == 'y')
			continue;

		int c_x1, c_y1, c_x2, c_y2;
		stbtt_GetCodepointBitmapBox(&info, word[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

		y = ascent + c_y1;

		if (y > bitmap_y)
			bitmap_y = y;

		int ax;
		stbtt_GetCodepointHMetrics(&info, word[i], &ax, 0);
		x += ax * scale;

		int kern;
		kern = stbtt_GetCodepointKernAdvance(&info, word[i], word[i + 1]);
		x += kern * scale;
	}

	bitmap_x = x;

	x = 0;
	y = 0;

	unsigned char* bitmap = new unsigned char[bitmap_x * bitmap_y];

	for (int i = 0; i < strlen(word); ++i)
	{
		if (word[i] == 'y')
			continue;

		/* get bounding box for character (may be offset to account for chars that dip above or below the line */
		int c_x1, c_y1, c_x2, c_y2;
		stbtt_GetCodepointBitmapBox(&info, word[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

		/* compute y (different characters have different heights */
		y = ascent + c_y1;

		/* render character (stride and offset is important here) */
		int byteOffset = x + (y * bitmap_x);
		stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, bitmap_x, scale, scale, word[i]);

		/* how wide is this character */
		int ax;
		stbtt_GetCodepointHMetrics(&info, word[i], &ax, 0);
		x += ax * scale;

		/* add kerning */
		int kern;
		kern = stbtt_GetCodepointKernAdvance(&info, word[i], word[i + 1]);
		x += kern * scale;
	}

	uint id = LoadTexture(bitmap, font->GetBitmapSize(), bitmap_x, bitmap_y);

	RELEASE_ARRAY(bitmap)

	return SuStoTexture(id, SuStoVec2(bitmap_x, bitmap_y));

	//stbtt_fontinfo info = font->GetInfo();

	//unsigned char* bitmap = new unsigned char[font->GetWidth()* font->GetHeight()];

	//float scale = stbtt_ScaleForPixelHeight(&info, font->GetLineHeight());

	//const char* word = text;

	//int ascent, descent, lineGap;
	//stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);

	//ascent *= scale;
	//descent *= scale;

	//int x = 0;
	//int y = 0;

	//int bitmap_x = 0;
	//int bimtap_y = 0;

	//word = "dfasdf";

	//for (int i = 0; i < strlen(word); ++i)
	//{
	//	/* get bounding box for character (may be offset to account for chars that dip above or below the line */
	//	int c_x1, c_y1, c_x2, c_y2;
	//	stbtt_GetCodepointBitmapBox(&info, word[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

	//	/* compute y (different characters have different heights */
	//	y = ascent + c_y1;

	//	/* render character (stride and offset is important here) */
	//	int byteOffset = x + (y * font->GetWidth());
	//	stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, font->GetWidth(), scale, scale, word[i]);

	//	/* how wide is this character */
	//	int ax;
	//	stbtt_GetCodepointHMetrics(&info, word[i], &ax, 0);
	//	x += ax * scale;

	//	/* add kerning */
	//	int kern;
	//	kern = stbtt_GetCodepointKernAdvance(&info, word[i], word[i + 1]);
	//	x += kern * scale;
	//}

	//uint id = LoadTexture(bitmap, font->GetBitmapSize(), font->GetWidth()+1 , ascent);

	//RELEASE_ARRAY(bitmap)

	//return SuStoTexture(id, SuStoVec2(font->GetWidth(), ascent));
}

void UIFontsSystem::UnloadText(uint id)
{
	UnloadTexture(id);
}


UIFont::UIFont()
{
}

UIFont::UIFont(const char* _filepath, const char * _name, unsigned char * _bitmap, uint _bitmap_size, int _height, int _width, int _line_height, stbtt_fontinfo _info)
{
	name = _name;
	bitmap = _bitmap;
	bitmap_size = _bitmap_size;
	height = _height;
	width = _width;
	line_height = _line_height;
	info = _info;
	filepath = _filepath;
}

UIFont::~UIFont()
{
}

void UIFont::CleanUp()
{
	RELEASE_ARRAY(bitmap);
}

const char * UIFont::GetName()
{
	return name;
}

unsigned char * UIFont::GetBitmap()
{
	return bitmap;
}

uint UIFont::GetBitmapSize()
{
	return bitmap_size;
}

int UIFont::GetWidth()
{
	return width;
}

int UIFont::GetHeight()
{
	return height;
}

int UIFont::GetLineHeight()
{
	return line_height;
}

stbtt_fontinfo UIFont::GetInfo()
{
	return info;
}

const char * UIFont::GetFilePath()
{
	return filepath;
}
