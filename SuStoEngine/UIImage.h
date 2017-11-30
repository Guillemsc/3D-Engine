#ifndef _UI_IMAGE_
#define _UI_IMAGE_

#include "UIElement.h"

using namespace SuStoUI;

class UIImage : public UIElement
{
public:
	UIImage(SuStoUIMain* main);
	~UIImage();

	void Awake();
	void Start();
	void Update();
	void CleanUp();

	void OnEvent();

private:

public:

};


#endif // !_UI_IMAGE_