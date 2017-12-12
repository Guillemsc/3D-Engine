#ifndef _UI_PANEL_
#define _UI_PANEL_

#include "UIElement.h"

class UIPanel : public UIElement
{
public:
	UIPanel(SuStoUIMain* ui_main, UICanvas* canvas);
	~UIPanel();

	void Awake();
	void Start();
	void Update();
	void CleanUp();

	void OnEvent(UIEvent ev);

private:

public:

};


#endif // !_UI_PANEL_
