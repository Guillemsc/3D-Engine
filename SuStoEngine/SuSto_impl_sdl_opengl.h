#ifndef SUSTO_IMPL_STD_OPENGL
#define SUSTO_IMPL_STD_OPENGL

struct SDL_Window;

class SuStoUIMain;
//struct SuStoPlane;

namespace SuStoUI
{
	SuStoUIMain* Init(SDL_Window* window);
	void NewFrame(SDL_Window* window);
	void EndFrame();

	//SuStoUIMain* ui_main = nullptr;
	//SuStoPlane* test_pl;
}

#endif // !SUSTO_IMPL_STD_OPENGL



