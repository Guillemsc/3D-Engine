#include "SuStoUI.h"
#include "UIElement.h"
#include "UICanvas.h"

// Data types
SuStoUI::Vec2::Vec2()
{
	x = 0;
	y = 0;
}

SuStoUI::Vec2::Vec2(float _x, float _y)
{
	x = _x;
	y = _y;
}

SuStoUI::Vec2::Vec2(const Vec2 & copy)
{
	x = copy.x;
	y = copy.y;
}

bool SuStoUI::Vec2::operator==(const Vec2 & comp)
{
	if (x == comp.x && y == comp.y)
		return true;
	return false;
}

SuStoUI::Vec2 SuStoUI::Vec2::Zero()
{
	return Vec2(0, 0);
}

SuStoUI::Vec3::Vec3()
{
	x = 0;
	y = 0;
	z = 0;
}

SuStoUI::Vec3::Vec3(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

SuStoUI::Vec3::Vec3(const Vec3 & copy)
{
	x = copy.x;
	y = copy.y;
	z = copy.z;
}

bool SuStoUI::Vec3::operator==(const Vec3 & comp)
{
	if (x == comp.x && y == comp.y && z == comp.z)
		return true;
	return false;
}

SuStoUI::Rect::Rect()
{
	x = 0;
	y = 0;
	w = 0;
	h = 0;
}

SuStoUI::Rect::Rect(float _x, float _y, float _w, float _h)
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
}

SuStoUI::Rect::Rect(const Rect & copy)
{
	x = copy.x;
	y = copy.y;
	w = copy.w;
	h = copy.h;
}

bool SuStoUI::Rect::operator==(const Rect & comp)
{
	if (x == comp.x && y == comp.y && w == comp.w && h == comp.h)
		return true;
	return false;
}

bool SuStoUI::Rect::PointInRect(const Vec2 & point)
{
	if (point.x >= x && point.x <= x + w && point.y >= y && point.y <= y + h)
		return true;
	return false;
}

void SuStoUI::Rect::SetPos(const Vec2 & pos)
{
	x = pos.x;
	y = pos.y;
}

void SuStoUI::Rect::SetSize(const Vec2 & size)
{
	w = size.x;
	h = size.y;
}

SuStoUI::Color::Color()
{
	r = 0;
	g = 0;
	b = 0;
	a = 0;
}

SuStoUI::Color::Color(float _r, float _g, float _b, float _a)
{
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}

SuStoUI::Color::Color(const Color & copy)
{
	r = copy.r;
	g = copy.g;
	b = copy.b;
	a = copy.a;
}

bool SuStoUI::Color::operator==(const Color & comp)
{
	if (r == comp.r && g == comp.g && b == comp.b && a == comp.a)
		return true;
	return false;

}

std::string SuStoUI::ToUpperCase(std::string str)
{
	for (uint i = 0; i < str.size(); i++)
	{
		str[i] = toupper(str[i]);
	}

	return str;
}

std::string SuStoUI::ToLowerCase(std::string str)
{
	for (uint i = 0; i < str.size(); i++)
	{
		str[i] = tolower(str[i]);
	}

	return str;
}

bool SuStoUI::TextCmp(const char * text1, const char * text2)
{
	bool ret = false;

	if (text1 == nullptr || text2 == nullptr)
		return false;

	if (strcmp(text1, text2) == 0)
		ret = true;

	return ret;
}

SuStoUI::Plane::Plane()
{
}

SuStoUI::Plane::Plane(Vec2 size)
{
	int resX = 2; // 2 minimum
	int resZ = 2;

	//vertices
	uint num_vert = resX * resZ;
	Vec3 ver[4];
	for (int z = 0; z < resZ; z++)
	{
		// [ -length / 2, length / 2 ]
		float zPos = ((float)z / (resZ - 1) - .5f) * size.y;
		for (int x = 0; x < resX; x++)
		{
			// [ -width / 2, width / 2 ]
			float xPos = ((float)x / (resX - 1) - .5f) * size.x;
			ver[x + z * resX] = Vec3(xPos, 0.f, zPos);
		}
	}

	vertices = new float[num_vert * 3];
	for (int i = 0; i < num_vert; ++i)
	{
		memcpy(vertices + i * 3, &ver[i].x, sizeof(float) * 3);
	}

	//indices
	uint num_indices = (resX - 1) * (resZ - 1) * 6;
	uint ind[6];
	int t = 0;
	for (int face = 0; face < num_indices / 6; ++face)
	{
		int i = face % (resX - 1) + (face / (resZ - 1) * resX);

		ind[t++] = i + resX;
		ind[t++] = i + 1;
		ind[t++] = i;

		ind[t++] = i + resX;
		ind[t++] = i + resX + 1;
		ind[t++] = i + 1;
	}
	indices = new uint[num_indices];
	memcpy(indices, ind, sizeof(uint)*num_indices);

	//uv
	Vec3 uv[4];
	for (int v = 0; v < resZ; v++)
	{
		for (int u = 0; u < resX; u++)
		{
			uv[u + v * resX] = Vec3((float)u / (resX - 1), (float)v / (resZ - 1), 0.f);
		}
	}

	float* uvs = new float[num_vert * 3];
	for (int i = 0; i < num_vert; ++i)
	{
		memcpy(uvs + i * 3, &uv[i].x, sizeof(float) * 3);
	}
}

void SuStoUI::Plane::LoadToMem()
{
}

void SuStoUI::Plane::UnloadFromMem()
{
}

void SuStoUI::Plane::CleanUp()
{
	UnloadFromMem();

	// Vertices
	id_vertices = 0;
	num_vertices = 0;
	RELEASE_ARRAY(vertices);

	// Indices
	id_indices = 0;
	num_indices = 0;
	RELEASE_ARRAY(indices);

	// UVs
	id_uv = 0;
	num_uvs = 0;
	RELEASE_ARRAY(uvs);
}

SuStoUI::SuStoUIMain::SuStoUIMain()
{
}

SuStoUI::SuStoUIMain::~SuStoUIMain()
{
}

void SuStoUI::SuStoUIMain::Awake()
{
}

void SuStoUI::SuStoUIMain::Start()
{
}

void SuStoUI::SuStoUIMain::PreUpdate()
{
}

void SuStoUI::SuStoUIMain::Update()
{
}

void SuStoUI::SuStoUIMain::PostUpdate()
{
}

void SuStoUI::SuStoUIMain::CleanUp()
{
}

void SuStoUI::SuStoUIMain::PushEvent()
{
}

UIElement * SuStoUI::SuStoUIMain::CreateElement(ElementType type)
{
	UIElement* ret = nullptr;

	switch (type)
	{
	case SuStoUI::CANVAS:
		ret = new UICanvas(this);
		break;
	case SuStoUI::PANEL:
		break;
	case SuStoUI::IMAGE:
		break;
	case SuStoUI::TEXT:
		break;
	case SuStoUI::BUTTON:
		break;
	case SuStoUI::TEXT_INPUT:
		break;
	case SuStoUI::CHECKBOX:
		break;
	default:
		LOG_OUTPUT("Error while creating the element, type not found");
		break;
	}

	return ret;
}

void SuStoUI::SuStoUIMain::DeleteElement(UIElement * del)
{
}

void SuStoUI::SuStoUIMain::DestroyElements()
{
}