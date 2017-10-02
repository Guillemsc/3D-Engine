#include <ctype.h>
#include "Functions.h"
#include "App.h"
#include <stdio.h>
#include <cmath>
#include <random>

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
