#ifndef UI_ELEMENT
#define UI_ELEMENT

#include "SuStoUI.h"
#include "SuSto_event_system.h"

using namespace SuStoUI;

class UIElement
{
public:
	UIElement();
	UIElement(SuStoUIMain* main, ElementType type);
	~UIElement();

	virtual void Awake() {};
	virtual void Start() {};
	virtual void Update() {};
	virtual void CleanUp() {};

	virtual void OnEvent(UIEvent ev) {};

	void SetPos(const Vec2& pos);
	Vec2 GetPos() const;

	void SetSize(const Vec2 & size);
	Vec2 GetSize() const;

	void SetRect(const Rect& _rect);
	Rect GetRect() const;

private:
	SuStoUIMain* ui_main = nullptr;
	ElementType  type = ElementType::UNDEFINED;
	Rect		 rect;
	Plane		 plane;
};

#endif