#include "UITextInput.h"
#include "SuSto_fonts_system.h"

UITextInput::UITextInput(SuStoUIMain * ui_main, UICanvas* canvas) : UIElement(ui_main, ElementType::TEXT_INPUT, canvas)
{
	LOG_OUTPUT("Text Input Created");

	image = new PrintableElement(0, SuStoVec2(0, 0), SuStoVec2(0, 0), this);
	ui_main->DrawPrintable(image);

	font = ui_main->GetFontsSystem()->GetFont("default");
	SetTextSize(200);
	SetText("Set Name");
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
	if (OnFocus())
	{
		std::string text = GetText();

		if (GetUIMain()->GetEventSystem()->text_input.size() > 0)
		{
			if (fist_click)
			{
				text.clear();
				fist_click = false;
			}

			text += GetUIMain()->GetEventSystem()->text_input;
			SetText(text.c_str());
		}
	}
}

void UITextInput::CleanUp()
{
	GetUIMain()->DestroyPrintable(image);
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

void UITextInput::SetTextSize(int size)
{
	text_size = size;

	SetText(text_data.c_str());
}

std::string UITextInput::GetText() const
{
	return text_data;
}

PrintableElement * UITextInput::GetImage()
{
	return image;
}

void UITextInput::SetText(const char * new_text)
{
	text_data = new_text;

	GetUIMain()->GetFontsSystem()->UnloadText(image->GetTextureId());
	SuStoTexture tex = GetUIMain()->GetFontsSystem()->LoadText(new_text, font, text_size);
	image->SetTexture(tex);
}
