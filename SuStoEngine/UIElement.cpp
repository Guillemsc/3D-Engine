#include "UIElement.h"

void UIElement::SetPos(Vec2 pos)
{
	rect.SetPos(pos);
}

Vec2 UIElement::GetPos()
{
	return Vec2(rect.x, rect.y);
}
