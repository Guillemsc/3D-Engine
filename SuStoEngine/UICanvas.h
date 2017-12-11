#ifndef _UI_CANVAS_
#define _UI_CANVAS_

#include "UIElement.h"

class UICanvas
{
public:
	UICanvas(SuStoUIMain* main);
	~UICanvas();

	void Awake();
	void Start();
	void Update();
	void CleanUp();

	void OnEvent(UIEvent ev);

	void SetSize();

private:
	SuStoRect rect;
	SuStoVec2 center;

public:

};


#endif // !_UI_CANVAS_