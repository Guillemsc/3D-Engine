#include "App.h"
#include "ModuleGameObject.h"
#include "GameObjectAbstractor.h"
#include "ModuleFileSystem.h"
#include "GameObject.h"
#include "Component.h"
#include "JSONLoader.h"
#include "GeometryMath.h"

GameObjectAbstractor::GameObjectAbstractor()
{
}

GameObjectAbstractor::~GameObjectAbstractor()
{
}

GameObjectAbstraction GameObjectAbstractor::Abstract(GameObject * go)
{
	GameObjectAbstraction ret;

	if (go != nullptr)
	{
		ret.name = go->GetName();

		int id_count = 0;

		std::vector<GameObject*> to_add;
		to_add.push_back(go);


		while (!to_add.empty())
		{
			GameObject* curr = *to_add.begin();

			if (curr != App->gameobj->GetRoot())
			{
				int go_id = ret.GetIdFromGo(curr);

				if (go_id == -1)
					go_id = ret.AddGo(curr);

				if (curr->parent != nullptr && curr->parent != App->gameobj->GetRoot())
				{
					int parent_go_id = ret.GetIdFromGo(curr->parent);

					if (parent_go_id == -1)
						parent_go_id = ret.AddGo(curr);

					ret.AddRelation(go_id, parent_go_id);
				}
			}

			for (std::vector<GameObject*>::iterator it = curr->childs.begin(); it != curr->childs.end(); ++it)
			{
				to_add.push_back(*it);
			}

			to_add.erase(to_add.begin());
		}

		ret.valid = true;
	}

	return ret;
}

GameObject * GameObjectAbstractor::DeAbstract(GameObjectAbstraction abs)
{
	GameObject* ret = nullptr;

	bool first_go = true;

	if (abs.GetValid())
	{
		for (std::vector<IDRelation>::iterator it = abs.id_relations.begin(); it != abs.id_relations.end(); ++it)
		{
			GameObject* obj = App->gameobj->Create();

			if (first_go)
			{
				ret = obj;
				first_go = false;
			}

			if (obj != nullptr)
			{
				obj->OnLoadAbstraction((*it).data_game_object);

				for (std::vector<DataAbstraction>::iterator comp = (*it).data_components.begin(); comp != (*it).data_components.end(); ++comp)
				{
					int comp_int = (*comp).GetInt("type", -1);

					if (comp_int != -1)
					{
						ComponentType type = static_cast<ComponentType>(comp_int);

						if (type != ComponentType::TRANSFORM)
						{
							Component* component = obj->AddComponent(type);

							if (component != nullptr)
							{
								component->OnLoadAbstraction(*comp);
							}
						}
						else
						{
							obj->transform->OnLoadAbstraction(*comp);
						}
					}
				}

				(*it).go = obj;
			}
		}

		for (std::vector<GORelation>::iterator it = abs.go_relations.begin(); it != abs.go_relations.end(); ++it)
		{
			int id = (*it).id;
			int id_parent = (*it).id_parent;

			GameObject* obj = nullptr;
			GameObject* parent_obj = nullptr;

			if (id != -1 && id_parent != -1)
			{
				obj = abs.GetGoFromId(id);
				parent_obj = abs.GetGoFromId(id_parent);

				if (obj != nullptr && parent_obj != nullptr)
				{
					obj->SetParent(parent_obj);
				}
			}
		}
	}

	return ret;
}

