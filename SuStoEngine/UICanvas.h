#ifndef _UI_CANVAS_
#define _UI_CANVAS_

#include "UIElement.h"

enum UICanvasRenderMode
{
	WORLD_SPACE,
	CAMERA_SPACE,
};

class UICanvas : public UIElement
{
public:
	UICanvas(SuStoUIMain* ui_main);
	~UICanvas();

	void Awake();
	void Start();
	void Update();
	void CleanUp();

	void OnEvent(UIEvent ev);

	void SetSize(SuStoVec2 size);

	void SetScaleFactor(float set);
	float GetScaleFactor();
	void SetRenderMode(UICanvasRenderMode mode);
	UICanvasRenderMode GetRenderMode();

private:
	PrintableElement* image = nullptr;

	SuStoVec2 size;
	SuStoVec2 center;
	
	float scale_factor = 1.0f;

	UICanvasRenderMode render_mode = UICanvasRenderMode::WORLD_SPACE;

public:

};


#endif // !_UI_CANVAS_