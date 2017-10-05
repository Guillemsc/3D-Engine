
#include "Globals.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Primitive.h"

// ------------------------------------------------------------
Primitive::Primitive(PrimitiveTypes type) : type(type)
{}

// ------------------------------------------------------------
PrimitiveTypes Primitive::GetType() const
{
	return type;
}

// ------------------------------------------------------------
//void Primitive::SetPos(float x, float y, float z)
//{
//	transform.translate(x, y, z);
//}

// ------------------------------------------------------------
//void Primitive::SetRotation(float angle, const vec3 &u)
//{
//	transform.rotate(angle, u);
//}

// ------------------------------------------------------------
//void Primitive::Scale(float x, float y, float z)
//{
//	transform.scale(x, y, z);
//}

// CUBE ============================================
PCube::PCube() : Primitive(PrimitiveTypes::Primitive_Cube), size(1.0f, 1.0f, 1.0f)
{

}

PCube::PCube(float sizeX, float sizeY, float sizeZ) : Primitive(PrimitiveTypes::Primitive_Cube), size(sizeX, sizeY, sizeZ)
{

}

void PCube::Render()
{
}

// SPHERE ============================================
PSphere::PSphere() : Primitive(PrimitiveTypes::Primitive_Sphere), radius(1.0f)
{
	
}

PSphere::PSphere(float radius) : Primitive(PrimitiveTypes::Primitive_Sphere), radius(radius)
{

}

void PSphere::Render()
{
}


// CYLINDER ============================================
PCylinder::PCylinder() : Primitive(PrimitiveTypes::Primitive_Cylinder), radius(1.0f), height(1.0f)
{

}

PCylinder::PCylinder(float radius, float height) : Primitive(PrimitiveTypes::Primitive_Cylinder), radius(radius), height(height)
{

}

void PCylinder::Render()
{
}

// LINE ==================================================
PLine::PLine() : Primitive(PrimitiveTypes::Primitive_Line), origin(0, 0, 0), destination(1, 1, 1)
{

}

PLine::PLine(float x, float y, float z) : Primitive(PrimitiveTypes::Primitive_Line), origin(0, 0, 0), destination(x, y, z)
{

}

void PLine::Render()
{
}

// PLANE ==================================================
PPlane::PPlane() : Primitive(PrimitiveTypes::Primitive_Plane), normal(0, 1, 0), constant(1)
{
	
}

PPlane::PPlane(float x, float y, float z, float d) : Primitive(PrimitiveTypes::Primitive_Plane), normal(x, y, z), constant(d)
{

}

void PPlane::Render()
{
}
