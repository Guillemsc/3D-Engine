#include "UIImage.h"
#include "SuStoUI.h"
#include "Globals.h"
#include "DebugDraw.h"

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

	image = new PrintableElement(3, SuStoVec2(0, 0), size, this);
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
	if (image->CheckPoint(SuStoVec2(0, 0)))
	{
		int i = 0;
	}
}

void UIImage::CleanUp()
{
	GetUIMain()->DestroyPrintable(image);
}

void UIImage::OnEvent(UIEvent ev)
{
	switch (ev.GetType())
	{
	case UIEventType::MOUSE_OVER:
		LOG_OUTPUT("OVER!!");
		break;
	}
}

PrintableElement * UIImage::GetImage()
{
	return image;
}
