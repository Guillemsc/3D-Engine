#ifndef _UI_BUTTON_
#define _UI_BUTTON_

#include "UIElement.h"

enum ButtonState
{
	STANDARD,
	HIGHLIGHT,
	CLICK
};

class UIButton : public UIElement
{
public:
	UIButton(SuStoUIMain* ui_main, UICanvas* canvas);
	~UIButton();

	void Awake();
	void Start();
	void PreUpdate();
	void Update();
	void PostUpdate();
	void CleanUp();

	void OnEvent(UIEvent ev);

	void SetStandardImage(uint id, SuStoVec2 pos, SuStoVec2 size);
	void SetHighlightImage(uint id, SuStoVec2 pos, SuStoVec2 size);
	void SetClickImage(uint id, SuStoVec2 pos, SuStoVec2 size);

	void SetToStandard();
	void SetToHighlight();
	void SetToClick();


private:
	PrintableElement* standard = nullptr;
	PrintableElement* highlight = nullptr;
	PrintableElement* click = nullptr;

	PrintableElement* current_state = nullptr;

	ButtonState state = ButtonState::STANDARD;

public:

};


#endif // !_UI_BUTTON_