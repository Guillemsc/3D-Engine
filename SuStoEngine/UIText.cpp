#include "UIText.h"
#include "SuSto_fonts_system.h"

UIText::UIText(SuStoUIMain * ui_main, UICanvas* canvas) : UIElement(ui_main, ElementType::TEXT, canvas)
{
	LOG_OUTPUT("Text Created");

	text_data = "Text";
	image = new PrintableElement(0, SuStoVec2(0, 0), SuStoVec2(90, 30), this);
	ui_main->DrawPrintable(image);

	SuStoTexture t(3, SuStoVec2(500, 300)); ui_main->GetFontsSystem()->LoadFont("c:/windows/fonts/arial.ttf");
	image->SetTexture(t);

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
