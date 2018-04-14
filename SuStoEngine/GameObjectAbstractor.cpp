#include "App.h"
#include "ModuleGameObject.h"
#include "GameObjectAbstractor.h"
#include "GameObject.h"
#include "Component.h"

GameObjectAbstractor::GameObjectAbstractor()
{
}

GameObjectAbstractor::~GameObjectAbstractor()
{
}

GameObjectAbstraction GameObjectAbstractor::Abstract(GameObject * go)
{
	GameObjectAbstraction ret;

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

	return ret;
}

GameObject * GameObjectAbstractor::DeAbstract(GameObjectAbstraction abs)
{
	GameObject* ret = nullptr;

	if (abs.GetValid())
	{
		for (std::vector<IDRelation>::iterator it = abs.id_relations.begin(); it != abs.id_relations.end(); ++it)
		{

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
	IDRelation relation(++max_id, go);
	
	std::vector<Component*> components_list = go->GetComponents();

	for (std::vector<Component*>::iterator it = components_list.begin(); it != components_list.end(); ++it)
	{
		Component* curr = (*it);

		DataAbstraction comp_abs(curr->GetType());
		curr->OnSaveAbstraction(comp_abs);

		relation.data_components.push_back(comp_abs);
	}

	id_relations.push_back(relation);

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
