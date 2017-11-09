#include <ctype.h>
#include "Functions.h"
#include "App.h"
#include <stdio.h>
#include <cmath>
#include <random>
#include <limits>
#include "imgui.h"
#include <stdlib.h>

// Returns the angle between two points in degrees
float AngleFromTwoPoints(float x1, float y1, float x2, float y2)
{
	float deltaY = y2 - y1;
	float deltaX = x2 - x1;

	return (atan2(deltaY, deltaX) * RADTODEG);
}

float DistanceFromTwoPoints(float x1, float y1, float x2, float y2)
{
	int distance_x = x2 - x1;
	int distance_y = y2 - y1;
	float sign = ((distance_x * distance_x) + (distance_y * distance_y));
	float dist = abs((distance_x * distance_x) + (distance_y * distance_y));

	if (sign > 0)
		return sqrt(dist);
	else
		return -sqrt(dist);
}

bool TextCmp(const char * text1, const char * text2)
{
	bool ret = false;

	if (text1 == nullptr || text2 == nullptr)
		return false;

	if (strcmp(text1, text2) == 0)
		ret = true;

	return ret;
}

void TextCpy(char* destination, const char * origen)
{
	if (origen != nullptr)
	{
		strcpy_s(destination, strlen(origen), origen);
	}
}

void Tokenize(std::string string, const char separator, std::list<std::string>& tokens)
{
	uint i = 0;
	const char* str = string.c_str();
	while (*(str + i) != 0)
	{
		std::string temporal;
		while (*(str + i) != separator && *(str + i) && *(str + i) != '\n')
		{
			temporal.push_back(*(str + i));
			i++;
		}
		tokens.push_back(temporal);
		if (*(str + i)) i++;
	}
}

string ToUpperCase(string str)
{
	for (uint i = 0; i < str.size(); i++)
	{
		str[i] = toupper(str[i]);
	}

	return str;
}

string ToLowerCase(std::string str)
{
	for (uint i = 0; i < str.size(); i++) 
	{
		str[i] = tolower(str[i]);
	}

	return str;
}

int GetRandomValue(int range_1, int range_2)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(range_1, range_2);

	return dis(gen);
}

float GetRandomValue(float range_1, float range_2)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<> dis(range_1, range_2);

	return dis(gen);
}

double GetRandomValue(double range_1, double range_2)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<> dis(range_1, range_2);

	return dis(gen);
}

double GetUIDRandomDouble()
{
	return GetRandomValue(DBL_MIN, DBL_MAX);
}

float GetUIDRandomFloat()
{
	return GetRandomValue(FLT_MIN, FLT_MAX);
}

int GetUIDRandomInt()
{
	return GetRandomValue(INT_MIN, INT_MAX);
}

std::string GetUIDRandomHexadecimal()
{
	char buf[17];
	snprintf(buf, sizeof(buf), "%016llx", GetUIDRandomInt());
	buf[16] = 0;
	return buf;
}

bool PointInRect(float2 point_xy, float4 rect_xywh)
{
	if (point_xy.x > rect_xywh.x && point_xy.x < rect_xywh.x + rect_xywh.w && point_xy.y > rect_xywh.y && point_xy.y < rect_xywh.y + rect_xywh.z)
		return true;

	return false;
}

string GetFileExtension(const char * file_name)
{
	string ret;

	bool adding = false;
	for (int i = 0; file_name[i] != '\0'; i++)
	{
		if (file_name[i] == '.')
		{
			ret.clear();
			adding = true;
			continue;
		}

		if (adding)
			ret += file_name[i];
	}

	return ret;
}

std::string GetFilenameWithoutExtension(const char * file_name, bool without_)
{
	string ret;

	for (int i = 0; file_name[i] != '\0'; i++)
	{
		if (file_name[i] == '.' || (without_ && file_name[i] == '_'))
		{
			break;
		}

		ret += file_name[i];
	}

	return ret;
}

std::string GetFileNameFromFilePath(const char * file_path)
{
	string ret;

	for (int i = 0; file_path[i] != '\0'; i++)
	{
		if (file_path[i] == '\\' || file_path[i] == '/')
		{
			ret.clear();
			continue;
		}

		ret += file_path[i];
	}

	return ret;
}

std::string GetPathFromFilePath(const char * file_path)
{
	string ret;

	int last = 0;
	for (int i = 0; file_path[i] != '\0'; i++)
	{
		if (file_path[i] == '\\' || file_path[i] == '/')
		{
			last = i;
			last++;
		}
	}

	for (int i = 0; i < last && file_path[i] != '\0'; i++)
	{
		ret += file_path[i];
	}

	return ret;
}

std::string ProcessFilePath(const char * file_path)
{
	string ret;

	for (int i = 0; file_path[i] != '\0'; i++)
	{
		if (file_path[i] == '\\')
		{
			ret += '/';
			continue;
		}

		ret += file_path[i];
	}

	return ret;
}

std::wstring utf8_to_utf16(const std::string& utf8)
{
	std::vector<unsigned long> unicode;
	size_t i = 0;
	while (i < utf8.size())
	{
		unsigned long uni;
		size_t todo;
		bool error = false;
		unsigned char ch = utf8[i++];
		if (ch <= 0x7F)
		{
			uni = ch;
			todo = 0;
		}
		else if (ch <= 0xBF)
		{
			throw std::logic_error("not a UTF-8 string");
		}
		else if (ch <= 0xDF)
		{
			uni = ch & 0x1F;
			todo = 1;
		}
		else if (ch <= 0xEF)
		{
			uni = ch & 0x0F;
			todo = 2;
		}
		else if (ch <= 0xF7)
		{
			uni = ch & 0x07;
			todo = 3;
		}
		else
		{
			throw std::logic_error("not a UTF-8 string");
		}
		for (size_t j = 0; j < todo; ++j)
		{
			if (i == utf8.size())
				throw std::logic_error("not a UTF-8 string");
			unsigned char ch = utf8[i++];
			if (ch < 0x80 || ch > 0xBF)
				throw std::logic_error("not a UTF-8 string");
			uni <<= 6;
			uni += ch & 0x3F;
		}
		if (uni >= 0xD800 && uni <= 0xDFFF)
			throw std::logic_error("not a UTF-8 string");
		if (uni > 0x10FFFF)
			throw std::logic_error("not a UTF-8 string");
		unicode.push_back(uni);
	}
	std::wstring utf16;
	for (size_t i = 0; i < unicode.size(); ++i)
	{
		unsigned long uni = unicode[i];
		if (uni <= 0xFFFF)
		{
			utf16 += (wchar_t)uni;
		}
		else
		{
			uni -= 0x10000;
			utf16 += (wchar_t)((uni >> 10) + 0xD800);
			utf16 += (wchar_t)((uni & 0x3FF) + 0xDC00);
		}
	}
	return utf16;
}
