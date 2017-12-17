#include "UITextInput.h"

UITextInput::UITextInput(SuStoUIMain * main, UICanvas* canvas) : UIElement(main, ElementType::TEXT_INPUT, canvas)
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
	if (ev.mouse_over.element == this || ev.mouse_click.element == this || ev.GetType() == UIEventType::MOUSE_OUT)
	{

	}
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
