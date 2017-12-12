#include "UIElement.h"

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
	return transform;
}

void UIElement::SetPos(const SuStoVec2& _pos)
{
	pos = _pos;
}

SuStoVec2 UIElement::GetPos() const
{
	return pos;
}

void UIElement::SetSize(const SuStoVec2 & _size)
{
	size = _size;
}

SuStoVec2 UIElement::GetSize() const
{
	return size;
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