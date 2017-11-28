#ifndef SUSTO_UI
#define SUSTO_UI

#include <vector>
#include <list>

class UIElement;

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

		float x, y, w, z = 0;
	};

	struct Color 
	{
		Color();
		Color(float r, float g, float b, float a);
		Color(const Color& copy);
		bool operator == (const Color& comp);

		float r, g, b, a = 0;
	};

	enum ElementType
	{
		UNDEFINED,
		CANVAS,
		PANEL,
		IMAGE,
		TEXT,
		BUTTON,
		TEXT_INPUT,
		CHECKBOX
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
}






















#endif // !SUSTO_UI
