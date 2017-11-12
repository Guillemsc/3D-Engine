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
#include "JSONLoader.h"
#include "Assimp\include\cimport.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")


ResourceMeshLoader::ResourceMeshLoader()
{
}

ResourceMeshLoader::~ResourceMeshLoader()
{
}

bool ResourceMeshLoader::Load(const char * filepath, bool as_new_gameobject)
{
	bool ret = true;

	LOG_OUTPUT("\nStarting mesh scene Loading -------------------- \n\n");
	const aiScene* scene = aiImportFile(filepath, aiProcessPreset_TargetRealtime_MaxQuality);
	LOG_OUTPUT("Finishing mesh scene Loading ---------------------");

	if (scene == nullptr)
		ret = false;

	if (ret && !scene->HasMeshes())
	{
		LOG_OUTPUT("WARNING, scene has no meshes!");
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
		if (as_new_gameobject)
		{
			parent = App->gameobj->Create();
			parent->transform->SetPosition(float3(position.x, position.y, position.z));
			parent->transform->SetRotation(Quat(rotation.x, rotation.y, rotation.w, rotation.z));
			parent->transform->SetScale(float3(scale.x, scale.y, scale.z));

			string name = GetFileNameFromFilePath(filepath);
			name = GetFilenameWithoutExtension(name.c_str());
			parent->SetName(name);
		}

		// Total mesh bbox
		AABB total_abb;
		total_abb.SetNegativeInfinity();

		// Keep track of resources loaded (avoid repeating)

		// Iterate
		for (int i = 0; i < root->mNumChildren; i++)
		{
			RecursiveLoadMesh(scene, root->mChildren[i], filepath, total_abb, parent);
		}

		used_resources.clear();

		// Set camera focus
		if (ret)
		{
			App->camera->GetCurrentCamera()->Focus(total_abb.CenterPoint(), total_abb.Size().Length());
		}
	}

	// Release scene
	if (scene != nullptr)
		aiReleaseImport(scene);

	return ret;
}

void ResourceMeshLoader::RecursiveLoadMesh(const aiScene * scene, aiNode * node, const char * full_path, AABB & total_abb, GameObject * parent)
{
	bool node_valid = true;

	if (node->mNumMeshes == 0)
		node_valid = false;

	aiMesh* aimesh = nullptr;
	ResourceMesh* mesh = nullptr;
	GameObject* go = nullptr;

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

			if (!aimesh->HasFaces())
			{
				LOG_OUTPUT("WARNING, geometry has no faces!");
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
					LOG_OUTPUT("WARNING, geometry face with != 3 indices!");
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
		float3 position(0, 0, 0);
		Quat rotation(0, 0, 0, 0);
		float3 scale(0, 0, 0);

		if (mesh_valid && node_valid)
		{
			aiVector3D aitranslation;
			aiVector3D aiscaling;
			aiQuaternion airotation;

			node->mTransformation.Decompose(aiscaling, airotation, aitranslation);
			position = float3(aitranslation.x, aitranslation.y, aitranslation.z);
			scale = float3(aiscaling.x, aiscaling.y, aiscaling.z);
			rotation = Quat(airotation.x, airotation.y, airotation.z, airotation.w);

			mesh->SetTransform(
				float3(position.x, position.y, position.z),
				Quat(rotation.x, rotation.y, rotation.w, rotation.z),
				float3(scale.x, scale.y, scale.z));

		}

		// GENERAL BBOX
		if (mesh_valid && node_valid)
		{
			AABB mesh_with_scale = mesh->GetBBox();
			mesh_with_scale.Scale(position, scale);

			total_abb.Enclose(mesh_with_scale);
		}

		// MATERIALS
		ResourceTexture* texture = nullptr;
		if (mesh_valid && node_valid)
		{
			// Check if its already loaded
			Resource* res_tex = nullptr;
			bool texture_already_loaded = false;
			if (ResourceIsUsed(aimesh->mMaterialIndex, RT_TEXTURE, res_tex))
			{
				texture = (ResourceTexture*)res_tex;
				texture_already_loaded = true;
			}

			if (!texture_already_loaded)
			{
				aiMaterial* material = scene->mMaterials[aimesh->mMaterialIndex];

				string path = GetPathFromFilePath(full_path);

				// Difuse -------------------
				aiString file;
				material->GetTexture(aiTextureType_DIFFUSE, 0, &file);
				path += GetFileNameFromFilePath(file.C_Str());

				texture = (ResourceTexture*)App->resource_manager->LoadResource(path.c_str());

				AddResource(aimesh->mMaterialIndex, RT_TEXTURE, texture);
			}
		}

		// CREATE GAME OBJECT
		if (mesh_valid && node_valid && parent != nullptr)
		{
			go = App->gameobj->Create();

			string name = node->mName.C_Str();
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

			if (texture != nullptr)
			{
				go->AddComponent(MATERIAL);
				ComponentMaterial* cmaterial = (ComponentMaterial*)go->GetComponent(MATERIAL);
				cmaterial->SetTexture(texture);
			}
		}

		if (!mesh_valid || !node_valid)
		{
			RELEASE(mesh);
		}
		else if (!mesh_already_loaded)
		{
			AddResource(mesh_index, RT_MESH, mesh);
		}
		else
			App->resource_manager->SaveResourceIntoFile(mesh);
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
		RecursiveLoadMesh(scene, node->mChildren[i], full_path, total_abb, pare);
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

void ResourceMeshLoader::ImportAllMeshes()
{
	vector<string> files = App->file_system->GetFilesInPath(App->file_system->GetLibraryMeshPath().c_str(), "sustomesh");

	for (vector<string>::iterator it = files.begin(); it != files.end(); it++)
	{
		Import((*it).c_str());
	}
}


void ResourceMeshLoader::Import(const char * filepath)
{
	string path = GetPathFromFilePath(filepath);
	string filename = GetFileNameFromFilePath(filepath);
	string name = GetFilenameWithoutExtension(filename.c_str());

	// -------------------------------------
	// META --------------------------------
	// -------------------------------------
	string meta_name = path + name + ".meta";
	JSON_Doc* doc = App->json->LoadJSON(meta_name.c_str());
	string uid = doc->GetString("uid", "no_uid");

	// -------------------------------------
	// FILE --------------------------------
	// -------------------------------------
	//Open the file and get the size
	FILE* file = fopen(filepath, "rb");
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

	LOG_OUTPUT("New mesh with %d vertices", ranges[0] * 3);
	LOG_OUTPUT("New mesh with %d indices", ranges[1]);

	RELEASE_ARRAY(indices);
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(uvs);
	App->json->UnloadJSON(doc);
}

bool ResourceMeshLoader::Export(const char * path, ResourceMesh* mesh)
{
	bool ret = true;

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
	if (App->file_system->FileSave(path, data, name.c_str(), "sustomesh", size) == false)
	{
		return false;
	}

	RELEASE_ARRAY(data);

	// -------------------------------------
	// META --------------------------------
	// -------------------------------------
	string meta_name = path + name + ".meta";

	JSON_Doc* doc = App->json->LoadJSON(meta_name.c_str());
	if (doc == nullptr)
		doc = App->json->CreateJSON(meta_name.c_str());

	if (doc != nullptr)
	{
		doc->Clear();

		doc->SetString("uid", mesh->GetUniqueId().c_str());

		doc->Save();
	}

	App->json->UnloadJSON(doc);

	return ret;
}
