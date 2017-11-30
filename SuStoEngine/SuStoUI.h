#ifndef SUSTO_UI
#define SUSTO_UI

#include <vector>
#include <list>
#include <string>

class UIElement;

// -------------------------------
// GLOBALS -----------------------
// -------------------------------
typedef unsigned int uint;

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

namespace SuStoUI
{
	// Data types
	struct Vec2 
	{
		Vec2();
		Vec2(float x, float y);
		Vec2(const Vec2& copy);
		bool operator == (const Vec2& comp);

		Vec2 Zero();

		float x, y = 0;
	};
	struct Vec3 
	{
		Vec3();
		Vec3(float x, float y, float z);
		Vec3(const Vec3& copy);
		bool operator == (const Vec3& comp);

		float x, y, z = 0;
	};
	struct Rect 
	{
		Rect();
		Rect(float x, float y, float w, float z);
		Rect(const Rect& copy);
		bool operator == (const Rect& comp);
		bool PointInRect(const Vec2& point);
		void SetPos(const Vec2& pos);
		void SetSize(const Vec2& size);

		float x, y, w, h = 0;
	};
	struct Color  
	{
		Color();
		Color(float r, float g, float b, float a);
		Color(const Color& copy);
		bool operator == (const Color& comp);

		float r, g, b, a = 0;
	};
	struct Plane
	{
		Plane();
		Plane(Vec2 size);

		void LoadToMem();
		void UnloadFromMem();

		void CleanUp();

	private:
		uint   id_vertices = 0;
		uint   num_vertices = 0;
		float* vertices = nullptr;

		uint   id_indices = 0;
		uint   num_indices = 0;
		uint*  indices = nullptr;

		uint   id_uv = 0;
		uint   num_uvs = 0;
		float* uvs = nullptr;
	};

	enum ElementType
	{
		CANVAS,
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

		void PushEvent();

		UIElement* CreateElement(ElementType type);
		void DeleteElement(UIElement* del);

	private:
		void DestroyElements();

	private:
		std::vector<UIElement*> elements;
		std::list<UIElement*>  to_delete;
	};

	std::string ToUpperCase(std::string str);
	std::string ToLowerCase(std::string str);
	bool TextCmp(const char * text1, const char * text2); 
}






















#endif // !SUSTO_UI
