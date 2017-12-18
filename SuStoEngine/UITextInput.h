#ifndef _UI_TEXT_INPUT_
#define _UI_TEXT_INPUT_

#include "UIElement.h"

class UIFont;

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
	void SetTextSize(int size);
	std::string GetText() const;

	PrintableElement* GetImage();

private:
	void SetText(const char* new_text);

private:
	PrintableElement* image = nullptr;
	UIFont*			  font = nullptr;
	uint			  text_size = 100;

	std::string		  text_data;

	bool			  fist_click = true;
};


#endif // !_UI_TEXT_INPUT_