#ifndef _UI_BUTTON_
#define _UI_BUTTON_

#include "UIElement.h"

enum ButtonState
{
	IDLE,
	OVER,
	PRESSED,
};

class UIButton : public UIElement
{
public:
	UIButton(SuStoUIMain* ui_main, UICanvas* canvas);
	~UIButton();

	void Awake();
	void Start();
	void PreUpdate();
	void Update();
	void PostUpdate();
	void CleanUp();

	void OnEvent(UIEvent ev);

	void SetIdleImage(uint id, SuStoVec2 size);
	void SetOverImage(uint id, SuStoVec2 size);
	void SetPressedImage(uint id, SuStoVec2 size);

	int GetIdleId() const;
	int GetOverId() const;
	int GetPressedId() const;

	SuStoVec2 GetIdleSize() const;
	SuStoVec2 GetOverSize() const;
	SuStoVec2 GetPressedSize() const;

	void SetState(ButtonState state);

	PrintableElement* GetImage();

private:
	PrintableElement* image = nullptr;
	SuStoTexture idle;
	SuStoTexture over;
	SuStoTexture pressed;

	ButtonState state = ButtonState::IDLE;

public:

};


#endif // !_UI_BUTTON_