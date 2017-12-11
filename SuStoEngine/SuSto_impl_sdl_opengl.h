#ifndef SUSTO_IMPL_STD_OPENGL
#define SUSTO_IMPL_STD_OPENGL

struct SDL_Window;

class SuStoUIMain;
struct SuStoVec2;
struct SuStoRect;

namespace SuStoUI
{
	void Init(SDL_Window* window, SuStoUIMain* ui_main);
	void NewFrame(SuStoVec2 viewport_size, SuStoUIMain* ui_main);
	void Render(SuStoUIMain* ui_main, SuStoRect viewport, bool ortographic = false);
	void EndFrame(SuStoUIMain* ui_main);
}

#endif // !SUSTO_IMPL_STD_OPENGL



