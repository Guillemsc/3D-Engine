#ifndef _UI_BUTTON_
#define _UI_BUTTON_

#include "UIElement.h"

using namespace SuStoUI;

class UIButton : public UIElement
{
public:
	UIButton(SuStoUIMain* main);
	~UIButton();

	void Awake();
	void Start();
	void Update();
	void CleanUp();

	void OnEvent();

private:

public:

};


#endif // !_UI_BUTTON_