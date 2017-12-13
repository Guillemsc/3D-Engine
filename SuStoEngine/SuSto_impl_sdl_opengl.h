#ifndef SUSTO_IMPL_STD_OPENGL
#define SUSTO_IMPL_STD_OPENGL

struct SDL_Window;

class SuStoUIMain;
class UIEvent;

struct SuStoVec2;
struct SuStoRect;

#define SCREEN_SIZE 1

namespace SuStoUI
{
	void Init(SDL_Window* window, SuStoUIMain* ui_main);
	void NewFrame(SuStoUIMain* ui_main, SDL_Window* window, SuStoVec2 viewport);
	void Render(SuStoUIMain* ui_main);
	void EndFrame(SuStoUIMain* ui_main);
	void Draw(float* vertices, uint num_indices, uint* indices, float* uvs, uint texture_id);

	UIEvent* event_system = nullptr;
	void EventPreUpdate();
	void EventCleanUp();
	int CharToKey(const char* key);
	const char* KeyToChar(int key);

}

#endif // !SUSTO_IMPL_STD_OPENGL



