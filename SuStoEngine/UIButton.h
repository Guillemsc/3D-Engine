#ifndef _UI_BUTTON_
#define _UI_BUTTON_

#include "UIElement.h"

class UIButton : public UIElement
{
public:
	UIButton(SuStoUIMain* main, UICanvas* canvas);
	~UIButton();

	void Awake();
	void Start();
	void Update();
	void CleanUp();

	void OnEvent(UIEvent ev);

	void SetStandardImage(uint id, SuStoVec2 pos, SuStoVec2 size);
	void SetHighlightImage(uint id, SuStoVec2 pos, SuStoVec2 size);
	void SetClickImage(uint id, SuStoVec2 pos, SuStoVec2 size);


private:
	PrintableElement* standard = nullptr;
	PrintableElement* highlight = nullptr;
	PrintableElement* click = nullptr;

public:

};


#endif // !_UI_BUTTON_