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
}

float4x4 UIElement::GetTransform()
{
	float4x4 anchor_trans = GetAnchorPos();

	anchor_trans = anchor_trans * float4x4::FromTRS(float3(local_pos.x, local_pos.y, 0), Quat::identity, float3(local_scale.x, local_scale.y, 0));

	transform = anchor_trans;

	SetTransform(transform);

	return transform;
}

float4x4 UIElement::GetOrthoTransform()
{
	SetTransform(transform);

	float4x4 new_trans = transform;

	new_trans[0][3] = GetOrthoAnchorPos().x + local_pos.x;
	new_trans[1][3] = GetOrthoAnchorPos().y - local_pos.y;

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

void UIElement::SetLocalScale(const SuStoVec2 & scale)
{
	local_scale = scale;
}

SuStoVec2 UIElement::GetLocalScale()
{
	return local_scale;
}

void UIElement::SetAnchor(const SuStoVec2 & _anchor)
{
	anchor = _anchor;

	if (anchor.x > 1)
		anchor.x = 1;
	else if (anchor.x < 0)
		anchor.x = 0;

	if (anchor.y > 1)
		anchor.y = 1;
	else if (anchor.y < 0)
		anchor.y = 0;
}

SuStoVec2 UIElement::GetAnchor()
{
	return anchor;
}

SuStoVec2 UIElement::GetOrthoAnchorPos()
{
	float anchor_pos_x = GetCanvas()->GetCurrentSize().x * GetAnchor().x;
	float anchor_pos_y = GetCanvas()->GetCurrentSize().y * -(GetAnchor().y - 1.0f);

	return SuStoVec2(anchor_pos_x, anchor_pos_y);
}

float4x4 UIElement::GetAnchorPos()
{
	float4x4 canvas_trans = GetCanvas()->GetTransform();

	canvas_trans[0][3] -= GetCanvas()->GetCurrentSize().x / 2;
	canvas_trans[1][3] -= GetCanvas()->GetCurrentSize().y / 2;

	canvas_trans[0][3] += GetCanvas()->GetCurrentSize().x * GetAnchor().x;
	canvas_trans[1][3] += GetCanvas()->GetCurrentSize().y * GetAnchor().y;

	//canvas_trans[0][3] -= GetUIMain()->GetViewport().x / 2;
	//canvas_trans[1][3] -= GetUIMain()->GetViewport().y / 2;

	//canvas_trans[0][3] += GetUIMain()->GetViewport().x * GetAnchor().x;
	//canvas_trans[1][3] += GetUIMain()->GetViewport().y * GetAnchor().y;

	return canvas_trans;
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

	local_pos = SuStoVec2(0, 0);
	local_scale = SuStoVec2(1, 1);
}

UIElement::~UIElement()
{
}