void GameObjectAbstractor::Serialize(GameObjectAbstraction abs, const char* path, const char* name, const char* extension)
{
	if (abs.GetValid())
	{
		if (App->file_system->FolderExists(path))
		{
			std::string filepath = std::string(path) + std::string(name) + std::string(".") + std::string(extension);

			JSON_Doc* doc = App->json->CreateJSON(path, name, extension);

			if (doc != nullptr)
			{
				int curr_go_count = 0;
				doc->SetNumber("GameObjectsCount", abs.id_relations.size());
				for (std::vector<IDRelation>::iterator it = abs.id_relations.begin(); it != abs.id_relations.end(); ++it, ++curr_go_count)
				{
					doc->MoveToRoot();

					std::string curr_go_section_name = "GameObject_" + std::to_string(curr_go_count);
					doc->AddSection(curr_go_section_name);

					if (doc->MoveToSection(curr_go_section_name))
					{
						JSON_Doc go_node = doc->GetJsonNode();

						go_node.SetNumber("id", (*it).id);
						go_node.SetNumber("parent_id", abs.GetParentFromId((*it).id));

						(*it).data_game_object.Serialize(go_node);

						doc->SetArray("Components");

						bool curr_comp_count = 0;
						for (std::vector<DataAbstraction>::iterator comp = (*it).data_components.begin(); comp != (*it).data_components.end(); ++comp, ++curr_comp_count)
						{
							JSON_Doc comp_node = doc->GetJsonNode();

							comp_node.AddSectionToArray("Components");

							if (comp_node.MoveToSectionFromArray("Components", curr_comp_count))
							{
								(*comp).Serialize(comp_node);
							}
						}

					}
				}

				doc->Save();
				doc->MoveToRoot();
			}
		}
	}
}

GameObjectAbstraction GameObjectAbstractor::DeSerialize(const char * filepath)
{
	GameObjectAbstraction ret;

	if (App->file_system->FileExists(filepath))
	{
		JSON_Doc* doc = App->json->LoadJSON(filepath);

		if (doc != nullptr)
		{
			int game_objects_count = doc->GetNumber("GameObjectsCount", 0);

			for (int i = 0; i < game_objects_count; ++i)
			{
				doc->MoveToRoot();

				std::string curr_go_section_name = "GameObject_" + std::to_string(i);

				if (doc->MoveToSection(curr_go_section_name))
				{
					JSON_Doc go_node = doc->GetJsonNode();

					IDRelation go_relation;
					int id = go_node.GetNumber("id", -1);
					int parent_id = go_node.GetNumber("parent_id", -1);
					if (id != -1)
					{
						go_relation.id = id;

						if (parent_id != -1)
						{
							ret.AddRelation(id, parent_id);
						}

						go_relation.data_game_object.DeSerialize(go_node);

						int components_count = go_node.GetArrayCount("Components");

						for (int c = 0; c < components_count; ++c)
						{
							JSON_Doc comp_node = go_node;

							if (comp_node.MoveToSectionFromArray("Components", c))
							{
								DataAbstraction comp_data;
								comp_data.DeSerialize(comp_node);

								go_relation.data_components.push_back(comp_data);
							}
						}

						ret.id_relations.push_back(go_relation);
					}
				}
			}

			doc->MoveToRoot();
			ret.valid = true;
		}
	}

	return ret;
}

GameObjectAbstraction::GameObjectAbstraction()
{
}

GameObjectAbstraction::~GameObjectAbstraction()
{
}

bool GameObjectAbstraction::GetValid() const
{
	return valid;
}

std::string GameObjectAbstraction::GetName()
{
	return name;
}

void GameObjectAbstraction::Clear()
{
	go_relations.clear();
	id_relations.clear();
	max_id = -1;
	valid = false;
}

void GameObjectAbstraction::AddRelation(int id, int parent_id)
{
	if (id != -1 && parent_id != -1)
	{
		GORelation relation(id, parent_id);
		go_relations.push_back(relation);
	}
}

int GameObjectAbstraction::AddGo(GameObject * go)
{
	if (go != nullptr)
	{
		IDRelation relation(++max_id, go);

		go->OnSaveAbstraction(relation.data_game_object);

		std::vector<Component*> components_list = go->GetComponents();

		for (std::vector<Component*>::iterator it = components_list.begin(); it != components_list.end(); ++it)
		{
			Component* curr = (*it);

			DataAbstraction comp_abs;
			comp_abs.AddInt("type", curr->GetType());
			curr->OnSaveAbstraction(comp_abs);

			relation.data_components.push_back(comp_abs);
		}

		id_relations.push_back(relation);
	}

	return max_id;
}

GameObject * GameObjectAbstraction::GetGoFromId(int id)
{
	GameObject* ret = nullptr;

	for (std::vector<IDRelation>::iterator it = id_relations.begin(); it != id_relations.end(); ++it)
	{
		if ((*it).id == id)
		{
			ret = (*it).go;
			break;
		}
	}

	return ret;
}

