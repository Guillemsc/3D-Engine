#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include <list>
#include <string>

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

bool PointInRect(ImVec2 point_xywh, ImVec4 rect_xywh);

#endif //__FUNCTIONS_H__