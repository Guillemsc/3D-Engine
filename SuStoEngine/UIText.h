#ifndef _UI_TEXT_
#define _UI_TEXT_

#include <string>

#include "UIElement.h"

using namespace SuStoUI;

class UIText : public UIElement
{
public:
	UIText(SuStoUIMain* main);
	~UIText();

	void Awake();
	void Start();
	void Update();
	void CleanUp();

	void OnEvent();

	void SetText(const char* new_text);
	std::string GetText() const;

private:
	std::string text_data;

public:

};


#endif // !_UI_TEXT_
