#include "UIElement.h"
#include "UICanvas.h"

ElementType UIElement::GetType()
{
	return type;
}

SuStoUIMain * UIElement::GetUIMain()
{
	return ui_main;
}

void UIElement::SetTransform(float4x4 _transform)
{
	transform = _transform;

	local_pos.x = GetAnchorPos()[0][3] - transform[0][3];
	local_pos.y = GetAnchorPos()[1][3] - transform[1][3];
}

float4x4 UIElement::GetTransform()
{
	SetTransform(transform);

	return transform;
}

float4x4 UIElement::GetOrthoTransform()
{
	SetTransform(transform);

	float4x4 new_trans = transform;

	new_trans[0][3] = GetOrthoAnchorPos().x + local_pos.x;
	new_trans[1][3] = GetOrthoAnchorPos().y + local_pos.y;

	return new_trans;
}

void UIElement::SetLocalPos(const SuStoVec2& _pos)
{
	local_pos = _pos;
}

SuStoVec2 UIElement::GetLocalPos() const
{
	return local_pos;
}

void UIElement::SetAnchor(const SuStoVec2 & _anchor)
{
	if (anchor.x > 1)
		anchor.x = 1;
	else if (anchor.x < 0)
		anchor.x = 0;

	if (anchor.y > 1)
		anchor.y = 1;
	else if (anchor.y < 0)
		anchor.y = 0;

	anchor = _anchor;
}

SuStoVec2 UIElement::GetAnchor()
{
	return anchor;
}

SuStoVec2 UIElement::GetOrthoAnchorPos()
{
	float anchor_pos_x = GetUIMain()->GetViewport().x * GetAnchor().x;
	float anchor_pos_y = GetUIMain()->GetViewport().y * GetAnchor().y;

	return SuStoVec2(anchor_pos_x, anchor_pos_y);
}

float4x4 UIElement::GetAnchorPos()
{
	float4x4 anchor_pos = transform;

	float4x4 start = float4x4::identity;
	start[0][3] = GetCanvasCenter()[0][3] - (GetUIMain()->GetViewport().x / 2);
	start[1][3] = GetCanvasCenter()[1][3] - (GetUIMain()->GetViewport().y / 2);

	anchor_pos[0][3] = start[0][3] + GetOrthoAnchorPos().x;
	anchor_pos[1][3] = start[1][3] + GetOrthoAnchorPos().y;

	return anchor_pos;
}

float4x4 UIElement::GetCanvasCenter()
{
	float4x4 center = GetCanvas()->GetTransform();

	return center;
}

UICanvas * UIElement::GetCanvas()
{
	return canvas;
}

UIElement::UIElement()
{
	type = ElementType::UNDEFINED;
}

UIElement::UIElement(SuStoUIMain * main, ElementType type, UICanvas* _canvas)
{
	ui_main = main;
	this->type = type;
	canvas = _canvas;
}

UIElement::~UIElement()
{
}