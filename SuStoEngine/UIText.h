#ifndef _UI_TEXT_
#define _UI_TEXT_

#include <string>

#include "UIElement.h"

class UIText : public UIElement
{
public:
	UIText(SuStoUIMain* ui_main, UICanvas* canvas);
	~UIText();

	void Awake();
	void Start();
	void Update();
	void CleanUp();

	void OnEvent(UIEvent ev);

	void SetText(const char* new_text);
	std::string GetText() const;

private:
	std::string text_data;

public:

};


#endif // !_UI_TEXT_
