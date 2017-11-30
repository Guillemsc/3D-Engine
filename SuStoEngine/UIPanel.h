#ifndef _UI_PANEL_
#define _UI_PANEL_

#include "UIElement.h"

using namespace SuStoUI;

class UIPanel : public UIElement
{
public:
	UIPanel(SuStoUIMain* main);
	~UIPanel();

	void Awake();
	void Start();
	void Update();
	void CleanUp();

	void OnEvent();

private:

public:

};


#endif // !_UI_PANEL_
