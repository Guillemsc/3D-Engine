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

SuStoUI::Vec2 SuStoUI::Vec2::Zero()
{
	return Vec2(0, 0);
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
	h = 0;
}

SuStoUI::Rect::Rect(float _x, float _y, float _w, float _h)
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
}

SuStoUI::Rect::Rect(const Rect & copy)
{
	x = copy.x;
	y = copy.y;
	w = copy.w;
	h = copy.h;
}

bool SuStoUI::Rect::operator==(const Rect & comp)
{
	if (x == comp.x && y == comp.y && w == comp.w && h == comp.h)
		return true;
	return false;
}

bool SuStoUI::Rect::PointInRect(const Vec2 & point)
{
	if (point.x >= x && point.x <= x + w && point.y >= y && point.y <= y + h)
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

std::string SuStoUI::ToUpperCase(std::string str)
{
	for (uint i = 0; i < str.size(); i++)
	{
		str[i] = toupper(str[i]);
	}

	return str;
}

std::string SuStoUI::ToLowerCase(std::string str)
{
	for (uint i = 0; i < str.size(); i++)
	{
		str[i] = tolower(str[i]);
	}

	return str;
}

bool SuStoUI::TextCmp(const char * text1, const char * text2)
{
	bool ret = false;

	if (text1 == nullptr || text2 == nullptr)
		return false;

	if (strcmp(text1, text2) == 0)
		ret = true;

	return ret;
}