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

private:

public:

};


#endif // !_UI_CHECKBOX_