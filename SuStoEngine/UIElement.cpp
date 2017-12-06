#include "UIElement.h"

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