int GameObjectAbstraction::GetIdFromGo(GameObject * go)
{
	int ret = -1;

	for (std::vector<IDRelation>::iterator it = id_relations.begin(); it != id_relations.end(); ++it)
	{
		if ((*it).go == go)
		{
			ret = (*it).id;
			break;
		}
	}

	return ret;
}

int GameObjectAbstraction::GetParentFromId(int id)
{
	int ret = -1;

	for (std::vector<GORelation>::iterator it = go_relations.begin(); it != go_relations.end(); ++it)
	{
		if ((*it).id == id)
		{
			ret = (*it).id_parent;
			break;
		}
	}

	return ret;
}

void DataAbstraction::Clear()
{
	ints.clear();
	bools.clear();
	floats.clear();
	strings.clear();
	floats3.clear();
	floats4.clear();
}

void DataAbstraction::AddInt(std::string name, int val)
{
	ints[name] = val;
}

void DataAbstraction::AddBool(std::string name, bool val)
{
	bools[name] = val;
}

void DataAbstraction::AddFloat(std::string name, float val)
{
	floats[name] = val;
}

void DataAbstraction::AddString(std::string name, std::string val)
{
	strings[name] = val;
}

void DataAbstraction::AddFloat3(std::string name, float3 val)
{
	floats3[name] = val;
}

void DataAbstraction::AddFloat4(std::string name, float4 val)
{
	floats4[name] = val;
}

int DataAbstraction::GetInt(std::string name, int def)
{
	std::map<std::string, int>::iterator it = ints.find(name);

	if (it != ints.end())
		return it->second;

	return def;
}

bool DataAbstraction::GetBool(std::string name, bool def)
{
	std::map<std::string, bool>::iterator it = bools.find(name);

	if (it != bools.end())
		return it->second;

	return def;
}

float DataAbstraction::GetFloat(std::string name, float def)
{
	std::map<std::string, float>::iterator it = floats.find(name);

	if (it != floats.end())
		return it->second;

	return def;
}

std::string DataAbstraction::GetString(std::string name, std::string def)
{
	std::map<std::string, std::string>::iterator it = strings.find(name);

	if (it != strings.end())
		return it->second;

	return def;
}

float3 DataAbstraction::GetFloat3(std::string name, float3 def)
{
	std::map<std::string, float3>::iterator it = floats3.find(name);

	if (it != floats3.end())
		return it->second;

	return def;
}

float4 DataAbstraction::GetFloat4(std::string name, float4 def)
{
	std::map<std::string, float4>::iterator it = floats4.find(name);

	if (it != floats4.end())
		return it->second;

	return def;
}

