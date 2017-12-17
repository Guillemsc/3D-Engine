#include "ModuleGameObject.h"
#include "App.h"
#include "ModuleRenderer3D.h"
#include "ResourceMeshLoader.h"
#include "ResourceManager.h"
#include "DebugDraw.h"
#include "IDGenerator.h"
#include "Functions.h"
#include "KDTree.h"
#include "ModuleInput.h"
#include "SceneManager.h"
#include "ModuleCamera3D.h"
#include "EditorUI.h"
#include "ModuleWindow.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ComponentMaterial.h"
#include "ImGuizmo.h"
#include "imgui.h"
#include "Resource.h"
#include "ResourceTextureLoader.h"
#include "ComponentImage.h"
#include "ComponentText.h"
#include "ComponentButton.h"

#include "SuSto_impl_sdl_opengl.h"
#include "SuStoUI.h"
#include "UIImage.h"
#include "UICanvas.h"
#include "UIButton.h"

#define BUTTON_SIZE SuStoVec2(96, 25)
#define BUTTON_PADDING SuStoVec2(9, 5)


ModuleGameObject::ModuleGameObject(bool enabled)
{
	SetName("GameObject");
}

ModuleGameObject::~ModuleGameObject()
{
}

bool ModuleGameObject::Awake()
{
	bool ret = true;

	LOG_OUTPUT("Creating Module GameObject");	

	// KDTree
	kdtree = new KDTree();

	// Root GameObject
	root = new GameObject("Root", this);
	root->Start();
	root->SetName("Root");

	return ret;
}

bool ModuleGameObject::Start()
{
	bool ret = true;

	GameObject* t = App->gameobj->Create();

	ComponentMesh* cm = (ComponentMesh*)t->AddComponent(MESH);
	ResourceMesh* rm = App->resource_manager->GetMeshLoader()->CreatePlaneMesh(float2(10, 10));

	cm->SetMesh(rm);

	susto_ui = SuStoUI::Init(App->window->window);
	susto_ui->SetFrustum(App->camera->GetCurrentCamera()->GetFrustum());

	ResourceTextureLoader loader;
	TextureInfo backgroud = loader.LoadTexture("UI\\background.png", true);
	TextureInfo test = loader.LoadTexture("UI\\preview_164.png", true);
	TextureInfo standard = loader.LoadTexture("UI\\button_standard.png", true);
	TextureInfo highlight = loader.LoadTexture("UI\\button_highlight.png", true);
	TextureInfo click = loader.LoadTexture("UI\\button_click.png", true);
	TextureInfo check_false = loader.LoadTexture("UI\\checkbox_false.png", true);
	TextureInfo check_true = loader.LoadTexture("UI\\checkbox_true.png", true);
	TextureInfo options = loader.LoadTexture("UI\\options.png", true);
	
	textures.push_back(test);
	textures.push_back(standard);
	textures.push_back(highlight);
	textures.push_back(click);
	textures.push_back(check_false);
	textures.push_back(check_true);
	textures.push_back(options);

	GameObject* canvas_go = App->gameobj->Create();
	canvas_go->AddComponent(UI_CANVAS);

	//GameObject* background_go = App->gameobj->Create();
	//canvas_go->AddChild(background_go);

	//ComponentImage* cimage = (ComponentImage*)background_go->AddComponent(UI_IMAGE);
	//cimage->SetImage(backgroud.id, float2(backgroud.size_x, backgroud.size_y));

	GameObject* text_go = App->gameobj->Create();
	canvas_go->AddChild(text_go);

	ComponentText* ctext = (ComponentText*)text_go->AddComponent(UI_TEXT);
	ctext->SetText("hdddddddddddddi");

	GameObject* button_start_go = App->gameobj->Create();
	canvas_go->AddChild(button_start_go);

	ComponentButton* cbutton = (ComponentButton*)button_start_go->AddComponent(UI_BUTTON);
	cbutton->SetIdleImage(standard.id, float2(standard.size_x, standard.size_y));
	cbutton->SetOverImage(highlight.id, float2(highlight.size_x, highlight.size_y));
	cbutton->SetPressedImage(click.id, float2(click.size_x, click.size_y));

	cbutton->GetButton()->SetLocalPos(SuStoVec2(-100, -100));

	return ret;
}

