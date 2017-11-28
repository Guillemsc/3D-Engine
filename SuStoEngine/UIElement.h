#ifndef UI_ELEMENT
#define UI_ELEMENT

#include "SuStoUI.h"

using namespace SuStoUI;

class UIElement
{
public:
	UIElement(SuStoUIMain* main, ElementType type);
	~UIElement();

	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void CleanUp();

	virtual void OnEvent();

private:
	SuStoUIMain* ui_main = nullptr;
	ElementType  type = ElementType::UNDEFINED;
	Vec2		 pos;
};

#endif