void DataAbstraction::Serialize(JSON_Doc doc)
{
	int counter = 0;
	if(ints.size() > 0)
		doc.SetNumber("ints_count", ints.size());

	for (std::map<std::string, int>::iterator it = ints.begin(); it != ints.end(); ++it)
	{
		std::string int_name = "int_name_" + std::to_string(counter);
		std::string int_value = "int_value_" + std::to_string(counter);

		doc.SetString(int_name, (*it).first.c_str());
		doc.SetNumber(int_value, (*it).second);

		++counter;
	}
	
	counter = 0;
	if(bools.size() > 0)
		doc.SetNumber("bools_count", bools.size());

	for (std::map<std::string, bool>::iterator it = bools.begin(); it != bools.end(); ++it)
	{
		std::string bool_name = "bool_name_" + std::to_string(counter);
		std::string bool_value = "bool_value_" + std::to_string(counter);

		doc.SetString(bool_name, (*it).first.c_str());
		doc.SetBool(bool_value, (*it).second);

		++counter;
	}

	counter = 0;
	if(floats.size() > 0)
		doc.SetNumber("floats_count", floats.size());

	for (std::map<std::string, float>::iterator it = floats.begin(); it != floats.end(); ++it)
	{
		std::string float_name = "float_name_" + std::to_string(counter);
		std::string float_number = "float_value_" + std::to_string(counter);

		doc.SetString(float_name, (*it).first.c_str());
		doc.SetNumber(float_number, (*it).second);
		++counter;
	}

	counter = 0;
	if(strings.size() > 0)
		doc.SetNumber("strings_count", strings.size());

	for (std::map<std::string, std::string>::iterator it = strings.begin(); it != strings.end(); ++it)
	{
		std::string string_name = "string_name_" + std::to_string(counter);
		std::string string_value = "string_value_" + std::to_string(counter);

		doc.SetString(string_name, (*it).first.c_str());
		doc.SetString(string_value, (*it).second.c_str());

		++counter;
	}

	counter = 0;
	if(floats3.size() > 0)
		doc.SetNumber("floats3_count", floats3.size());

	for (std::map<std::string, float3>::iterator it = floats3.begin(); it != floats3.end(); ++it)
	{
		std::string float3_name = "float3_name_" + std::to_string(counter);
		std::string float3_value = "float3_value_" + std::to_string(counter);

		doc.SetString(float3_name, (*it).first.c_str());
		doc.SetNumber3(float3_value, (*it).second);

		++counter;
	}

	counter = 0;
	if(floats4.size() > 0)
		doc.SetNumber("floats4_count", floats4.size());

	for (std::map<std::string, float4>::iterator it = floats4.begin(); it != floats4.end(); ++it)
	{
		std::string float4_name = "float4_name_" + std::to_string(counter);
		std::string float4_value = "float4_value_" + std::to_string(counter);

		doc.SetString(float4_name, (*it).first.c_str());
		doc.SetNumber4(float4_value, (*it).second);

		++counter;
	}
}

void DataAbstraction::DeSerialize(JSON_Doc doc)
{
	int ints_count = doc.GetNumber("ints_count", 0);
	int bools_count = doc.GetNumber("bools_count", 0);
	int floats_count = doc.GetNumber("floats_count", 0);
	int strings_count = doc.GetNumber("strings_count", 0);
	int floats3_count = doc.GetNumber("floats3_count", 0);
	int floats4_count = doc.GetNumber("floats4_count", 0);

	for (int i = 0; i < ints_count; ++i)
	{
		std::string int_name = "int_name_" + std::to_string(i);
		std::string int_value = "int_value_" + std::to_string(i);

		std::string name = doc.GetString(int_name.c_str());
		int val = doc.GetNumber(int_value.c_str());

		AddInt(name, val);
	}

	for (int i = 0; i < bools_count; ++i)
	{
		std::string bool_name = "bool_name_" + std::to_string(i);
		std::string bool_value = "bool_value_" + std::to_string(i);

		std::string name = doc.GetString(bool_name.c_str());
		bool val = doc.GetBool(bool_value.c_str());

		AddBool(name, val);
	}

	for (int i = 0; i < floats_count; ++i)
	{
		std::string float_name = "float_name_" + std::to_string(i);
		std::string float_value = "float_value_" + std::to_string(i);

		std::string name = doc.GetString(float_name.c_str());
		float val = doc.GetNumber(float_value.c_str());

		AddFloat(name, val);
	}

	for (int i = 0; i < strings_count; ++i)
	{
		std::string string_name = "string_name_" + std::to_string(i);
		std::string string_value = "string_value_" + std::to_string(i);

		std::string name = doc.GetString(string_name.c_str());
		std::string val = doc.GetString(string_value.c_str());

		AddString(name, val);
	}

	for (int i = 0; i < floats3_count; ++i)
	{
		std::string float3_name = "float3_name_" + std::to_string(i);
		std::string float3_value = "float3_value_" + std::to_string(i);

		std::string name = doc.GetString(float3_name.c_str());
		float3 val = doc.GetNumber3(float3_value.c_str());

		AddFloat3(name, val);
	}

	for (int i = 0; i < floats4_count; ++i)
	{
		std::string float4_name = "float4_name_" + std::to_string(i);
		std::string float4_value = "float4_value_" + std::to_string(i);

		std::string name = doc.GetString(float4_name.c_str());
		float4 val = doc.GetNumber4(float4_value.c_str());

		AddFloat4(name, val);
	}
}
