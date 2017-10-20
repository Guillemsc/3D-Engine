#ifndef __DEBUG_DRAW_H__
#define __DEBUG_DRAW_H__

#include "GeometryMath.h"

class DebugDraw
{
public:
	DebugDraw();

	void DrawCross(float3 center, float size);
	void DrawBox(float3 center, float3 size, float3 color);

private:
	void DrawLinesList(float3 * line_points, int size, int line_thickness = 3, float3* colors = nullptr);

public:

private:
};

#endif // !_DEBUG_DRAW__H