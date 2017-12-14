#ifndef _UI_CANVAS_
#define _UI_CANVAS_

#include "UIElement.h"

enum UICanvasRenderMode
{
	WORLD_SPACE,
	CAMERA_SPACE,
};

class UICanvas
{
public:
	UICanvas(SuStoUIMain* ui_main);
	~UICanvas();

	void Awake();
	void Start();
	void Update();
	void CleanUp();

	void OnEvent(UIEvent ev);

	void SetScaleFactor(float set);
	float GetScaleFactor();

	void SetRenderMode(UICanvasRenderMode mode);
	UICanvasRenderMode GetRenderMode();

	void SetShowOnCamera(bool set);
	bool GetShowOnCamera();

	void SetTransform(float4x4 trans);
	float4x4 GetTransform();
	float4x4 GetOrthoTransform();

	PrintableElement* GetPlane();

	SuStoUIMain* GetUIMain();

	void SetWorldSize(const SuStoVec2& size);
	void SetCameraSize(const SuStoVec2& size);
	SuStoVec2 GetCurrentSize();

private:
	PrintableElement* image = nullptr;
	
	float scale_factor = 1.0f;
	UICanvasRenderMode render_mode = UICanvasRenderMode::WORLD_SPACE;
	bool show_on_camera = false;

	float4x4 transform = float4x4::identity;;

	SuStoUIMain* ui_main = nullptr;

	SuStoVec2 world_size;
	SuStoVec2 camera_size;

public:

};


#endif // !_UI_CANVAS_