#ifndef SUSTO_IMPL_STD_OPENGL
#define SUSTO_IMPL_STD_OPENGL

struct SDL_Window;

class SuStoUIMain;
//struct SuStoPlane;

namespace SuStoUI
{
	void Init(SDL_Window* window, SuStoUIMain* ui_main);
	void NewFrame(SDL_Window* window, SuStoUIMain* ui_main);
	void Render(SuStoUIMain* ui_main);
	void EndFrame(SuStoUIMain* ui_main);
}

#endif // !SUSTO_IMPL_STD_OPENGL



