#ifndef SUSTO_IMPL_STD_OPENGL
#define SUSTO_IMPL_STD_OPENGL

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
	void Init(SDL_Window* window, SuStoUIMain* ui_main);
	void NewFrame(SuStoUIMain* ui_main, SDL_Window* window, SuStoVec2 viewport);
	void Render(SuStoUIMain* ui_main);
	void EndFrame(SuStoUIMain* ui_main);
	void Draw(float* vertices, uint num_indices, uint* indices, float* uvs, uint texture_id);

	void EventPreUpdate(SuStoUIMain * ui_main);
	void EventCleanUp(SuStoUIMain * ui_main);

	int CharToKey(const char* key);
	const char* KeyToChar(int key);

	const bool GetKeyDown(int id, SuStoUIMain * ui_main);
	const bool GetKeyRepeat(int id, SuStoUIMain * ui_main);
	const bool GetKeyUp(int id, SuStoUIMain * ui_main);
	const bool GetKeyDown(const char * key, SuStoUIMain * ui_main);
	const bool GetKeyRepeat(const char * key, SuStoUIMain * ui_main);
	const bool GetKeyUp(const char * key, SuStoUIMain * ui_main);
	const UIKeyEvent GetMouseButton(int id, SuStoUIMain * ui_main);
}

#endif // !SUSTO_IMPL_STD_OPENGL



