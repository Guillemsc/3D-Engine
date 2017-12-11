#include "SuStoUI.h"
#include "UIElement.h"
#include "UICanvas.h"
#include "UIPanel.h"
#include "UIImage.h"
#include "UIText.h"
#include "UIButton.h"
#include "UITextInput.h"
#include "UICheckBox.h"

// Data types
SuStoVec2::SuStoVec2()
{
	x = 0;
	y = 0;
}

SuStoVec2::SuStoVec2(float _x, float _y)
{
	x = _x;
	y = _y;
}

SuStoVec2::SuStoVec2(const SuStoVec2 & copy)
{
	x = copy.x;
	y = copy.y;
}

bool SuStoVec2::operator==(const SuStoVec2 & comp)
{
	if (x == comp.x && y == comp.y)
		return true;
	return false;
}

SuStoVec2 SuStoVec2::Zero()
{
	return SuStoVec2(0, 0);
}

float * SuStoVec2::ptr()
{
	return &x;
}

SuStoVec3::SuStoVec3()
{
	x = 0;
	y = 0;
	z = 0;
}

SuStoVec3::SuStoVec3(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

SuStoVec3::SuStoVec3(const SuStoVec3 & copy)
{
	x = copy.x;
	y = copy.y;
	z = copy.z;
}

bool SuStoVec3::operator==(const SuStoVec3 & comp)
{
	if (x == comp.x && y == comp.y && z == comp.z)
		return true;
	return false;
}

SuStoVec3 SuStoVec3::Zero()
{
	return SuStoVec3(0, 0, 0);
}

float * SuStoVec3::ptr()
{
	return &x;
}

SuStoRect::SuStoRect()
{
	x = 0;
	y = 0;
	w = 0;
	h = 0;
	xy = xy.Zero();
	wh = wh.Zero();
}

SuStoRect::SuStoRect(float _x, float _y, float _w, float _h)
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
	xy = SuStoVec2(x, y);
	wh = SuStoVec2(w, h);
}

SuStoRect::SuStoRect(const SuStoRect & copy)
{
	x = copy.x;
	y = copy.y;
	w = copy.w;
	h = copy.h;
	xy = copy.xy;
	wh = copy.wh;
}

bool SuStoRect::operator==(const SuStoRect & comp)
{
	if (x == comp.x && y == comp.y && w == comp.w && h == comp.h)
		return true;
	return false;
}

bool SuStoRect::PointInRect(const SuStoVec2 & point)
{
	if (point.x >= x && point.x <= x + w && point.y >= y && point.y <= y + h)
		return true;
	return false;
}

void SuStoRect::SetPos(const SuStoVec2 & pos)
{
	x = pos.x;
	y = pos.y;
	xy = SuStoVec2(x, y);
}

void SuStoRect::SetSize(const SuStoVec2 & size)
{
	w = size.x;
	h = size.y;
	wh = SuStoVec2(w, h);
}

float * SuStoRect::ptr()
{
	return &x;
}

SuStoColor::SuStoColor()
{
	r = 0;
	g = 0;
	b = 0;
	a = 0;
}

SuStoColor::SuStoColor(float _r, float _g, float _b, float _a)
{
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}

SuStoColor::SuStoColor(const SuStoColor & copy)
{
	r = copy.r;
	g = copy.g;
	b = copy.b;
	a = copy.a;
}

bool SuStoColor::operator==(const SuStoColor & comp)
{
	if (r == comp.r && g == comp.g && b == comp.b && a == comp.a)
		return true;
	return false;

}

float * SuStoColor::ptr()
{
	return &r;
}

SuStoPlane::SuStoPlane(SuStoVec2 size)
{
	float length = size.y;
	float width = size.x;
	int resX = 2; // 2 minimum
	int resZ = 2;

	//vertices
	num_vertices = resX*resZ;
	SuStoVec3 ver[4];
	for (int z = 0; z < resZ; z++)
	{
		// [ -length / 2, length / 2 ]
		float zPos = ((float)z / (resZ - 1) - .5f) * length;
		for (int x = 0; x < resX; x++)
		{
			// [ -width / 2, width / 2 ]
			float xPos = ((float)x / (resX - 1) - .5f) * width;
			ver[x + z * resX] = SuStoVec3(xPos, zPos, 0.f);
		}
	}

	vertices = new float[num_vertices * 3];
	for (int i = 0; i < num_vertices; ++i)
	{
		memcpy(vertices + i * 3, &ver[i].x, sizeof(float) * 3);
	}

	//indices
	num_indices = (resX - 1) * (resZ - 1) * 6;
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
	memcpy(indices, ind, sizeof(uint) * num_indices);

	//uv
	SuStoVec3 uv[4];
	for (int v = 0; v < resZ; v++)
	{
		for (int u = 0; u < resX; u++)
		{
			uv[u + v * resX] = SuStoVec3((float)u / (resX - 1), (float)v / (resZ - 1), 0.f);
		}
	}

	uvs = new float[num_vertices * 3];
	num_uvs = num_vertices;
	for (int i = 0; i < num_vertices; ++i)
	{
		memcpy(uvs + i * 3, &uv[i].x, sizeof(float) * 3);
	}
}

