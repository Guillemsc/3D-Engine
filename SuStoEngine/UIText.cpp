#include "UIText.h"

UIText::UIText(SuStoUIMain * ui_main, UICanvas* canvas) : UIElement(ui_main, ElementType::TEXT, canvas)
{
	LOG_OUTPUT("Text Created");

	text_data = "Text";
	image = new PrintableElement(0, SuStoVec2(0, 0), SuStoVec2(90, 30), this);
	ui_main->DrawPrintable(image);
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

void UIText::PreUpdate()
{
}

void UIText::Update()
{
}

void UIText::PostUpdate()
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

PrintableElement * UIText::GetImage()
{
	return image;
}
