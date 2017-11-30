#ifndef _UI_TEXT_
#define _UI_TEXT_

#include "UIElement.h"

using namespace SuStoUI;

class UIText : public UIElement
{
public:
	UIText(SuStoUIMain* main);
	~UIText();

	void Awake();
	void Start();
	void Update();
	void CleanUp();

	void OnEvent();

private:

public:

};


#endif // !_UI_TEXT_
