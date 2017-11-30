#include "UITextInput.h"

UITextInput::UITextInput(SuStoUIMain * main) : UIElement(main, ElementType::TEXT_INPUT)
{
	LOG_OUTPUT("Text Input Created");
}

UITextInput::~UITextInput()
{
}

void UITextInput::Awake()
{
}

void UITextInput::Start()
{
}

void UITextInput::Update()
{
}

void UITextInput::CleanUp()
{
}

void UITextInput::OnEvent(UIEvent ev)
{
}

void UITextInput::SetBaseText(const char * base_text)
{
	text_data.clear();
	text_data = base_text;
}

std::string UITextInput::GetText() const
{
	return text_data;
}
