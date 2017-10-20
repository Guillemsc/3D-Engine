#include "DebugDraw.h"
#include "Glew\Include\glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

DebugDraw::DebugDraw()
{
}

void DebugDraw::DrawCross(float3 center, float size)
{
	const int s = 6;

	float3* lines = new float3[s];
	float3* colors = new float3[s];

	lines[0] = float3(center.x - (size / 2), center.y, center.z); colors[0] = float3(255, 0, 0);
	lines[1] = float3(center.x + (size / 2), center.y, center.z); colors[1] = float3(255, 0, 0);

	lines[2] = float3(center.x, center.y - size / 2, center.z); colors[2] = float3(0, 255, 0);
	lines[3] = float3(center.x, center.y + size / 2, center.z); colors[3] = float3(0, 255, 0);

	lines[4] = float3(center.x, center.y, center.z - size / 2); colors[4] = float3(0, 0, 255);
	lines[5] = float3(center.x, center.y, center.z + size / 2); colors[5] = float3(0, 0, 255);

	DrawLinesList(lines, s, 3, colors);

	delete[] lines;
	delete[] colors;
}

void DebugDraw::DrawBox(float3 center, float3 size, float3 color)
{
	float3* lines = new float3[6];

	lines[0] = float3(center.x - size.x / 2, center.y - size.y / 2, center.z - size.x / 2);
	lines[1] = float3(center.x - size.x / 2, center.y + size.y / 2, center.z - size.x / 2);


	////lines[0] = 0;
	////lines[1] = 0;
	////lines[2] = 0;

	////lines[3] = 0;
	////lines[4] = 100;
	////lines[5] = 0;

	//DrawLinesList(lines);

	delete[] lines;
}

void DebugDraw::DrawLinesList(float3 * line_points, int size, int line_thickness, float3* colors)
{
	glLineWidth((float)line_thickness);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, (float*)line_points->ptr());

	glEnableClientState(GL_COLOR_ARRAY);
	if(colors != nullptr && colors->Length() != 0)
		glColorPointer(3, GL_FLOAT, 0, (float*)colors->ptr());

	glDrawArrays(GL_LINES, 0, size);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glLineWidth(1);
}
