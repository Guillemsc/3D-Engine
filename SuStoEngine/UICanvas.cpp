#include "UICanvas.h"

UICanvas::UICanvas(SuStoUIMain * main) : UIElement(main, ElementType::CANVAS, this)
{
	LOG_OUTPUT("Canvas Created");

	image = new PrintableElement(0, SuStoVec2(0, 0), SuStoVec2(0, 0), this);
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
	image->SetSize(SuStoVec2(GetUIMain()->GetViewport().x, GetUIMain()->GetViewport().y));
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
	float4x4 ortho_trans = image->GetTransform();

	ortho_trans[3][0] = GetUIMain()->GetViewport().x * 0.5f;
	ortho_trans[3][1] = GetUIMain()->GetViewport().y * 0.5f;

	return ortho_trans;
}

AABB UICanvas::GetBbox()
{
	return image->GetBbox();
}
