#include "UICanvas.h"

UICanvas::UICanvas(SuStoUIMain * main) : UIElement(main, ElementType::CANVAS, this)
{
	LOG_OUTPUT("Canvas Created");

	image = new PrintableElement(0, size, SuStoVec2(0, 0), this);
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
}

void UICanvas::CleanUp()
{
	GetUIMain()->DestroyPrintable(image);
}

void UICanvas::OnEvent(UIEvent ev)
{
}

void UICanvas::SetSize(SuStoVec2 _size)
{
	size = _size;
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
