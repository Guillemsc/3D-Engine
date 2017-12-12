#ifndef _UI_TEXT_INPUT_
#define _UI_TEXT_INPUT_

#include "UIElement.h"

class UITextInput : public UIElement
{
public:
	UITextInput(SuStoUIMain* ui_main, UICanvas* canvas);
	~UITextInput();

	void Awake();
	void Start();
	void Update();
	void CleanUp();

	void OnEvent(UIEvent ev);

	void SetBaseText(const char* base_text);
	std::string GetText() const;

private:
	std::string text_data;
	bool		is_writting = false;

public:

};


#endif // !_UI_TEXT_INPUT_