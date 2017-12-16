#define STB_TRUETYPE_IMPLEMENTATION
#include "SuSto_fonts_system.h"
#include "SuSto_impl_sdl_opengl.h"

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

void UIFontsSystem::LoadFont(const char * filepath)
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

	unsigned char* bitmap = new unsigned char[b_w * b_h];

	float scale = stbtt_ScaleForPixelHeight(&info, l_h);

	char* word = "how are you?";

	int ascent, descent, lineGap;
	stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);

	ascent *= scale;
	descent *= scale;

	int x = 0;
	for (int i = 0; i < strlen(word); ++i)
	{
		/* get bounding box for character (may be offset to account for chars that dip above or below the line */
		int c_x1, c_y1, c_x2, c_y2;
		stbtt_GetCodepointBitmapBox(&info, word[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

		/* compute y (different characters have different heights */
		int y = ascent + c_y1;

		/* render character (stride and offset is important here) */
		int byteOffset = x + (y  * b_w);
		stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, b_w, scale, scale, word[i]);

		/* how wide is this character */
		int ax;
		stbtt_GetCodepointHMetrics(&info, word[i], &ax, 0);
		x += ax * scale;

		/* add kerning */
		int kern;
		kern = stbtt_GetCodepointKernAdvance(&info, word[i], word[i + 1]);
		x += kern * scale;
	}

	//SuStoUI::LoadTexture(bitmap, size, b_w, b_h);
	uint id = LoadTexture(bitmap, size, b_w, b_h);
	SuStoVec2 text_size = SuStoVec2(b_w, b_h);


}

UIFont::UIFont()
{
}

UIFont::~UIFont()
{
}
