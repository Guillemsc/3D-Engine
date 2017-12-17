#ifndef SUSTO_IMPL_STD_OPENGL
#define SUSTO_IMPL_STD_OPENGL

#include "SuStoUI.h"
#include "GeometryMath.h"

struct SDL_Window;

class SuStoUIMain;
class UIEvent;

struct SuStoVec2;
struct SuStoRect;

enum UIKeyEvent;

typedef unsigned int uint;

#define SCREEN_SIZE 1

namespace SuStoUI
{
	SuStoUIMain* Init(SDL_Window* window);
	void NewFrame(SuStoUIMain* ui_main, SDL_Window* window, SuStoRect viewport, Frustum frustum);
	void Render(SuStoUIMain* ui_main);
	void EndFrame(SuStoUIMain* ui_main);
	void Draw(float* vertices, uint num_indices, uint* indices, float* uvs, uint texture_id);
	void DeInit(SuStoUIMain* ui_main);

	void EventNewFrame(SuStoUIMain * ui_main);

	int CharToKey(const char* key);
	const char* KeyToChar(int key);
	uint LoadTexture(unsigned char * buffer, uint buffer_size, uint width, uint height);
	void UnLoadTexture(uint id);

}

#endif // !SUSTO_IMPL_STD_OPENGL



