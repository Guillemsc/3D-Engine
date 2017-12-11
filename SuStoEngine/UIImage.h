#ifndef _UI_IMAGE_
#define _UI_IMAGE_

#include "UIElement.h"

class UIImage : public UIElement
{
public:
	UIImage(SuStoUIMain* main, UICanvas* canvas);
	~UIImage();

	void SetImage(uint id, SuStoVec2 size);

private:
	void Awake();
	void Start();
	void Update();
	void CleanUp();

	void OnEvent(UIEvent ev);

private:
	PrintableElement* image = nullptr;

};


#endif // !_UI_IMAGE_
