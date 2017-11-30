#ifndef _UI_TEXT_INPUT_
#define _UI_TEXT_INPUT_

#include "UIElement.h"

using namespace SuStoUI;

class UITextInput : public UIElement
{
public:
	UITextInput(SuStoUIMain* main);
	~UITextInput();

	void Awake();
	void Start();
	void Update();
	void CleanUp();

	void OnEvent();

	void SetBaseText(const char* base_text);
	std::string GetText() const;

private:
	std::string text_data;
	bool		is_writting = false;

public:

};


#endif // !_UI_TEXT_INPUT_