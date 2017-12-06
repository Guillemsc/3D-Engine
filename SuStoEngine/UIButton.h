#ifndef _UI_BUTTON_
#define _UI_BUTTON_

#include "UIElement.h"

class UIButton : public UIElement
{
public:
	UIButton(SuStoUIMain* main);
	~UIButton();

	void Awake();
	void Start();
	void Update();
	void CleanUp();

	void OnEvent(UIEvent ev);

private:

public:

};


#endif // !_UI_BUTTON_