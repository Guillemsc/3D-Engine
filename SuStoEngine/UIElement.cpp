#include "UIElement.h"

void UIElement::SetPos(Vec2 pos)
{
	rect.SetPos(pos);
}

Vec2 UIElement::GetPos()
{
	return Vec2(rect.x, rect.y);
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