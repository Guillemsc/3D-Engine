#ifndef __DEBUGDRAW_H__
#define __DEBUGDRAW_H__

#include "GeometryMath.h"
#include "Color.h"

void BeginDebugDraw();
void EndDebugDraw();

void DebugDraw(const Sphere& sphere, Color color = White, const float4x4& transform = float4x4::identity);
void DebugDraw(const AABB& aabb, Color color = White, bool lines = false, const float& line_size = 1.0f, const float4x4& transform = float4x4::identity);
void DebugDraw(const OBB& obb, Color color = White, bool lines = false, const float& line_size = 1.0f, const float4x4& transform = float4x4::identity);
void DebugDraw(const Capsule& capsule, Color color = White, const float4x4& transform = float4x4::identity);
void DebugDraw(const float4x4& transform);
void DebugDraw(const Frustum& frustum, Color color = White, bool lines = false, const float& line_size = 1.0f);
void DebugDraw(const LineSegment& segment, Color color = White, bool is_arrow = false, const float4x4& transform = float4x4::identity);
void DebugDraw(const Ray& ray, Color color = White, float max_dist = 5000.0f);
void DebugDraw(const float3& point, float size = 10.0f, Color color = White);
//void DebugDraw(const Cylinder& cylinder, Color color = White, const float4x4& transform = float4x4::identity);
//void DebugDraw(const Cone& cone, Color color = White, const float4x4& transform = float4x4::identity);
void DebugDrawBox(const float3* box_8_vertices, Color color = White, bool lines = false, const float& line_size = 1.0f);
void DebugDrawArrow(const float3 & dir, const float3 & offset, Color color, const float4x4 & transform = float4x4::identity);
void DebugDraw(const Circle& circle, Color color = White);
void DebugDraw(const Circle& pos, float inner_radius, Color color = White);
void DebugDrawArc(const float3& pos, float radius, float angle_left, float angle_right, float inner_radius = 0.0f, Color color = White, const float4x4& transform = float4x4::identity);

#endif // __DEBUGDRAW_H__