#ifndef _UI_CANVAS_
#define _UI_CANVAS_

#include "UIElement.h"

using namespace SuStoUI;

class UICanvas : public UIElement
{
public:
	UICanvas(SuStoUIMain* main);
	~UICanvas();

	void Awake();
	void Start();
	void Update();
	void CleanUp();

	void OnEvent(UIEvent ev);

	void SetWidthAndHeight(float width, float height);

private:

public:
	
};


#endif // !_UI_CANVAS_