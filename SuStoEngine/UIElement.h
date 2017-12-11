#ifndef UI_ELEMENT
#define UI_ELEMENT

#include "SuStoUI.h"
#include "SuSto_event_system.h"

class UICanvas;

class UIElement
{
public:
	UIElement();
	UIElement(SuStoUIMain* main, ElementType type, UICanvas* canvas);
	~UIElement();

	virtual void Awake() {};
	virtual void Start() {};
	virtual void Update() {};
	virtual void CleanUp() {};

	virtual void OnEvent(UIEvent ev) {};

	ElementType GetType();
	SuStoUIMain* GetUIMain();

	void SetPos(const SuStoVec2& pos);
	SuStoVec2 GetPos() const;

	void SetSize(const SuStoVec2 & size);
	SuStoVec2 GetSize() const;

	void SetRect(const SuStoRect& _rect);
	SuStoRect GetRect() const;

	UICanvas* GetCanvas();

private:
	SuStoUIMain* ui_main = nullptr;
	ElementType  type = ElementType::UNDEFINED;
	SuStoRect    rect;
	UICanvas*    canvas = nullptr;
};

#endif