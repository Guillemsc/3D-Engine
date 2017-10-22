#ifndef __DEBUG_DRAW_H__
#define __DEBUG_DRAW_H__

#include "GeometryMath.h"

class DebugDraw
{
public:
	DebugDraw();

	void DrawCross(float3 center = { 0, 0, 0 }, float size = 1);
	void DrawAxis(float3 start = { 0, 0, 0 }, float size = 1);
	void DrawBox(float3 center = { 0, 0, 0 }, float3 size = { 1, 1, 1 }, float3 color = {255, 255, 255});
	void DrawFrustum(float3* corners, float3 color = { 255, 255, 255 });
	void DrawCone(float3 center = { 0, 0, 0 }, float radius = 1, float height = 1);
	void DrawArrow(float3 start = { 0, 0, 0 }, float lenght = 1, float radius = 0.3f, float3 color = { 255, 255, 255 });

private:
	void DrawLinesList(float3 * line_points, int size, int line_thickness = 3, float3* colors = nullptr);

public:

private:
};

#endif // !_DEBUG_DRAW__H