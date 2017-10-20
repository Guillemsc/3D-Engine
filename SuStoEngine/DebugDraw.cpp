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

	float half_size = size / 2;

	lines[0] = float3(center.x - half_size, center.y, center.z); colors[0] = float3(255, 0, 0);
	lines[1] = float3(center.x + half_size, center.y, center.z); colors[1] = float3(255, 0, 0);

	lines[2] = float3(center.x, center.y - half_size, center.z); colors[2] = float3(0, 255, 0);
	lines[3] = float3(center.x, center.y + half_size, center.z); colors[3] = float3(0, 255, 0);

	lines[4] = float3(center.x, center.y, center.z - half_size); colors[4] = float3(0, 0, 255);
	lines[5] = float3(center.x, center.y, center.z + half_size); colors[5] = float3(0, 0, 255);

	DrawLinesList(lines, s, 3, colors);

	delete[] lines;
	delete[] colors;
}

void DebugDraw::DrawBox(float3 center, float3 size, float3 color)
{
	const int s = 24;

	float3* lines = new float3[s];
	float3* colors = new float3[s];

	float3 half_size = size / 2;

	lines[0] = float3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z); 
	lines[1] = float3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z);

	lines[2] = float3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z);
	lines[3] = float3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z);

	lines[4] = float3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z);
	lines[5] = float3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z);

	lines[6] = float3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z);
	lines[7] = float3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z);

	lines[8] = float3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z);
	lines[9] = float3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z);

	lines[10] = float3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z);
	lines[11] = float3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z);

	lines[12] = float3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z);
	lines[13] = float3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z);

	lines[14] = float3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z);
	lines[15] = float3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z);

	lines[16] = float3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z);
	lines[17] = float3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z);

	lines[18] = float3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z);
	lines[19] = float3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z);

	lines[20] = float3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z);
	lines[21] = float3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z);

	lines[22] = float3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z);
	lines[23] = float3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z);

	for (int i = 0; i < s; i++)
	{
		colors[i] = color;
	}

	DrawLinesList(lines, s, 3, colors);

	delete[] lines;
	delete[] colors;
}

void DebugDraw::DrawLinesList(float3 * line_points, int size, int line_thickness, float3* colors)
{
	glLineWidth((float)line_thickness);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, (float*)line_points->ptr());

	if (colors != nullptr)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, (float*)colors->ptr());
	}

	glDrawArrays(GL_LINES, 0, size);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glLineWidth(1);
}
