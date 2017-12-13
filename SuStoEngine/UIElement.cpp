#include "UIElement.h"

ElementType UIElement::GetType()
{
	return type;
}

SuStoUIMain * UIElement::GetUIMain()
{
	return ui_main;
}

float4x4 UIElement::GetTransform()
{
	float4x4 canvas_trans = canvas->g

	return transform;
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
	anchor = _anchor;
}

SuStoVec2 UIElement::GetAnchor()
{
	return anchor;
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