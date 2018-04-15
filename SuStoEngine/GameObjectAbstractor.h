#ifndef __GAME_OBJECT_ABSTRACTOR_H__
#define __GAME_OBJECT_ABSTRACTOR_H__

#include "Resource.h"
#include <string>
#include <map>
#include "Globals.h"
#include "GeometryMath.h"

enum ComponentType;
class GameObject;
class GameObjectAbstractor;
class GameObjectAbstraction;
class JSON_Doc;

class DataAbstraction
{
public:
	DataAbstraction() {}

	void Clear();

	void AddInt(std::string name, int val);
	void AddBool(std::string name, bool val);
	void AddFloat(std::string name, float val);
	void AddString(std::string name, std::string val);
	void AddFloat3(std::string name, float3 val);
	void AddFloat4(std::string name, float4 val);

	int GetInt(std::string name, int def = 0);
	bool GetBool(std::string name, bool def = false);
	float GetFloat(std::string name, float def = 0.0f);
	std::string GetString(std::string name, std::string def = "");
	float3 GetFloat3(std::string name, float3 def = float3::zero);
	float4 GetFloat4(std::string name, float4 def = float4::zero);

	void Serialize(JSON_Doc doc);
	void DeSerialize(JSON_Doc doc);

private:
	std::map<std::string, int> ints;
	std::map<std::string, bool> bools;
	std::map<std::string, float> floats;
	std::map<std::string, std::string> strings;
	std::map<std::string, float3> floats3;
	std::map<std::string, float4> floats4;
};

class IDRelation
{
	friend GameObjectAbstraction;
	friend GameObjectAbstractor;

public:
	IDRelation(){}
	IDRelation(int _id, GameObject* _go)
	{
		id = _id;
		go = _go;
	}

	int id = 0;
	std::vector<DataAbstraction> data_components;
	DataAbstraction data_game_object;

private:
	GameObject* go = nullptr;
};

class GORelation
{
	friend GameObjectAbstraction;

public:
	GORelation(int _id, int _id_parent)
	{
		id = _id;
		id_parent = _id_parent;
	};

	int id = 0;
	int id_parent = 0;
};

class GameObjectAbstraction
{
	friend GameObjectAbstractor;

public:
	GameObjectAbstraction();
	virtual ~GameObjectAbstraction();

	bool GetValid() const;
	void Clear();

private:
	int AddGo(GameObject* go);
	void AddRelation(int id, int parent_id);

	GameObject* GetGoFromId(int id);
	int GetIdFromGo(GameObject* go);

private:
	std::vector<GORelation> go_relations;
	std::vector<IDRelation> id_relations;
	int max_id = -1;

	bool valid = false;
};

class GameObjectAbstractor
{
public:
	GameObjectAbstractor();
	virtual ~GameObjectAbstractor();

	GameObjectAbstraction Abstract(GameObject* go);
	GameObject* DeAbstract(GameObjectAbstraction abs);
	void Serialize(GameObjectAbstraction abs, const char* path, const char* name, const char* extension = "json");
	GameObjectAbstraction DeSerialize(const char* filepath);
};

#endif //!__GAME_OBJECT_ABSTRACTOR_H__