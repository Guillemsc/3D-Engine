#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include <list>
#include <string>
#include "GeometryMath.h"

class ImVec2;
class ImVec4;
// -----------------------------------------
// -----------------------------------------

// USEFULL FUNCTIONS //

// -----------------------------------------
// -----------------------------------------

// Returns the angle between two points in degrees
float AngleFromTwoPoints(float x1, float y1, float x2, float y2);

// Returns the distance from two points(can be a negative distance)
float DistanceFromTwoPoints(float x1, float y1, float x2, float y2);

// Compares two char*, true if success
bool TextCmp(const char* text1, const char* text2);

// Copies a string to another
void TextCpy(char * destination, const char * origen);

// Separate a string into tokens using the indicated iterator
void Tokenize(std::string string, const char separator, std::list<std::string>& tokens);

// Returns chars to upper case
std::string ToUpperCase(std::string str);

// Returns chars to lower case
std::string ToLowerCase(std::string str);

// Returns a random int given a range;
int GetRandomValue(int range_1, int range_2);

// Returns a random float given a range;
float GetRandomValue(float range_1, float range_2);

// Returns a random float given a range;
double GetRandomValue(double range_1, double range_2);

double GetUIDRandomDouble();

float GetUIDRandomFloat();

int GetUIDRandomInt();

std::string IntToHexa(int num);

std::string GetUIDRandomHexadecimal();

// Returns true if a point is inide a rect
bool PointInRect(float2 point_xy, float4 rect_xywh);

// Returns the file extension without the "."
std::string GetFileExtension(const char* file_name);

std::string GetFilenameWithoutExtension(const char* file_name, bool without_ = true);

// Returns only the file name from a file path like C:\\something\filename.exten
std::string GetFileNameFromFilePath(const char* file_path);

std::string GetPathFromFilePath(const char* file_path);

std::string ProcessFilePath(const char* file_path);

std::wstring utf8_to_utf16(const std::string& utf8);

#endif //__FUNCTIONS_H__