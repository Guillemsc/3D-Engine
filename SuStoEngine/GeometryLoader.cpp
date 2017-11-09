#include "GeometryLoader.h"
#include "App.h"
#include "ModuleRenderer3D.h"
#include "Functions.h"
#include "GameObject.h"
#include "ModuleGameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ModuleCamera3D.h"
#include "ModuleFileSystem.h"
#include "TextureLoader.h"
#include "ResourceManager.h"
#include "ResourceMesh.h"
#include <filesystem>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Assimp\include\cimport.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

GeometryLoader::GeometryLoader(bool start_enabled) : Module(start_enabled)
{
	SetName("Geometry Loader");
}

GeometryLoader::~GeometryLoader()
{
}

bool GeometryLoader::Awake()
{
	bool ret = true;

	LOG_OUTPUT("Loading GeometryLoader Module");

	mesh_importer = new MeshImporter();

	return ret;
}

bool GeometryLoader::Start()
{
	bool ret = true;

	// Stream log messages to Debug window
	//Assimp::DefaultLogger::create();
	//const uint severity = Assimp::Logger::Debugging | Assimp::Logger::Info | Assimp::Logger::Err | Assimp::Logger::Warn;
	//Assimp::DefaultLogger::get()->attachStream(new AssimpLogger(), severity);

	//struct aiLogStream stream;
	//stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	//aiAttachLogStream(&stream);

	mesh_importer->ImportAllMeshes();

	return ret;
}

bool GeometryLoader::Update()
{
	bool ret = true;

	return ret;
}

bool GeometryLoader::CleanUp()
{
	bool ret = true;

	// Detach log stream
	aiDetachAllLogStreams();

	// Free mesh importer
	RELEASE(mesh_importer);

	return ret;
}

void GeometryLoader::OnLoadFile(const char* file_path, const char* file_name, const char* file_extension)
{
	if (TextCmp("fbx", file_extension))
	{
		LoadFile(file_path, true);
	}
}

bool GeometryLoader::LoadFile(const char * full_path, bool as_new_gameobject)
{
	bool ret = true;

	LOG_OUTPUT("\nStarting mesh scene Loading -------------------- \n\n");
	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);
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

			string name = GetFileNameFromFilePath(full_path);
			name = GetFilenameWithoutExtension(name.c_str());
			parent->SetName(name);
		}

		// Total mesh bbox
		AABB total_abb;
		total_abb.SetNegativeInfinity();

		// Iterate
		for (int i = 0; i < root->mNumChildren; i++)
		{
			RecursiveLoadMesh(scene, root->mChildren[i], full_path, total_abb, parent);
		}

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

void GeometryLoader::RecursiveLoadMesh(const aiScene* scene, aiNode * node, const char* full_path, AABB& total_abb, GameObject* parent)
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
		if (mesh_valid && node_valid)
		{
			int mesh_index = node->mMeshes[i];
			aimesh = scene->mMeshes[mesh_index];

			mesh = (ResourceMesh*)App->resource_manager->CreateNewResource(RT_MESH);

			if (!aimesh->HasFaces())
			{
				LOG_OUTPUT("WARNING, geometry has no faces!");
				mesh_valid = false;
			}
		}

		// VERTICES && INDICES
		if (mesh_valid && node_valid)
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
		if (mesh_valid && node_valid && aimesh->HasTextureCoords(0))
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
			aiMaterial* material = scene->mMaterials[aimesh->mMaterialIndex];

			string path = GetPathFromFilePath(full_path);

			// Difuse -------------------
			aiString file;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &file);
			path += GetFileNameFromFilePath(file.C_Str());

			texture = App->texture->LoadTexture(path.c_str());
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

MeshImporter * GeometryLoader::GetMeshImporter()
{
	return mesh_importer;
}

ResourceMesh* MeshImporter::Load(const char * filepath)
{
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
	ResourceMesh* new_mesh = (ResourceMesh*)App->resource_manager->CreateNewResource(RT_MESH);
	new_mesh->SetFaces(vertices, ranges[0], indices, ranges[1]);
	new_mesh->SetUvs(uvs, ranges[2]);

	LOG_OUTPUT("New mesh with %d vertices", ranges[0] * 3);
	LOG_OUTPUT("New mesh with %d indices", ranges[1]);

	RELEASE_ARRAY(indices);
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(uvs);

	return new_mesh;
}

bool MeshImporter::Save(const char * path, ResourceMesh* mesh)
{
	bool ret = true;
	
	string name = GetFilenameWithoutExtension(mesh->GetName().c_str());
	name += "_";
	name += std::to_string(App->id->NewId("mesh"));

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
	
	return ret;
}

void MeshImporter::ImportAllMeshes()
{
	//vector<string> paths = App->file_system->GetFilesInPath(App->file_system->GetLibraryMeshPath().c_str(), "sustomesh");

	//for (vector<string>::iterator it = paths.begin(); it != paths.end(); it++)
	//{
	//	Mesh* new_mesh = Load((*it).c_str());
	//}
}

