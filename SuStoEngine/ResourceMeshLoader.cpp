#include "ResourceMeshLoader.h"
#include "App.h"
#include "ModuleGameObject.h"
#include "ModuleCamera3D.h"
#include "ResourceManager.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "Globals.h"
#include "GameObject.h"
#include "Functions.h"
#include "ModuleFileSystem.h"
#include "ResourceTextureLoader.h"
#include "JSONLoader.h"
#include "SceneManager.h"
#include "Assimp\include\cimport.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")


ResourceMeshLoader::ResourceMeshLoader() : 
ResourceLoader(ResourceType::RT_MESH, App->file_system->GetAssetsPath(), App->file_system->GetLibraryMeshPath())
{
	
}

ResourceMeshLoader::~ResourceMeshLoader()
{
}

Resource * ResourceMeshLoader::CreateResource(std::string new_uid)
{
	Resource* ret = nullptr;

	ret = new ResourceMesh(new_uid);

	return ret;
}

bool ResourceMeshLoader::LoadFileToEngine(DecomposedFilePath d_filepath, std::vector<Resource*>& resources)
{
	bool ret = true;

	const aiScene* scene = aiImportFile(d_filepath.file_path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene == nullptr)
		ret = false;

	if (ret && !scene->HasMeshes())
	{
		CONSOLE_LOG("WARNING, scene has no meshes! Path: %s", d_filepath.file_path.c_str());
		ret = false;
	}

	if (ret)
	{
		aiNode* root = scene->mRootNode;

		// Root transform
		float3 position(0, 0, 0);
		Quat rotation(0, 0, 0, 0);
		float3 scale(0, 0, 0);

		aiVector3D aitranslation;
		aiVector3D aiscaling;
		aiQuaternion airotation;

		if (root != nullptr)
		{
			root->mTransformation.Decompose(aiscaling, airotation, aitranslation);
			position = float3(aitranslation.x, aitranslation.y, aitranslation.z);
			scale = float3(aiscaling.x, aiscaling.y, aiscaling.z);
			rotation = Quat(airotation.x, airotation.y, airotation.z, airotation.w);
		}

		// Create root go
		GameObject* parent = nullptr;

		parent = App->gameobj->Create();
		parent->transform->SetPosition(float3(position.x, position.y, position.z));
		parent->transform->SetRotation(Quat(rotation.x, rotation.y, rotation.w, rotation.z));
		parent->transform->SetScale(float3(scale.x, scale.y, scale.z));

		string name = App->file_system->GetFileNameFromFilePath(d_filepath.file_path.c_str());
		name = App->file_system->GetFilenameWithoutExtension(name.c_str());
		parent->SetName(d_filepath.file_name.c_str());

		// Total mesh bbox
		AABB total_abb;
		total_abb.SetNegativeInfinity();

		// Keep track of resources loaded (avoid repeating)

		// Iterate
		for (int i = 0; i < root->mNumChildren; i++)
		{
			RecursiveLoadMesh(scene, root->mChildren[i], d_filepath.file_path.c_str(), total_abb, resources, parent);
		}

		used_resources.clear();

		// Set camera focus
		App->camera->GetEditorCamera()->Focus(total_abb);

		// Create fbx prefab
		//App->scene_manager->SavePrefab((d_filepath.file_name + "." + d_filepath.file_extension).c_str(), "prefab", assets_path.c_str(), parent);

		// Crate meta
		std::string meta_path = (assets_path + d_filepath.file_name + "." + d_filepath.file_extension + ".meta");
		JSON_Doc* doc = App->json->CreateJSON(meta_path.c_str());
		if (doc != nullptr)
		{
			std::string uid = doc->GetString("uid", "no_uid");
			doc->SetArray("resources");

			for (std::vector<Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
			{
				doc->AddStringToArray("resources", (*it)->GetUniqueId().c_str());
			}

			doc->Save();
			App->json->UnloadJSON(doc);
		}
	}

	return ret;
}
bool ResourceMeshLoader::UnloadAssetFromEngine(DecomposedFilePath d_filepath)
{
	bool ret = true;

	std::string prefab_file = d_filepath.file_path + ".prefab";
	std::string meta_file = d_filepath.file_path + ".meta";

	App->file_system->FileDelete(d_filepath.file_path.c_str());
	App->file_system->FileDelete(prefab_file.c_str());

	JSON_Doc* doc = App->json->LoadJSON(meta_file.c_str());

	if (doc != nullptr)
	{
		for (int i = 0; i < doc->GetArrayCount("resources"); ++i)
		{
			std::string uid = doc->GetStringFromArray("resources", i);

			Resource* res = App->resource_manager->Get(uid);

			if (res != nullptr)
			{
				App->resource_manager->ClearResourceFromGameObjects(res);
			}

			App->resource_manager->DeleteResource(uid);

			std::string resource_path = library_path + uid + ".sustomesh";
			std::string meta_path = library_path + uid + ".meta";

			App->file_system->FileDelete(resource_path.c_str());
			App->file_system->FileDelete(meta_path.c_str());
		}
	}

	App->file_system->FileDelete(meta_file.c_str());

	return ret;
}
bool ResourceMeshLoader::ExportAssetToLibrary(DecomposedFilePath decomposed_file_path)
{
	return false;
}
void ResourceMeshLoader::ClearFromGameObject(Resource * resource, GameObject * go)
{
	if (go != nullptr)
	{
		ComponentMesh* mesh = (ComponentMesh*)go->GetComponent(ComponentType::MESH);

		if (mesh != nullptr)
		{
			if (mesh->GetMesh() == resource)
			{
				mesh->RemoveMesh();
			}
		}
	}
}
bool ResourceMeshLoader::ExportResourceToLibrary(Resource * resource)
{
	bool ret = true;

	if (resource != nullptr)
	{
		ResourceMesh* mesh = (ResourceMesh*)resource;

		string name = mesh->GetUniqueId();

		// -------------------------------------
		// FILE --------------------------------
		// -------------------------------------
		uint ranges[3] = { mesh->GetNumVertices(), mesh->GetNumIndices(), mesh->GetNumUVs() };
		uint size = sizeof(ranges) +
			sizeof(uint) * mesh->GetNumIndices() +
			sizeof(float) * mesh->GetNumVertices() * 3 +
			sizeof(float) * mesh->GetNumUVs() * 3;

		// Allocate data
		char* data = new char[size];
		char* cursor = data;

		// Store ranges
		uint bytes = sizeof(ranges);
		memcpy(cursor, ranges, bytes);
		cursor += bytes;

		// Store indices
		bytes = sizeof(uint) * mesh->GetNumIndices();
		memcpy(cursor, mesh->GetIndices(), bytes);
		cursor += bytes;

		// Store vertices
		bytes = sizeof(float) * mesh->GetNumVertices() * 3;
		memcpy(cursor, mesh->GetVertices(), bytes);
		cursor += bytes;

		// Store UVs
		bytes = sizeof(float) * mesh->GetNumUVs() * 3;
		memcpy(cursor, mesh->GetUVs(), bytes);

		//fopen
		if (App->file_system->FileSave(library_path.c_str(), data, name.c_str(), "sustomesh", size) == false)
		{
			RELEASE_ARRAY(data);
			return false;
		}

		RELEASE_ARRAY(data);

		// -------------------------------------
		// META --------------------------------
		// -------------------------------------
		string meta_name = library_path + name + ".meta";

		JSON_Doc* doc = App->json->LoadJSON(meta_name.c_str());
		if (doc == nullptr)
			doc = App->json->CreateJSON(meta_name.c_str());

		if (doc != nullptr)
		{
			doc->Clear();

			doc->SetString("uid", mesh->GetUniqueId().c_str());
			doc->SetString("name", mesh->GetFileName().c_str());

			doc->Save();
		}

		App->json->UnloadJSON(doc);

	}

	return ret;
}
bool ResourceMeshLoader::ImportResourceFromLibrary(const char * uid)
{
	bool ret = true;

	// -------------------------------------
	// META --------------------------------
	// -------------------------------------
	string meta_name = library_path + std::string(uid) + ".meta";
	JSON_Doc* doc = App->json->LoadJSON(meta_name.c_str());
	if (doc != nullptr)
	{
		string uid = doc->GetString("uid", "no_uid");
		string resource_name = doc->GetString("name");

		// -------------------------------------
		// FILE --------------------------------
		// -------------------------------------
		//Open the file and get the size
		FILE* file = fopen((std::string(uid) + ".sustomesh").c_str(), "rb");
		fseek(file, 0, SEEK_END);
		uint size = ftell(file);
		rewind(file);

		// Create a buffer to get the data of the file
		char* buffer = new char[size];
		char* cursor = buffer;

		// Read the file and close it
		fread(buffer, sizeof(char), size, file);
		fclose(file);

		// Copy the ranges
		// ranges[0] = Vertices, ranges[1] = Indices, ranges[2] = Uvs
		uint ranges[3];
		uint bytes = sizeof(ranges);
		memcpy(ranges, cursor, bytes);
		cursor += bytes;

		// Store indices
		uint* indices = new uint[ranges[1]];
		bytes = sizeof(uint) * ranges[1];
		memcpy(indices, cursor, bytes);
		cursor += bytes;

		// Store vertices
		float* vertices = new float[ranges[0] * 3];
		bytes = sizeof(float) * ranges[0] * 3;
		memcpy(vertices, cursor, bytes);
		cursor += bytes;

		// Store UVs
		float* uvs = new float[ranges[2] * 3];
		bytes = sizeof(float) * ranges[2] * 3;
		memcpy(uvs, cursor, bytes);
		cursor += bytes;

		// Create mesh --------------
		ResourceMesh* new_mesh = (ResourceMesh*)App->resource_manager->CreateNewResource(RT_MESH, uid);
		new_mesh->SetFaces(vertices, ranges[0], indices, ranges[1]);
		new_mesh->SetUvs(uvs, ranges[2]);

		new_mesh->SetFileName(resource_name.c_str());

		RELEASE_ARRAY(buffer);
		RELEASE_ARRAY(indices);
		RELEASE_ARRAY(vertices);
		RELEASE_ARRAY(uvs);
		App->json->UnloadJSON(doc);
	}

	return true;
}
bool ResourceMeshLoader::LoadAssetResourceIntoScene(DecomposedFilePath decomposed_file_path)
{
	bool ret = true;

	std::string prefab_path = decomposed_file_path.file_path + ".prefab";

	//App->scene_manager->LoadPrefab(prefab_path.c_str());

	return ret;
}
bool ResourceMeshLoader::IsAssetOnLibrary(DecomposedFilePath d_filepath)
{
	bool ret = false;

	std::string meta_file = d_filepath.file_path + ".meta";

	JSON_Doc* doc = App->json->LoadJSON(meta_file.c_str());
	if(doc != nullptr)
	{
		int resources_count = doc->GetArrayCount("resources");
		int correct = 0;
		for (int i = 0; i < resources_count; ++i)
		{
			std::string uid = doc->GetStringFromArray("resources", i);

			std::string resource_path = library_path + uid + ".sustomesh";
			std::string meta_path = library_path + uid + ".meta";

			if (App->file_system->FileExists(resource_path.c_str()) && App->file_system->FileExists(meta_path.c_str()))
			{
				++correct;
			}
		}

		if (correct == resources_count)
			ret = true;
	}
	
	return ret;
}


void ResourceMeshLoader::RecursiveLoadMesh(const aiScene * scene, aiNode * node, const char * full_path, AABB & total_abb, 
	vector<Resource*>& resources, GameObject * parent)
{
	bool node_valid = true;

	if (node->mNumMeshes == 0)
		node_valid = false;

	aiMesh* aimesh = nullptr;
	ResourceMesh* mesh = nullptr;
	GameObject* go = nullptr;
	string name = node->mName.C_Str();

	for (int i = 0; i < node->mNumMeshes; i++)
	{
		bool mesh_valid = true;

		int mesh_index = node->mMeshes[i];
		aimesh = scene->mMeshes[mesh_index];

		// Check if its already loaded
		Resource* res_mesh = nullptr;
		bool mesh_already_loaded = false;
		if (ResourceIsUsed(mesh_index, RT_MESH, res_mesh))
		{
			mesh = (ResourceMesh*)res_mesh;
			mesh_already_loaded = true;
		}

		if (mesh_valid && node_valid && !mesh_already_loaded)
		{
			mesh = (ResourceMesh*)App->resource_manager->CreateNewResource(RT_MESH);
			mesh->SetFileName(name.c_str());

			if (!aimesh->HasFaces())
			{
				CONSOLE_LOG("WARNING, geometry has no faces!");
				mesh_valid = false;
			}
		}

		// VERTICES && INDICES
		if (mesh_valid && node_valid && !mesh_already_loaded)
		{
			float* vertices = new float[aimesh->mNumVertices * 3];
			memcpy(vertices, aimesh->mVertices, sizeof(float) * aimesh->mNumVertices * 3);

			uint* indices = new uint[aimesh->mNumFaces * 3];

			for (uint i = 0; i < aimesh->mNumFaces && mesh_valid; ++i)
			{
				if (aimesh->mFaces[i].mNumIndices == 3)
				{
					memcpy(&indices[i * 3], aimesh->mFaces[i].mIndices, 3 * sizeof(uint));
				}
				else
				{
					CONSOLE_LOG("WARNING, geometry face with != 3 indices!");
					mesh_valid = false;
				}
			}

			mesh->SetFaces(vertices, aimesh->mNumVertices, indices, aimesh->mNumFaces * 3);

			RELEASE_ARRAY(vertices);
			RELEASE_ARRAY(indices);
		}

		// UVS
		if (mesh_valid && node_valid && !mesh_already_loaded && aimesh->HasTextureCoords(0))
		{
			float* uvs = new float[aimesh->mNumVertices * 3];
			memcpy(uvs, (float*)aimesh->mTextureCoords[0], sizeof(float) * aimesh->mNumVertices * 3);

			mesh->SetUvs(uvs, aimesh->mNumVertices);

			RELEASE_ARRAY(uvs);
		}

		// POSITION, ROTATION AND SCALE
		float3 position = float3::zero;
		Quat rotation = Quat::identity;
		float3 scale = float3::one;

		if (mesh_valid && node_valid)
		{
			aiVector3D aitranslation;
			aiVector3D aiscaling;
			aiQuaternion airotation;

			node->mTransformation.Decompose(aiscaling, airotation, aitranslation);
			position = float3(aitranslation.x, aitranslation.y, aitranslation.z);
			scale = float3(aiscaling.x, aiscaling.y, aiscaling.z);
			rotation = Quat(airotation.x, airotation.y, airotation.z, airotation.w);

			mesh->SetTransform(position, rotation, scale);
		}

		// GENERAL BBOX
		if (mesh_valid && node_valid)
		{
			AABB mesh_with_scale = mesh->GetBBox();
			mesh_with_scale.Scale(position, scale);

			total_abb.Enclose(mesh_with_scale);
		}

		//// MATERIALS
		//ResourceTexture* texture = nullptr;
		//if (mesh_valid && node_valid)
		//{
		//	// Check if its already loaded
		//	Resource* res_tex = nullptr;
		//	bool texture_already_loaded = false;
		//	if (ResourceIsUsed(aimesh->mMaterialIndex, RT_TEXTURE, res_tex))
		//	{
		//		texture = (ResourceTexture*)res_tex;
		//		texture_already_loaded = true;
		//	}

		//	if (!texture_already_loaded)
		//	{
		//		aiMaterial* material = scene->mMaterials[aimesh->mMaterialIndex];

		//		string path = App->file_system->GetPathFromFilePath(full_path);

		//		// Difuse -------------------
		//		aiString file;
		//		material->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		//		path += App->file_system->GetFileNameFromFilePath(file.C_Str());

		//		vector<Resource*> tex;
		//		App->resource_manager->LoadResource(path.c_str(), tex);
		//		if (!tex.empty())
		//		{
		//			texture = (ResourceTexture*)*tex.begin();
		//			AddResource(aimesh->mMaterialIndex, RT_TEXTURE, texture);
		//		}
		//	}
		//}

		// CREATE GAME OBJECT
		if (mesh_valid && node_valid && parent != nullptr)
		{
			go = App->gameobj->Create();

			if (name == "")
				name = "no_name";

			go->SetName(name);

			parent->AddChild(go);

			go->transform->SetPosition(mesh->GetPosition());
			go->transform->SetRotation(mesh->GetRotation());
			go->transform->SetScale(mesh->GetScale());

			go->AddComponent(MESH);
			ComponentMesh* cmesh = (ComponentMesh*)go->GetComponent(MESH);
			cmesh->SetMesh(mesh);

		/*	if (texture != nullptr)
			{
				go->AddComponent(MATERIAL);
				ComponentMaterial* cmaterial = (ComponentMaterial*)go->GetComponent(MATERIAL);
				cmaterial->SetTexture(texture);
			}*/
		}

		if (mesh_valid && node_valid && !mesh_already_loaded && mesh != nullptr)
		{
			ExportResourceToLibrary(mesh);

			AddResource(mesh_index, RT_MESH, mesh);

			resources.push_back(mesh);
		}
		else if (!mesh_valid && !mesh_already_loaded && mesh != nullptr)
			App->resource_manager->DeleteResource(mesh->GetUniqueId());
	}

	// Select parent
	GameObject* pare = nullptr;
	if (node_valid && go != nullptr)
		pare = go;
	else
		pare = parent;

	// RECURSE
	for (int i = 0; i < node->mNumChildren; i++)
	{
		RecursiveLoadMesh(scene, node->mChildren[i], full_path, total_abb, resources, pare);
	}
}

bool ResourceMeshLoader::ResourceIsUsed(int index, ResourceType type, Resource*& res)
{
	bool ret = false;

	for (vector<UsedResource>::iterator it = used_resources.begin(); it != used_resources.end(); it++)
	{
		if ((*it).GetIndex() == index && (*it).GetType() == type)
		{
			res = (*it).GetResource();
			ret = true;
			break;
		}
	}

	return ret;
}

void ResourceMeshLoader::AddResource(int index, ResourceType type, Resource * res)
{
	UsedResource used(res, index, type);
	used_resources.push_back(used);
}

//void ResourceMeshLoader::ImportAllMeshes()
//{
//	vector<string> files = App->file_system->GetFilesInPath(App->file_system->GetLibraryMeshPath().c_str(), "sustomesh");
//
//	for (vector<string>::iterator it = files.begin(); it != files.end(); it++)
//	{
//		Import((*it).c_str());
//	}
//}
//
//
//void ResourceMeshLoader::Import(const char * filepath)
//{
//	string path = App->file_system->GetPathFromFilePath(filepath);
//	string filename = App->file_system->GetFileNameFromFilePath(filepath);
//	string name = App->file_system->GetFilenameWithoutExtension(filename.c_str());
//
//	// -------------------------------------
//	// META --------------------------------
//	// -------------------------------------
//	string meta_name = path + name + ".meta";
//	JSON_Doc* doc = App->json->LoadJSON(meta_name.c_str());
//	string uid = doc->GetString("uid", "no_uid");
//	string resource_name = doc->GetString("name");
//
//	// -------------------------------------
//	// FILE --------------------------------
//	// -------------------------------------
//	//Open the file and get the size
//	FILE* file = fopen(filepath, "rb");
//	fseek(file, 0, SEEK_END);
//	uint size = ftell(file);
//	rewind(file);
//
//	// Create a buffer to get the data of the file
//	char* buffer = new char[size];
//	char* cursor = buffer;
//
//	// Read the file and close it
//	fread(buffer, sizeof(char), size, file);
//	fclose(file);
//
//	// Copy the ranges
//	// ranges[0] = Vertices, ranges[1] = Indices, ranges[2] = Uvs
//	uint ranges[3];
//	uint bytes = sizeof(ranges);
//	memcpy(ranges, cursor, bytes);
//	cursor += bytes;
//
//	// Store indices
//	uint* indices = new uint[ranges[1]];
//	bytes = sizeof(uint) * ranges[1];
//	memcpy(indices, cursor, bytes);
//	cursor += bytes;
//
//	// Store vertices
//	float* vertices = new float[ranges[0] * 3];
//	bytes = sizeof(float) * ranges[0] * 3;
//	memcpy(vertices, cursor, bytes);
//	cursor += bytes;
//
//	// Store UVs
//	float* uvs = new float[ranges[2] * 3];
//	bytes = sizeof(float) * ranges[2] * 3;
//	memcpy(uvs, cursor, bytes);
//	cursor += bytes;
//
//	// Create mesh --------------
//	ResourceMesh* new_mesh = (ResourceMesh*)App->resource_manager->CreateNewResource(RT_MESH, uid);
//	new_mesh->SetFaces(vertices, ranges[0], indices, ranges[1]);
//	new_mesh->SetUvs(uvs, ranges[2]);
//
//	new_mesh->SetFileName(resource_name.c_str());
//
//	RELEASE_ARRAY(buffer);
//	RELEASE_ARRAY(indices);
//	RELEASE_ARRAY(vertices);
//	RELEASE_ARRAY(uvs);
//	App->json->UnloadJSON(doc);
//}
//
//bool ResourceMeshLoader::Export(const char * path, ResourceMesh* mesh)
//{
//	bool ret = true;
//
//	string name = mesh->GetUniqueId();
// 
//	// -------------------------------------
//	// FILE --------------------------------
//	// -------------------------------------
//	uint ranges[3] = { mesh->GetNumVertices(), mesh->GetNumIndices(), mesh->GetNumUVs() };
//	uint size = sizeof(ranges) +
//		sizeof(uint) * mesh->GetNumIndices() +
//		sizeof(float) * mesh->GetNumVertices() * 3 +
//		sizeof(float) * mesh->GetNumUVs() * 3;
//
//	// Allocate data
//	char* data = new char[size];
//	char* cursor = data;
//
//	// Store ranges
//	uint bytes = sizeof(ranges);
//	memcpy(cursor, ranges, bytes);
//	cursor += bytes;
//
//	// Store indices
//	bytes = sizeof(uint) * mesh->GetNumIndices();
//	memcpy(cursor, mesh->GetIndices(), bytes);
//	cursor += bytes;
//
//	// Store vertices
//	bytes = sizeof(float) * mesh->GetNumVertices() * 3;
//	memcpy(cursor, mesh->GetVertices(), bytes);
//	cursor += bytes;
//
//	// Store UVs
//	bytes = sizeof(float) * mesh->GetNumUVs() * 3;
//	memcpy(cursor, mesh->GetUVs(), bytes);
//
//	//fopen
//	if (App->file_system->FileSave(path, data, name.c_str(), "sustomesh", size) == false)
//	{
//		RELEASE_ARRAY(data);
//		return false;
//	}
//
//	RELEASE_ARRAY(data);
//
//	// -------------------------------------
//	// META --------------------------------
//	// -------------------------------------
//	string meta_name = path + name + ".meta";
//
//	JSON_Doc* doc = App->json->LoadJSON(meta_name.c_str());
//	if (doc == nullptr)
//		doc = App->json->CreateJSON(meta_name.c_str());
//
//	if (doc != nullptr)
//	{
//		doc->Clear();
//
//		doc->SetString("uid", mesh->GetUniqueId().c_str());
//		doc->SetString("name", mesh->GetFileName().c_str());
//
//		doc->Save();
//	}
//
//	App->json->UnloadJSON(doc);
//
//	return ret;
//}
//
//void ResourceMeshLoader::LoadIntoScene(const char * filepath)
//{
//	string path = App->file_system->GetPathFromFilePath(filepath);
//	string filename = App->file_system->GetFileNameFromFilePath(filepath);
//	string extension = ToLowerCase(App->file_system->GetFileExtension(filename.c_str()));
//	string name = App->file_system->GetFilenameWithoutExtension(filename.c_str());
//
//	string prefab_path = filepath;
//	prefab_path += ".prefab";
//
//	GameObject* loaded_go = nullptr;
//	App->scene_manager->LoadPrefab(prefab_path.c_str(), loaded_go);
//}
//
//void ResourceMeshLoader::Unload(const char * filepath)
//{
//	string path = App->file_system->GetPathFromFilePath(filepath);
//	string filename = App->file_system->GetFileNameFromFilePath(filepath);
//	string extension = App->file_system->GetFileExtension(filename.c_str());
//	string name = App->file_system->GetFilenameWithoutExtension(filename.c_str(), false);
//
//	string meta_path = path + filename + ".meta";
//	string prefab_path = path + filename + ".prefab";
//
//	JSON_Doc* meta = App->json->LoadJSON(meta_path.c_str());
//
//	if (meta != nullptr)
//	{
//		int resources_count = meta->GetArrayCount("resources");
//
//		for (int i = 0; i < resources_count; i++)
//		{
//			string res_uid = meta->GetStringFromArray("resources", i);
//
//			string resource_path = App->file_system->GetLibraryTexturePath() + res_uid + ".sustomesh";
//			string resource_meta_path = App->file_system->GetLibraryTexturePath() + res_uid + ".meta";
//
//			App->gameobj->DeleteGameObjectsUsingResource(App->resource_manager->Get(res_uid));
//
//			App->file_system->FileDelete(resource_path.c_str());
//			App->file_system->FileDelete(resource_meta_path.c_str());
//		}
//	}
//
//	App->file_system->FileDelete(meta_path.c_str());
//	App->file_system->FileDelete(prefab_path.c_str());
//	App->file_system->FileDelete(filepath);
//}
//
//ResourceMesh * ResourceMeshLoader::CreatePlaneMesh(float2 size)
//{
//	float length = size.y;
//	float width = size.x;
//	int resX = 2; // 2 minimum
//	int resZ = 2;
//
//	//vertices
//	uint num_vert = resX*resZ;
//	float3 ver[4];
//	for (int z = 0; z < resZ; z++)
//	{
//		// [ -length / 2, length / 2 ]
//		float zPos = ((float)z / (resZ - 1) - .5f) * length;
//		for (int x = 0; x < resX; x++)
//		{
//			// [ -width / 2, width / 2 ]
//			float xPos = ((float)x / (resX - 1) - .5f) * width;
//			ver[x + z * resX] = float3(xPos, 0.f, zPos);
//		}
//	}
//
//	float* vertices = new float[num_vert * 3];
//	for (int i = 0; i < num_vert; ++i)
//	{
//		memcpy(vertices + i * 3, &ver[i].x, sizeof(float) * 3);
//	}
//
//	//indices
//	uint num_indices = (resX - 1) * (resZ - 1) * 6;
//	uint ind[6];
//	int t = 0;
//	for (int face = 0; face < num_indices / 6; ++face)
//	{
//		int i = face % (resX - 1) + (face / (resZ - 1) * resX);
//
//		ind[t++] = i + resX;
//		ind[t++] = i + 1;
//		ind[t++] = i;
//
//		ind[t++] = i + resX;
//		ind[t++] = i + resX + 1;
//		ind[t++] = i + 1;
//	}
//	uint* indices = new uint[num_indices];
//	memcpy(indices, ind, sizeof(uint)*num_indices);
//
//	//uv
//	float3 uv[4];
//	for (int v = 0; v < resZ; v++)
//	{
//		for (int u = 0; u < resX; u++)
//		{
//			uv[u + v * resX] = float3((float)u / (resX - 1), (float)v / (resZ - 1), 0.f);
//		}
//	}
//
//	float* uvs = new float[num_vert * 3];
//	for (int i = 0; i < num_vert; ++i)
//	{
//		memcpy(uvs + i * 3, &uv[i].x, sizeof(float) * 3);
//	}
//
//	ResourceMesh* rmesh = (ResourceMesh*)App->resource_manager->CreateNewResource(ResourceType::RT_MESH);
//	rmesh->SetFaces(vertices, num_vert, indices, num_indices);
//	rmesh->SetUvs(uvs, num_vert);
//
//	RELEASE_ARRAY(vertices);
//	RELEASE_ARRAY(indices);
//	RELEASE_ARRAY(uvs);
//
//	return rmesh;
//}

