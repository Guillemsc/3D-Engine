#include "UIElement.h"

void UIElement::SetPos(const Vec2& pos)
{
	rect.SetPos(pos);
}

Vec2 UIElement::GetPos() const
{
	return rect.xy;
}

void UIElement::SetSize(const Vec2 & size)
{
	rect.SetSize(size);
}

Vec2 UIElement::GetSize() const
{
	return rect.wh;
}

void UIElement::SetRect(const Rect & _rect)
{
	rect.SetPos(_rect.xy);
	rect.SetSize(_rect.wh);
}

Rect UIElement::GetRect() const
{
	return rect;
}

UIElement::UIElement()
{
	type = ElementType::UNDEFINED;
}

UIElement::UIElement(SuStoUIMain * main, ElementType type)
{
	ui_main = main;
	this->type = type;
}

UIElement::~UIElement()
{
}