uint SuStoPlane::GetVerticesId()
{
	return id_vertices;
}

uint SuStoPlane::GetIndicesId()
{
	return id_indices;
}

uint SuStoPlane::GetUvsId()
{
	return id_uvs;
}

void SuStoPlane::SetVerticesId(uint set)
{
	id_vertices = set;
}

void SuStoPlane::SetIndicesId(uint set)
{
	id_indices = set;
}

void SuStoPlane::SetUvsId(uint set)
{
	id_uvs = set;
}

uint SuStoPlane::GetNumVertices()
{
	return num_vertices;
}

uint SuStoPlane::GetNumIndices()
{
	return num_indices;
}

uint SuStoPlane::GetNumUvs()
{
	return num_uvs;
}

float * SuStoPlane::GetVertices()
{
	return vertices;
}

uint * SuStoPlane::GetIndices()
{
	return indices;
}

float * SuStoPlane::GetUvs()
{
	return uvs;
}

void SuStoPlane::CleanUp()
{
	// Vertices
	id_vertices = 0;
	num_vertices = 0;
	RELEASE_ARRAY(vertices);

	// Indices
	id_indices = 0;
	num_indices = 0;
	RELEASE_ARRAY(indices);

	// UVs
	id_uvs = 0;
	num_uvs = 0;
	RELEASE_ARRAY(uvs);
}

SuStoUIMain::SuStoUIMain()
{
}

SuStoUIMain::~SuStoUIMain()
{
}

void SuStoUIMain::Awake()
{
}

void SuStoUIMain::Start()
{
}

void SuStoUIMain::PreUpdate()
{
}

void SuStoUIMain::Update()
{
}

void SuStoUIMain::PostUpdate()
{
}

void SuStoUIMain::CleanUp()
{
}

void SuStoUIMain::PushEvent(UIEvent ev)
{
}

UIElement * SuStoUIMain::CreateElement(ElementType type)
{
	UIElement* ret = nullptr;

	switch (type)
	{
	case CANVAS:
		ret = new UICanvas(this);
		break;
	case PANEL:
		ret = new UIPanel(this);
		break;
	case IMAGE:
		ret = new UIImage(this);
		break;
	case TEXT:
		ret = new UIText(this);
		break;
	case BUTTON:
		ret = new UIButton(this);
		break;
	case TEXT_INPUT:
		ret = new UITextInput(this);
		break;
	case CHECKBOX:
		ret = new UICheckBox(this);
		break;
	default:
		LOG_OUTPUT("Error while creating the element, type not found");
		break;
	}

	return ret;
}

void SuStoUIMain::DeleteElement(UIElement * del)
{
	for (std::vector<UIElement*>::iterator it = elements.begin(); it != elements.end(); ++it)
	{
		if (*it == del) 
		{
			to_delete.push_back(*it);
			break;
		}
	}
}

void SuStoUIMain::SetViewport(SuStoVec2 view)
{
	viewport = view;
}

SuStoVec2 SuStoUIMain::GetViewport()
{
	return viewport;
}

void SuStoUIMain::Draw(SuStoPlane * plane)
{
	draw.push_back(plane);
}

std::vector<SuStoPlane*> SuStoUIMain::GetDrawList()
{
	return draw;
}

void SuStoUIMain::DestroyElements()
{
	for (std::list<UIElement*>::iterator it = to_delete.begin(); it != to_delete.end(); ++it)
	{
		RELEASE(*it);
		it = to_delete.erase(it);
	}
}

std::string ImGui::ToUpperCase(std::string str)
{
	for (uint i = 0; i < str.size(); i++)
	{
		str[i] = toupper(str[i]);
	}

	return str;
}

std::string ImGui::ToLowerCase(std::string str)
{
	for (uint i = 0; i < str.size(); i++)
	{
		str[i] = tolower(str[i]);
	}

	return str;
}

bool ImGui::TextCmp(const char * text1, const char * text2)
{
	bool ret = false;

	if (text1 == nullptr || text2 == nullptr)
		return false;

	if (strcmp(text1, text2) == 0)
		ret = true;

	return ret;
}
