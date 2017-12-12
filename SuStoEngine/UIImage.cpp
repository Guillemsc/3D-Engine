#include "UIImage.h"
#include "SuStoUI.h"

UIImage::UIImage(SuStoUIMain * main, UICanvas* canvas) : UIElement(main, ElementType::IMAGE, canvas)
{
	LOG_OUTPUT("Image Created");
	image = nullptr;
}

UIImage::~UIImage()
{
}

void UIImage::SetImage(uint id, SuStoVec2 size)
{
	if (image != nullptr)
	{
		GetUIMain()->DestroyPrintable(image);
		image = nullptr;
	}

	image = new PrintableElement(id, size, SuStoVec2(0, 0), this);
	GetUIMain()->DrawPrintable(image);
}

void UIImage::Awake()
{
}

void UIImage::Start()
{
}

void UIImage::Update()
{
	
}

void UIImage::CleanUp()
{
}

void UIImage::OnEvent(UIEvent ev)
{
}
