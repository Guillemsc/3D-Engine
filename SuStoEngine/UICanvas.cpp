#include "UICanvas.h"

UICanvas::UICanvas(SuStoUIMain * main) : UIElement(main, ElementType::CANVAS, this)
{
	LOG_OUTPUT("Canvas Created");

	image = new PrintableElement(0, SuStoVec2(200, 200), SuStoVec2(0, 0), this);
	main->DrawPrintable(image);
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
	float center_x = GetUIMain()->GetViewport().x / 2;
	float center_y = GetUIMain()->GetViewport().y / 2;

	SetPos(SuStoVec2(center_x, center_y));
	image->SetPos(SuStoVec2(center_x, center_y));
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
