// based on https://github.com/nem0/LumixEngine/blob/master/external/imgui/imgui_dock.h

#pragma once

#include "imgui_internal.h"

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

struct DockContext;
class JSON_Doc;
class EditorElement;

extern void igBeginWorkspace(bool *open, ImVec2 pos, ImVec2 size, ImGuiWindowFlags flags);
extern void igEndWorkspace();
extern void igShutdownDock();
extern void igSetNextDock(ImGuiDockSlot slot);
extern bool igBeginDock(const char* label, bool* opened, ImGuiWindowFlags extra_flags);
extern void igEndDock();
extern void igSetDockActive();
extern void igDockDebugWindow();
extern DockContext* getDockContext();

#ifdef __cplusplus
}

struct DockContext
{
	enum EndAction_
	{
		EndAction_None,
		EndAction_Panel,
		EndAction_End,
		EndAction_EndChild
	};


	enum Status_
	{
		Status_Docked,
		Status_Float,
		Status_Dragged
	};


	struct Dock
	{
		Dock();
		~Dock();

		ImVec2 getMinSize();
		bool isHorizontal() const;
		void setParent(Dock* dock);

		Dock& getRoot();
		Dock& getSibling();
		Dock& getFirstTab();

		void setActive();
		bool isContainer() const;

		void setChildrenPosSize(const ImVec2& _pos, const ImVec2& _size);
		void setPosSize(const ImVec2& _pos, const ImVec2& _size);

		char* label;
		ImU32 id;
		Dock* next_tab;
		Dock* prev_tab;
		Dock* children[2];
		Dock* parent;
		bool active;
		ImVec2 pos;
		ImVec2 size;
		Status_ status;
		int last_frame;
		int invalid_frames;
		char location[16];
		bool opened;
		bool first;
	};


	ImVector<Dock*> m_docks;
	ImVec2 m_drag_offset;
	Dock* m_current;
	Dock *m_next_parent;
	int m_last_frame;
	EndAction_ m_end_action;
	ImVec2 m_workspace_pos;
	ImVec2 m_workspace_size;
	ImGuiDockSlot m_next_dock_slot;

	DockContext();
	~DockContext();

	Dock& getDock(const char* label, bool opened);

	void putInBackground();
	void splits();
	void checkNonexistent();

	Dock* getDockAt(const ImVec2& pos) const;

	ImRect getDockedRect(const ImRect& rect, ImGuiDockSlot dock_slot);
	ImRect getSlotRect(ImRect parent_rect, ImGuiDockSlot dock_slot);
	ImRect getSlotRectOnBorder(ImRect parent_rect, ImGuiDockSlot dock_slot);

	Dock* getRootDock();

	bool dockSlots(Dock& dock, Dock* dest_dock, const ImRect& rect, bool on_border);
	void handleDrag(Dock& dock);
	void fillLocation(Dock& dock);

	void doUndock(Dock& dock);
	void drawTabbarListButton(Dock& dock);
	bool tabbar(Dock& dock, bool close_button);

	void setDockPosSize(Dock& dest, Dock& dock, ImGuiDockSlot dock_slot, Dock& container);
	void doDock(Dock& dock, Dock* dest, ImGuiDockSlot dock_slot);

	void rootDock(const ImVec2& pos, const ImVec2& size);
	void setDockActive();
	static ImGuiDockSlot getSlotFromLocationCode(char code);
	static char getLocationCode(Dock* dock);

	void tryDockToStoredLocation(Dock& dock);

	bool begin(const char* label, bool* opened, ImGuiWindowFlags extra_flags);
	void end();
	void debugWindow();
	int getDockIndex(Dock* dock);

	void SetDockSlotPosition(ImGuiDockSlot slot, Dock dock);

	DockContext::Dock * getDockByIndex(int idx);

	void SaveLayout(JSON_Doc* config, const char* layout_name = "default");
	bool LoadLayout(JSON_Doc* config, const char* layout_name);
};

#endif