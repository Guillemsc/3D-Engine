#include "UICanvas.h"

UICanvas::UICanvas(SuStoUIMain * main)
{
	LOG_OUTPUT("Canvas Created");

	ui_main = main;

	image = new PrintableElement(0, SuStoVec2(0, 0), SuStoVec2(1, 1), this);
	//main->DrawPrintable(image);

	SetWorldSize(SuStoVec2(1080, 720));
}

UICanvas::~UICanvas()
{
}

void UICanvas::Awake()
{
}

void UICanvas::Start()
{
}

void UICanvas::Update()
{
	SetCameraSize(SuStoVec2(GetUIMain()->GetViewport().x, GetUIMain()->GetViewport().y));

	image->SetSize(SuStoVec2(GetCurrentSize().x, GetCurrentSize().y));
}

void UICanvas::CleanUp()
{
	GetUIMain()->DestroyPrintable(image);
}

void UICanvas::OnEvent(UIEvent ev)
{
}

void UICanvas::SetScaleFactor(float set)
{
	scale_factor = set;
}

float UICanvas::GetScaleFactor()
{
	return scale_factor;
}

void UICanvas::SetRenderMode(UICanvasRenderMode mode)
{
	render_mode = mode;
}

UICanvasRenderMode UICanvas::GetRenderMode()
{
	return render_mode;
}

void UICanvas::SetShowOnCamera(bool set)
{
	show_on_camera = set;
}

bool UICanvas::GetShowOnCamera()
{
	return show_on_camera;
}

void UICanvas::SetTransform(float4x4 trans)
{
	transform = trans;
}

float4x4 UICanvas::GetTransform()
{
	return transform;
}

float4x4 UICanvas::GetOrthoTransform()
{
	float4x4 ortho_trans = transform;

	ortho_trans[0][3] = (GetUIMain()->GetViewport().x * 0.5f);
	ortho_trans[1][3] = (GetUIMain()->GetViewport().y * 0.5f);

	return ortho_trans;
}

PrintableElement * UICanvas::GetPlane()
{
	return image;
}

SuStoUIMain * UICanvas::GetUIMain()
{
	return ui_main;
}

void UICanvas::SetWorldSize(const SuStoVec2 & size)
{
	world_size = size;
}

void UICanvas::SetCameraSize(const SuStoVec2 & size)
{
	camera_size = size;
}

SuStoVec2 UICanvas::GetCurrentSize()
{
	SuStoVec2 ret;

	switch (render_mode)
	{
	case UICanvasRenderMode::WORLD_SPACE:
		ret = world_size;
		break;

	case UICanvasRenderMode::CAMERA_SPACE:
		ret = camera_size;
		break;
	}

	return ret;
}
