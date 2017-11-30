#ifndef _UI_CHECKBOX_
#define _UI_CHECKBOX_

#include "UIElement.h"

using namespace SuStoUI;

class UICheckBox : public UIElement
{
public:
	UICheckBox(SuStoUIMain* main);
	~UICheckBox();

	void Awake();
	void Start();
	void Update();
	void CleanUp();

	void OnEvent();

private:

public:

};


#endif // !_UI_CHECKBOX_