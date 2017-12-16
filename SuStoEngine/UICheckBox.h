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
	void PreUpdate();
	void Update();
	void PostUpdate();
	void CleanUp();

	void OnEvent(UIEvent ev);

	void SetCheckTrueImage(uint id, SuStoVec2 size);
	void SetCheckFalseImage(uint id, SuStoVec2 size);

	void SetState(const bool state);
	const bool GetState() const;

	PrintableElement * GetImage();

private:
	PrintableElement* image = nullptr;
	SuStoTexture check_true;
	SuStoTexture check_false;

	bool state = false;

public:

};


#endif // !_UI_CHECKBOX_