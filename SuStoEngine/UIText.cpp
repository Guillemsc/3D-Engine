#include "UIText.h"

UIText::UIText(SuStoUIMain * main) : UIElement(main, ElementType::TEXT)
{
	LOG_OUTPUT("Text Created");
}

UIText::~UIText()
{
}

void UIText::Awake()
{
}

void UIText::Start()
{
}

void UIText::Update()
{
}

void UIText::CleanUp()
{
	text_data.clear();
}

void UIText::OnEvent(UIEvent ev)
{
}

void UIText::SetText(const char * new_text)
{
	text_data.clear();
	text_data = new_text;
}

std::string UIText::GetText() const
{
	return text_data;
}