bool ModuleGameObject::PreUpdate()
{
	bool ret = true;

	root->RecursiveCalcGlobalTransform();
	root->RecursiveCalcBBox();

	UpdateKDTree();

	float x = App->editorUI->GameRect().left;
	float y = App->editorUI->GameRect().top;
	float w = App->editorUI->GameRect().right - App->editorUI->GameRect().left;
	float h = App->editorUI->GameRect().bottom - App->editorUI->GameRect().top;

	SuStoUI::NewFrame(susto_ui, App->window->window, SuStoRect(x, y, w, h), App->camera->GetCurrentCamera()->GetFrustum());

	return ret;
}

bool ModuleGameObject::Update()
{
	bool ret = true;

	if (App->input->GetMouseButton(1))
		MousePick();

	SuStoUI::Render(susto_ui);
	//DebugDraw(susto_ui->GetPicking());

	vector<Camera3D*> cameras = App->camera->GetCameras();

	// Get elements to draw from all cameras
	App->camera->GetEditorCamera()->GetElementsToDraw();
	
	// Update
	for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		(*it)->UpdateComponents();

		if (App->scene_manager->GetState() == SceneState::PLAY)
			(*it)->UpdateLogic();

		(*it)->Draw();

		(*it)->SetDebugDraw(show_bboxes);
	}

	for (vector<GameObject*>::iterator it = selected.begin(); it != selected.end(); ++it)
	{
		float4x4 transform = (*it)->transform->GetGlobalTransform().Transposed();

		float transformation[16];
		ImGuizmo::Manipulate(App->camera->GetCurrentCamera()->GetOpenGLViewMatrix().ptr(),
			App->camera->GetCurrentCamera()->GetOpenGLProjectionMatrix().ptr(),
			current_gizmo_operation,
			ImGuizmo::MODE::WORLD,
			transform.ptr(), transformation);

		if (ImGuizmo::IsUsing() && can_move)
		{
			float addition[3];
			float rotation[3];
			float scale[3];
			ImGuizmo::DecomposeMatrixToComponents(transformation, addition, rotation, scale);
			float3 add(addition[0], addition[1], addition[2]);
			float3 rot(rotation[0], rotation[1], rotation[2]);
			float3 sc(scale[0], scale[1], scale[2]);

			switch (current_gizmo_operation)
			{
				case ImGuizmo::OPERATION::TRANSLATE:
				{
					if (add.IsFinite()) 
					{
						if ((*it)->parent != nullptr) 
						{
							add = (*it)->parent->transform->GetGlobalTransform().Inverted().TransformPos(add);
						}

						(*it)->transform->Translate(add);
					}		
				}
				break;

				case ImGuizmo::OPERATION::ROTATE:
				{
					if (rot.IsFinite()) 
					{
						if ((*it)->parent != nullptr) 
						{
							rot = (*it)->parent->transform->GetGlobalTransform().Inverted().TransformPos(rot);
						}
						(*it)->transform->Rotate(rot);
					}				
				}
				break;
				case ImGuizmo::OPERATION::SCALE:
				{
					if (sc.IsFinite()) 
					{
						(*it)->transform->SetScale(sc);
					}
				}
				break;
			}
		}
	}

	if (ImGuizmo::IsOver())
		can_pick = false;
	else
		can_pick = true;
	
	if (show_kdtree)
		kdtree->DebugDraw();

	/*App->renderer3D->DrawGrid(100);*/

	return ret;
}

bool ModuleGameObject::PostUpdate()
{
	bool ret = true;

	DestroyGameObjects();

	return ret;
}

bool ModuleGameObject::CleanUp()
{
	bool ret = true;
 
	Destroy(root);

	DestroyGameObjects();

	kdtree->CleanUp();
	RELEASE(kdtree);

	SuStoUI::DeInit(susto_ui);

	textures.clear();

	return ret;
}

GameObject * ModuleGameObject::Create(std::string force_id)
{
	string new_id;

	if (force_id == "")
		new_id = App->resource_manager->GetNewUID();
	else
		new_id = force_id;

	GameObject* game_object = new GameObject(new_id, this);

	game_objects.push_back(game_object);
	root->AddChild(game_object);
	game_object->Start();

	return game_object;
}

