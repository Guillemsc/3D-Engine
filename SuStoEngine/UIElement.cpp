#include "UIElement.h"

void UIElement::SetTransform(float * _transform)
{
	transform = _transform;
}

float * UIElement::GetTransform()
{
	return transform;
}

ElementType UIElement::GetType()
{
	return type;
}

SuStoUIMain * UIElement::GetUIMain()
{
	return ui_main;
}

void UIElement::SetPos(const SuStoVec2& pos)
{
	rect.SetPos(pos);
}

SuStoVec2 UIElement::GetPos() const
{
	return rect.xy;
}

void UIElement::SetSize(const SuStoVec2 & size)
{
	rect.SetSize(size);
}

SuStoVec2 UIElement::GetSize() const
{
	return rect.wh;
}

void UIElement::SetRect(const SuStoRect & _rect)
{
	rect.SetPos(_rect.xy);
	rect.SetSize(_rect.wh);
}

SuStoRect UIElement::GetRect() const
{
	return rect;
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