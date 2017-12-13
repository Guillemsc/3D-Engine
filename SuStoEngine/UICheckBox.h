#ifndef _UI_CHECKBOX_
#define _UI_CHECKBOX_

#include "UIElement.h"

class UICheckBox : public UIElement
{
public:
	UICheckBox(SuStoUIMain* ui_main, UICanvas* canvas);
	~UICheckBox();

	void Awake();
	void Start();
	void Update();
	void CleanUp();

	void OnEvent(UIEvent ev);

	void SetCheckFalseImage(uint id, SuStoVec2 pos, SuStoVec2 size);
	void SetCheckTrueImage(uint id, SuStoVec2 pos, SuStoVec2 size);

	void SetToFalse();
	void SetToTrue();

	const bool GetState() const;

private:
	PrintableElement* check_false = nullptr;
	PrintableElement* check_true = nullptr;

	PrintableElement* current_state = nullptr;

	bool checkbox_state = false;

public:

};


#endif // !_UI_CHECKBOX_