void ModuleGameObject::Destroy(GameObject * go)
{
	for (list<GameObject*>::iterator it = to_delete.begin(); it != to_delete.end(); ++it)
	{
		if (go == (*it))
			return;
	}

	to_delete.push_back(go);
}

GameObject* ModuleGameObject::Find(std::string unique_id)
{
	GameObject* ret = nullptr;

	for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		if ((*it)->GetUniqueId() == unique_id)
		{
			ret = (*it);
			break;
		}
	}

	return ret;
}

void ModuleGameObject::DestroyAllGameObjects()
{
	for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		Destroy((*it));
	}
}

void ModuleGameObject::DestroyAllGameObjectsNow()
{
	DestroyAllGameObjects();

	DestroyGameObjects();
}

const vector<GameObject*> ModuleGameObject::GetListGameObjects() const
{
	return game_objects;
}

void ModuleGameObject::AddGameObjectToSelected(GameObject * go)
{
	if (go->GetSelected())
		return;

	for (vector<GameObject*>::iterator it = selected.begin(); it != selected.end(); ++it)
	{
		if ((*it) == go)
			return;
	}

	selected.push_back(go);
	go->selected = true;
}

void ModuleGameObject::RemoveGameObjectFromSelected(GameObject * go)
{
	if (!go->GetSelected())
		return;

	for (vector<GameObject*>::iterator it = selected.begin(); it != selected.end(); ++it)
	{
		if ((*it) == go)
		{
			go->selected = false,
			selected.erase(it);
			return;
		}
	}
}

void ModuleGameObject::ClearSelection()
{
	for (vector<GameObject*>::iterator it = selected.begin(); it != selected.end();)
	{
		(*it)->selected = false;
		it = selected.erase(it);
	}
}

void ModuleGameObject::DestroySelectedGameObjects()
{
	for (vector<GameObject*>::iterator it = selected.begin(); it != selected.end(); it++)
	{
		App->gameobj->Destroy((*it));
	}
}

GameObject * ModuleGameObject::GetRoot()
{
	return root;
}

const vector<GameObject*> ModuleGameObject::GetSelectedGameObjects() const
{
	return selected;
}

void ModuleGameObject::AddGameObjectToStatic(GameObject * go)
{
	if (go->GetStatic())
		return;
	
	for (vector<GameObject*>::iterator it = statics.begin(); it != statics.end(); ++it)
	{
		if ((*it) == go)
		{
			return;
		}
	}

	statics.push_back(go);
	go->is_static = true;

	RecalculateKDTree();
}

void ModuleGameObject::RemoveGameObjectFromStatic(GameObject * go)
{
	if (!go->GetStatic())
		return;

	for (vector<GameObject*>::iterator it = statics.begin(); it != statics.end(); ++it)
	{
		if ((*it) == go)
		{
			statics.erase(it);
			go->is_static = false;
			break;
		}
	}

	RecalculateKDTree();
}

const vector<GameObject*> ModuleGameObject::GetStaticGameObjects() const
{
	return statics;
}

void ModuleGameObject::AddGameObjectToStaticRecursive(GameObject * go)
{
	AddGameObjectToStatic(go);

	vector<GameObject*> childs = go->GetChilds();
	for (vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		AddGameObjectToStaticRecursive(*it);
	}
}

void ModuleGameObject::RemoveGameObjectFromStaticRecursive(GameObject * go)
{
	RemoveGameObjectFromStatic(go);

	vector<GameObject*> childs = go->GetChilds();
	for (vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		RemoveGameObjectFromStaticRecursive(*it);
	}
}

const vector<GameObject*> ModuleGameObject::GetDynamicGameObjects() const
{
	vector<GameObject*> dynamics;

	for (vector<GameObject*>::const_iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		if (!(*it)->is_static)
			dynamics.push_back((*it));
	}

	return dynamics;
}

void ModuleGameObject::DeleteGameObjectsUsingResource(Resource * res)
{
	for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		switch (res->GetType())
		{
		case RT_MESH:
		{
			ComponentMesh* cmesh = (ComponentMesh*)(*it)->GetComponent(MESH);
			if (cmesh != nullptr)
			{
				if (cmesh->GetMesh() == (ResourceMesh*)res)
				{
					(*it)->RemoveComponent(MESH);
					Destroy((*it));
				}
			}
		}
			break;
		case RT_TEXTURE:
		{
			ComponentMaterial* cmat = (ComponentMaterial*)(*it)->GetComponent(MATERIAL);
			if (cmat != nullptr)
			{
				if (cmat->GetTexture() == (ResourceTexture*)res)
				{
					(*it)->RemoveComponent(MATERIAL);
					Destroy((*it));
				}
			}
		}
			break;
		}
	}
}

