#ifndef _UI_TEXT_
#define _UI_TEXT_

#include <string>
#include "UIElement.h"

class UIFont;

class UIText : public UIElement
{
public:
	UIText(SuStoUIMain* ui_main, UICanvas* canvas);
	~UIText();

	void Awake();
	void Start();
	void PreUpdate();
	void Update();
	void PostUpdate();
	void CleanUp();

	void OnEvent(UIEvent ev);

	void SetText(const char* new_text);
	std::string GetText() const;

	void SetTextSize(uint size);
	uint GetTextSize();

	PrintableElement* GetImage();

private:
	PrintableElement* image = nullptr;
	UIFont*			  font = nullptr;
	uint			  text_size = 100;

	std::string		  text_data;

public:

};


#endif // !_UI_TEXT_
