// based on https://github.com/nem0/LumixEngine/blob/master/external/imgui/imgui_dock.h

#pragma once

#ifdef __cplusplus
extern "C" 
{
#else
typedef struct ImVec2 ImVec2;
typedef int ImGuiWindowFlags;
#endif

typedef enum ImGuiDockSlot 
{
	ImGuiDockSlot_Left,
	ImGuiDockSlot_Right,
	ImGuiDockSlot_Top,
	ImGuiDockSlot_Bottom,
	ImGuiDockSlot_Tab,

	ImGuiDockSlot_Float,
	ImGuiDockSlot_None
} ImGuiDockSlot;

extern void igBeginWorkspace(ImVec2 pos, ImVec2 size, ImGuiWindowFlags extra_flags);
extern void igEndWorkspace();
extern void igShutdownDock();
extern void igSetNextDock(ImGuiDockSlot slot);
extern bool igBeginDock(const char* label, bool* opened, ImGuiWindowFlags extra_flags);
extern void igEndDock();
extern void igSetDockActive();
extern void igDockDebugWindow();

#ifdef __cplusplus
}
#endif