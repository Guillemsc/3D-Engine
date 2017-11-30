#include "UICanvas.h"

UICanvas::UICanvas(SuStoUIMain * main) : UIElement(main, ElementType::CANVAS)
{
	LOG_OUTPUT("Canvas Created");
}

UICanvas::~UICanvas()
{
}