void ModuleGameObject::SetGuizmoOperation(ImGuizmo::OPERATION op)
{
	current_gizmo_operation = op;
}

KDTree * ModuleGameObject::GetKDTree()
{
	return kdtree;
}

void ModuleGameObject::RecalculateKDTree()
{
	update_kdtree = true;
}

void ModuleGameObject::SetDebugKDTree(bool set)
{
	show_kdtree = set;
}

void ModuleGameObject::SetDebugBBoxes(bool set)
{
	show_bboxes = set;
}

void ModuleGameObject::RecursiveGetGameObjectTree(GameObject * go, vector<GameObject*>& fill)
{
	fill.push_back(go);

	vector<GameObject*> childs = go->GetChilds();
	for (vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		RecursiveGetGameObjectTree(*it, fill);
	}
}

void ModuleGameObject::SetCanPick(bool set)
{
	can_pick = set;
}

void ModuleGameObject::SetCanMove(bool set)
{
	can_move = set;
}

SuStoUIMain * ModuleGameObject::GetUIMain()
{
	return susto_ui;
}

vector<TextureInfo> ModuleGameObject::GetTextures()
{
	return textures;
}

void ModuleGameObject::DestroyGameObjects()
{
	for (list<GameObject*>::iterator to_del = to_delete.begin(); to_del != to_delete.end();)
	{
		// Add childs to delete
		vector<GameObject*> childs = (*to_del)->GetChilds();
		for (vector<GameObject*>::iterator ch = childs.begin(); ch != childs.end(); ++ch)
		{
			(*ch)->SetParentToNull();
			Destroy(*ch);
		}
		
		// Reset parent
		if ((*to_del)->GetParent() != nullptr)
		{
			(*to_del)->GetParent()->EraseChild(*to_del, false);
		}

		// Delete from list
		for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end();)
		{
			if ((*to_del) == (*it))
			{
				it = game_objects.erase(it);
				break;
			}
			else
				++it;
		}

		// Delete from selected
		RemoveGameObjectFromSelected(*to_del);
		RemoveGameObjectFromStatic(*to_del);

		// Free
		(*to_del)->CleanUp();
		delete (*to_del);

		to_del = to_delete.erase(to_del);
	}
}

void ModuleGameObject::MousePick()
{
	Rect window = App->editorUI->GameRect();
	float2 mouse_pos = App->input->GetMouse();

	if (PointInRect(mouse_pos, window))
	{
		 //The point (1, 1) corresponds to the top-right corner of the near plane
		 //(-1, -1) is bottom-left

		float first_normalized_x = (mouse_pos.x - window.left) / (window.right - window.left);
		float first_normalized_y = (mouse_pos.y - window.top) / (window.bottom - window.top);

		float normalized_x = (first_normalized_x * 2) - 1;
		float normalized_y = 1 - (first_normalized_y * 2);

		LineSegment picking = App->camera->GetCurrentCamera()->GetFrustum().UnProjectLineSegment(normalized_x, normalized_y);

		float distance = 99999999999;
		GameObject* closest = nullptr;

		vector<GameObject*> gos = App->gameobj->GetDynamicGameObjects();
		kdtree->GetElementsToTest(picking, 0, App->camera->GetCurrentCamera()->GetFarPlaneDistance(), gos);

		for (vector<GameObject*>::iterator it = gos.begin(); it != gos.end(); it++)
		{
			bool hit;
			float dist;
			(*it)->TestRay(picking, hit, dist);

			if (hit)
			{
				if (dist < distance)
				{
					distance = dist;
					closest = (*it);
				}
			}
		}

		if (closest != nullptr)
		{
			ClearSelection();
			AddGameObjectToSelected(closest);
		}
	}
}

void ModuleGameObject::UpdateKDTree()
{
	if (update_kdtree)
	{
		update_kdtree = false;
		kdtree->CreateTree(statics);
	}
}

