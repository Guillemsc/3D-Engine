#ifndef SUSTO_UI
#define SUSTO_UI

#include <vector>
#include <list>
#include <string>

#include "SuSto_event_system.h"


class UIElement;
class UICanvas;

// -------------------------------
// GLOBALS -----------------------
// -------------------------------
typedef unsigned int uint;
typedef unsigned char byte;
typedef wchar_t wchar;

// Deletes a buffer
#define RELEASE( x )		\
    {                       \
    if( x != nullptr )      \
	    {                   \
      delete x;             \
	  x = nullptr;          \
	    }                   \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x )  \
    {                       \
    if( x != nullptr )      \
	    {                   \
      delete[] x;           \
	  x = nullptr;          \
	    }                   \
                            \
    }

// Log
#define LOG_OUTPUT(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);
void log(const char file[], int line, const char* format, ...);

// -------------------------------
// -------------------------------

// Data types
struct SuStoVec2 
{
	SuStoVec2();
	SuStoVec2(float x, float y);
	SuStoVec2(const SuStoVec2& copy);
	bool operator == (const SuStoVec2& comp);

	SuStoVec2 Zero();
	float* ptr();

	float x, y = 0;
};

struct SuStoVec3
{
	SuStoVec3();
	SuStoVec3(float x, float y, float z);
	SuStoVec3(const SuStoVec3& copy);
	bool operator == (const SuStoVec3& comp);

	SuStoVec3 Zero();
	float* ptr();

	float x, y, z = 0;
};

struct SuStoRect
{
	SuStoRect();
	SuStoRect(float x, float y, float w, float z);
	SuStoRect(const SuStoRect& copy);
	bool operator == (const SuStoRect& comp);
	bool PointInRect(const SuStoVec2& point);
	void SetPos(const SuStoVec2& pos);
	void SetSize(const SuStoVec2& size);

	float* ptr();

	float x, y, w, h = 0;
	SuStoVec2 xy, wh;
};

struct SuStoColor
{
	SuStoColor();
	SuStoColor(float r, float g, float b, float a);
	SuStoColor(const SuStoColor& copy);
	bool operator == (const SuStoColor& comp);

	float* ptr();

	float r, g, b, a = 0;
};

struct SuStoPlane
{
	SuStoPlane();
	SuStoPlane(SuStoVec2 size);

	uint GetVerticesId();
	uint GetIndicesId();
	uint GetUvsId();

	void SetVerticesId(uint set);
	void SetIndicesId(uint set);
	void SetUvsId(uint set);

	uint GetNumVertices();
	uint GetNumIndices();
	uint GetNumUvs();

	float* GetVertices();
	uint* GetIndices();
	float* GetUvs();

	void CleanUp();

private:
	uint   id_vertices = 0;
	uint   num_vertices = 0;
	float* vertices = nullptr;

	uint   id_indices = 0;
	uint   num_indices = 0;
	uint*  indices = nullptr;

	uint   id_uvs = 0;
	uint   num_uvs = 0;
	float* uvs = nullptr;
};

class PrintableElement
{
public:
	PrintableElement(uint texture_id, SuStoVec2 texture_size, SuStoVec2 pos);

	uint GetNumVertices();
	uint GetNumIndices();
	uint GetNumUvs();

	float* GetVertices();
	uint* GetIndices();
	float* GetUvs();

	uint GetTextureId();

	void SetPos(SuStoVec2 pos);
	SuStoVec2 GetPos();

	void CleanUp();

private:
	uint       texture_id;
	SuStoVec2  texture_size;
	SuStoVec2  local_pos;
	SuStoPlane plane;
};


enum ElementType
{
	PANEL,
	IMAGE,
	TEXT,
	BUTTON,
	TEXT_INPUT,
	CHECKBOX,

	UNDEFINED
};

class SuStoUIMain
{
public:
	SuStoUIMain();
	~SuStoUIMain();

	void Awake();
	void Start();
	void PreUpdate();
	void Update();
	void PostUpdate();
	void CleanUp();

	void PushEvent(UIEvent ev);

	UICanvas* CreateCanvas(SuStoVec2 pos, SuStoVec2 size);
	UIElement * CreateElement(ElementType type, UICanvas* canvas);
	void DeleteElement(UIElement* del);
	void DeleteCanvas(UICanvas* cv);

	void SetViewport(SuStoVec2 view);
	void SetWindowViewport(SuStoVec2 view);
	SuStoVec2 GetViewport();
	SuStoVec2 GetWindowViewport();

	void DrawPrintable(PrintableElement* element);
	void DestroyPrintable(PrintableElement* element);
	std::vector<PrintableElement*> GetDrawList();

private:
	void DestroyElements();

private:
	std::vector<UIElement*> elements;
	std::vector<UICanvas*> canvas;
	std::list<UIElement*>  to_delete;

	std::vector<PrintableElement*> draw;

private:
	SuStoVec2 window_viewport;
	SuStoVec2 viewport;
};

namespace ImGui
{
	std::string ToUpperCase(std::string str);
	std::string ToLowerCase(std::string str);
	bool TextCmp(const char * text1, const char * text2);
}


#endif // !SUSTO_UI
