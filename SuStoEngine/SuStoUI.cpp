#include "SuStoUI.h"

// Data types
SuStoUI::Vec2::Vec2()
{
	x = 0;
	y = 0;
}

SuStoUI::Vec2::Vec2(float _x, float _y)
{
	x = _x;
	y = _y;
}

SuStoUI::Vec2::Vec2(const Vec2 & copy)
{
	x = copy.x;
	y = copy.y;
}

bool SuStoUI::Vec2::operator==(const Vec2 & comp)
{
	if (x == comp.x && y == comp.y)
		return true;
	return false;
}

SuStoUI::Vec3::Vec3()
{
	x = 0;
	y = 0;
	z = 0;
}

SuStoUI::Vec3::Vec3(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

SuStoUI::Vec3::Vec3(const Vec3 & copy)
{
	x = copy.x;
	y = copy.y;
	z = copy.z;
}

bool SuStoUI::Vec3::operator==(const Vec3 & comp)
{
	if (x == comp.x && y == comp.y && z == comp.z)
		return true;
	return false;
}

SuStoUI::Rect::Rect()
{
	x = 0;
	y = 0;
	w = 0;
	z = 0;
}

SuStoUI::Rect::Rect(float _x, float _y, float _w, float _z)
{
	x = _x;
	y = _y;
	w = _w;
	z = _z;
}

SuStoUI::Rect::Rect(const Rect & copy)
{
	x = copy.x;
	y = copy.y;
	w = copy.w;
	z = copy.z;
}

bool SuStoUI::Rect::operator==(const Rect & comp)
{
	if (x == comp.x && y == comp.y && w == comp.w && z == comp.z)
		return true;
	return false;
}

SuStoUI::Color::Color()
{
	r = 0;
	g = 0;
	b = 0;
	a = 0;
}

SuStoUI::Color::Color(float _r, float _g, float _b, float _a)
{
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}

SuStoUI::Color::Color(const Color & copy)
{
	r = copy.r;
	g = copy.g;
	b = copy.b;
	a = copy.a;
}

bool SuStoUI::Color::operator==(const Color & comp)
{
	if (r == comp.r && g == comp.g && b == comp.b && a == comp.a)
		return true;
	return false;
}