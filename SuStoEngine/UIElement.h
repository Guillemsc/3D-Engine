#ifndef UI_ELEMENT
#define UI_ELEMENT

#include "SuStoUI.h"
#include "SuSto_event_system.h"
#include "GeometryMath.h"

class UICanvas;

class UIElement
{
	friend UICanvas;

public:
	UIElement();
	UIElement(SuStoUIMain* main, ElementType type, UICanvas* canvas);
	~UIElement();

	virtual void Awake() {};
	virtual void Start() {};
	virtual void PreUpdate() {};
	virtual void Update() {};
	virtual void PostUpdate() {};
	virtual void CleanUp() {};

	virtual void OnEvent(UIEvent ev) {};

	ElementType GetType();
	SuStoUIMain* GetUIMain();

	void SetTransform(float4x4 transform);
	float4x4 GetTransform();

	void SetLocalPos(const SuStoVec2& pos);
	SuStoVec2 GetLocalPos() const;

	void SetAnchor(const SuStoVec2& anchor);
	SuStoVec2 GetAnchor();

	UICanvas* GetCanvas();

private:
	SuStoUIMain* ui_main = nullptr;
	ElementType  type = ElementType::UNDEFINED;
	UICanvas*    canvas = nullptr;

	SuStoVec2    size;
	SuStoVec2    local_pos;
	float4x4	 transform;

	SuStoVec2    anchor = SuStoVec2(0.5f, 0.5f);
};

#endif