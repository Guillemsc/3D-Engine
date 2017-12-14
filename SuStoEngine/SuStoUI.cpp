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

SuStoPlane::SuStoPlane()
{
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

	bbox.SetNegativeInfinity();
	bbox.Enclose((vec*)vertices, num_vertices);
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

AABB SuStoPlane::GetBbox()
{
	return bbox;
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
	event_system = new EventSystem();
}

void SuStoUIMain::Start()
{
}

void SuStoUIMain::PreUpdate()
{
}

void SuStoUIMain::Update()
{
	if (SuStoUI::GetMouseButton(1, this) == 1)
	{
		MousePick();
	}

	for (std::vector<UICanvas*>::iterator it = canvas.begin(); it != canvas.end(); ++it)
	{
		(*it)->Update();
	}

	for (std::vector<UIElement*>::iterator it = elements.begin(); it != elements.end(); ++it)
	{
		(*it)->PreUpdate();
		(*it)->Update();
		(*it)->PostUpdate();
	}
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

UICanvas * SuStoUIMain::CreateCanvas()
{
	UICanvas* cv = nullptr;
	cv = new UICanvas(this);

	canvas.push_back(cv);

	return cv;
}

UIElement * SuStoUIMain::CreateElement(ElementType type, UICanvas* canvas)
{
	UIElement* ret = nullptr;

	switch (type)
	{
	case PANEL:
		ret = new UIPanel(this, canvas);
		break;
	case IMAGE:
		ret = new UIImage(this, canvas);
		break;
	case TEXT:
		ret = new UIText(this, canvas);
		break;
	case BUTTON:
		ret = new UIButton(this, canvas);
		break;
	case TEXT_INPUT:
		ret = new UITextInput(this, canvas);
		break;
	case CHECKBOX:
		ret = new UICheckBox(this, canvas);
		break;
	default:
		LOG_OUTPUT("Error while creating the element, type not found");
		break;
	}

	elements.push_back(ret);

	return ret;
}

void SuStoUIMain::DeleteElement(UIElement * del)
{
	if (del == nullptr)
		return;

	if (del->GetType() == ElementType::CANVAS)
	{
		DeleteCanvas((UICanvas*)del);
		return;
	}

	for (std::list<UIElement*>::iterator it = to_delete.begin(); it != to_delete.end(); ++it)
	{
		if (*it == del) 
			return;
	}

	for (std::vector<UIElement*>::iterator it = elements.begin(); it != elements.end(); ++it)
	{
		if ((*it) == del)
		{
			to_delete.push_back(del);
			break;
		}
	}
}

void SuStoUIMain::DeleteCanvas(UICanvas * can)
{
	if (can == nullptr)
		return;

	for (std::vector<UIElement*>::iterator it = elements.begin(); it != elements.end(); ++it)
	{
		if ((*it)->GetCanvas() == can)
		{
			DeleteElement(*it);
		}
	}

	for (std::vector<UICanvas*>::iterator cv = canvas.begin(); cv != canvas.end(); ++cv)
	{
		if ((*cv) == can)
		{
			RELEASE(*cv);
			canvas.erase(cv);
			break;
		}
	}
}

void SuStoUIMain::SetViewport(SuStoVec2 view)
{
	viewport = view;
}

void SuStoUIMain::SetWindowViewport(SuStoVec2 view)
{
	window_viewport = view;
}

SuStoVec2 SuStoUIMain::GetViewport()
{
	return viewport;
}

SuStoVec2 SuStoUIMain::GetWindowViewport()
{
	return window_viewport;
}

void SuStoUIMain::SetFrustum(const Frustum & frustum_)
{
	frustum = frustum_;
}

Frustum SuStoUIMain::GetFrustum() const
{
	return frustum;
}

std::vector<UIElement*> SuStoUIMain::GetElementList()
{
	return elements;
}

std::vector<UICanvas*> SuStoUIMain::GetCanvasList()
{
	return canvas;
}

void SuStoUIMain::DrawPrintable(PrintableElement * plane)
{
	draw.push_back(plane);
}

void SuStoUIMain::DestroyPrintable(PrintableElement * element)
{
	for (std::vector<PrintableElement*>::iterator it = draw.begin(); it != draw.end(); ++it)
	{
		if ((*it) == element)
		{
			(*it)->CleanUp();
			RELEASE(*it);
			draw.erase(it);
			break;
		}
	}
}

std::vector<PrintableElement*> SuStoUIMain::GetDrawList()
{
	return draw;
}

void SuStoUIMain::SetUIMode(UIMode _mode)
{
	mode = _mode;
}

UIMode SuStoUIMain::GetUIMode()
{
	return mode;
}

void SuStoUIMain::DestroyElements()
{
	for (std::list<UIElement*>::iterator it = to_delete.begin(); it != to_delete.end(); ++it)
	{
		RELEASE(*it);
		it = to_delete.erase(it);
	}
}

void SuStoUIMain::MousePick()
{
	SuStoRect window(0, 0, GetViewport().x, GetViewport().y);
	SuStoVec2 mouse_pos(event_system->GetMouseX(), event_system->GetMouseY());

	if (window.PointInRect(mouse_pos))
	{
		//The point (1, 1) corresponds to the top-right corner of the near plane
		//(-1, -1) is bottom-left

		float first_normalized_x = (mouse_pos.x - window.x) / (window.w - window.x);
		float first_normalized_y = (mouse_pos.y - window.y) / (window.h - window.y);

		float normalized_x = (first_normalized_x * 2) - 1;
		float normalized_y = 1 - (first_normalized_y * 2);

		LineSegment picking = GetFrustum().UnProjectLineSegment(normalized_x, normalized_y);

		float distance = 99999999999;
		PrintableElement* closest = nullptr;

		std::vector<PrintableElement*> elements_pick = GetDrawList();

		for (std::vector<PrintableElement*>::iterator it = elements_pick.begin(); it != elements_pick.end(); it++)
		{
			bool hit;
			float dist;
			(*it)->TestRay(picking, hit);

			if (hit)
			{
				if (dist < distance)
				{
					distance = dist;
					closest = (*it);
				}
			}
		}
	}
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

PrintableElement::PrintableElement(uint _texture_id, SuStoVec2 _pos, SuStoVec2 _texture_size, UIElement* _owner)
{
	owner = _owner;

	texture = SuStoTexture(_texture_id, _texture_size);
	local_pos = _pos;

	plane = SuStoPlane(SuStoVec2(SuStoVec2(1, 1)));

	SetPos(local_pos);
}

PrintableElement::PrintableElement(uint _texture_id, SuStoVec2 _pos, SuStoVec2 _texture_size, UICanvas * _owner)
{
	owner_c = _owner;

	texture = SuStoTexture(_texture_id, _texture_size);
	local_pos = _pos;

	plane = SuStoPlane(SuStoVec2(1, 1));

	SetPos(local_pos);
}

void PrintableElement::SetTexture(uint _texture_id, SuStoVec2 _texture_size)
{
	texture.id = _texture_id;
	texture.size = _texture_size;
}

uint PrintableElement::GetNumVertices()
{
	return plane.GetNumVertices();
}

uint PrintableElement::GetNumIndices()
{
	return plane.GetNumIndices();
}

uint PrintableElement::GetNumUvs()
{
	return plane.GetNumUvs();
}

float * PrintableElement::GetVertices()
{
	return plane.GetVertices();
}

uint * PrintableElement::GetIndices()
{
	return plane.GetIndices();
}

float * PrintableElement::GetUvs()
{
	return plane.GetUvs();
}

uint PrintableElement::GetTextureId()
{
	return texture.id;
}

void PrintableElement::SetPos(SuStoVec2 pos)
{
	local_pos = pos;
}

SuStoVec2 PrintableElement::GetPos()
{
	return local_pos;
}

SuStoVec2 PrintableElement::GetOrthoPos()
{
	SuStoVec2 ret;

	if (HasOwner())
	{
		ret.x = owner->GetOrthoTransform()[0][3] + local_pos.x;
		ret.y = owner->GetOrthoTransform()[1][3] + local_pos.y;
	}
	
	return ret;
}

void PrintableElement::SetSize(SuStoVec2 size)
{
	texture.size = size;
}

SuStoVec2 PrintableElement::GetSize()
{
	return texture.size;
}

float4x4 PrintableElement::GetTransform()
{
	float4x4 o_trans = float4x4::identity;

	if (owner != nullptr)
	{
		o_trans = owner->GetTransform();
	}
	else
	{
		o_trans = owner_c->GetTransform();
	}

	o_trans[0][3] += local_pos.x;
	o_trans[1][3] += local_pos.y;

	o_trans = o_trans * float4x4::Scale(float3(texture.size.x, texture.size.y, 0));

	transform = o_trans;

	return transform;
}

float4x4 PrintableElement::GetOrtoTransform()
{
	float4x4 o_trans = float4x4::identity;

	if (owner != nullptr)
	{
		o_trans = owner->GetOrthoTransform();
	}
	else
	{
		o_trans = owner_c->GetOrthoTransform();
	}

	o_trans[0][3] += local_pos.x;
	o_trans[1][3] += local_pos.y;

	o_trans = o_trans * float4x4::Scale(float3(texture.size.x, texture.size.y, 0));

	return o_trans;
}

UICanvasRenderMode PrintableElement::GetRenderMode()
{
	UICanvasRenderMode ret = UICanvasRenderMode::CAMERA_SPACE;

	if (owner != nullptr)
	{
		ret = owner->GetCanvas()->GetRenderMode();
	}
	else
	{
		ret = owner_c->GetRenderMode();
	}

	return ret;
}

bool PrintableElement::GetShowOnCamera()
{
	bool ret = false;

	if (owner != nullptr)
	{
		ret = owner->GetCanvas()->GetShowOnCamera();
	}
	else
	{
		ret = owner_c->GetShowOnCamera();
	}

	return ret;
}

UIElement * PrintableElement::GetOwner() const
{
	return owner;
}

bool PrintableElement::HasOwner()
{
	return (owner != nullptr) ? true : false;
}

AABB PrintableElement::GetBbox()
{
	return plane.GetBbox();
}

void PrintableElement::CleanUp()
{
	plane.CleanUp();
}

void PrintableElement::TestRay(const LineSegment& segment, bool& hit)
{
	hit = false;

	// Check if intersects with bbox
	if (GetBbox().IsFinite())
	{
		if (segment.Intersects(GetBbox()))
		{		
			hit = true;
		}
	}
}

bool PrintableElement::CheckPoint(SuStoVec2 pos)
{
	SuStoVec2 ortho_pos = GetOrthoPos();
	SuStoVec2 half_size = SuStoVec2(texture.size.x / 2, texture.size.y / 2);

	if (pos.x > ortho_pos.x - half_size.x && pos.x < ortho_pos.x + half_size.x)
	{
		if (pos.y > ortho_pos.y - half_size.y && pos.y < ortho_pos.y + half_size.y)
		{
			return true;
		}
	}

	return false;
}

SuStoTexture::SuStoTexture()
{
}

SuStoTexture::SuStoTexture(uint _texture_id, SuStoVec2 _texture_size)
{
	id = _texture_id;
	size = _texture_size;
}
