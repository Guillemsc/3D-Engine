#include "GeometryLoader.h"
#include "App.h"
#include "ModuleRenderer3D.h"
#include "Functions.h"
#include "GameObject.h"
#include "ModuleGameObject.h"
#include "ComponentMesh.h"
#include "ModuleCamera3D.h"
#include "ModuleFileSystem.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
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

	return ret;
}

bool GeometryLoader::Start()
{
	bool ret = true;

	// Stream log messages to Debug window
	Assimp::DefaultLogger::create();
	const uint severity = Assimp::Logger::Debugging | Assimp::Logger::Info | Assimp::Logger::Err | Assimp::Logger::Warn;
	Assimp::DefaultLogger::get()->attachStream(new AssimpLogger(), severity);

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	LoadFile("Models/BakerHouse/BakerHouse.fbx", true);

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

	MeshImporter importer;
	importer.Save(App->file_system->library_mesh_path.c_str(), meshes);

	UnloadAllFiles();

	// Detach log stream
	aiDetachAllLogStreams();

	return ret;
}

void GeometryLoader::OnLoadFile(const char* file_path, const char* file_name, const char* file_extension)
{
	if (TextCmp("fbx", file_extension))
	{
		App->gameobj->DestroyAllGameObjects();
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
		LOG_OUTPUT("\nLOADING %d MESHES", scene->mNumMeshes);

		string file_name = GetFileNameFromFilePath(full_path);

		// Parent GameObject
		GameObject* parent = nullptr;

		if (as_new_gameobject)
		{
			parent = App->gameobj->Create();
			parent->SetName(file_name);

			aiVector3D translation;
			aiVector3D scaling;
			aiQuaternion rotation;
			if (scene->mRootNode != nullptr)
			{
				scene->mRootNode->mTransformation.Decompose(scaling, rotation, translation);
				float3 pos(translation.x, translation.y, translation.z);
				float3 scale(scaling.x, scaling.y, scaling.z);
				Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
			}

			parent->transform->SetPosition(float3(translation.x, translation.y, translation.z));
			parent->transform->SetRotation(Quat(rotation.x, rotation.y, rotation.w, rotation.z));
			//parent->transform->SetScale(float3(scaling.x, scaling.y, scaling.z));
		
		}

		// -------------------------------------------
		// LOAD MESH ---------------------------------
		// -------------------------------------------
		for (int i = 0; i < scene->mNumMeshes && ret; ++i)
		{
			aiMesh* current_mesh = scene->mMeshes[i];

			uint* indices = new uint[current_mesh->mNumFaces * 3];

			// Load indices from faces ---------------
			if (!current_mesh->HasFaces())
			{
				LOG_OUTPUT("WARNING, geometry has no faces!");
				ret = false;
			}

			// Assume each face is a triangle
			for (uint i = 0; i < current_mesh->mNumFaces && ret; ++i)
			{
				if (current_mesh->mFaces[i].mNumIndices == 3)
				{
					memcpy(&indices[i * 3], current_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
				}
				else
				{
					LOG_OUTPUT("WARNING, geometry face with != 3 indices!");
					ret = false;
				}
			}
			

			// Create mesh --------------
			if (ret)
			{
				Mesh* new_mesh = new Mesh(
					(float*)current_mesh->mVertices, current_mesh->mNumVertices,
					indices, current_mesh->mNumFaces * 3,
					(float*)current_mesh->mTextureCoords[0], current_mesh->mNumVertices,
					file_name.c_str());

				new_mesh->LoadToMemory();

				meshes.push_back(new_mesh);

				LOG_OUTPUT("New mesh with %d vertices", current_mesh->mNumVertices);
				LOG_OUTPUT("New mesh with %d indices", current_mesh->mNumFaces * 3);

				// Create GameObjects
				if (as_new_gameobject)
				{
					string name = file_name; name += "_"; name += std::to_string(i);

					GameObject* go = App->gameobj->Create();
					go->SetName(name.c_str());
					go->AddComponent(MESH);
					ComponentMesh* component = (ComponentMesh*)go->FindComponentByType(MESH);
					component->SetMesh(new_mesh);

					parent->AddChild(go);

					// Set mesh pos, rot and scale
					aiVector3D translation;
					aiVector3D scaling;
					aiQuaternion rotation;

					aiNode* node = scene->mRootNode->mChildren[i];
					if (node != nullptr)
					{
						node->mTransformation.Decompose(scaling, rotation, translation);
						float3 pos(translation.x, translation.y, translation.z);
						float3 scale(scaling.x, scaling.y, scaling.z);
						Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
					}
					
					go->transform->SetPosition(float3(translation.x, translation.y, translation.z));
					go->transform->SetRotation(Quat(rotation.x, rotation.y, rotation.w, rotation.z));
					//go->transform->SetScale(float3(scaling.x, scaling.y, scaling.z));
				}
			}

			delete[] indices;
		}

		// -------------------------------------------
		// LOAD TEXTURES -----------------------------
		// -------------------------------------------
		if (ret)
		{
			aiMaterial* material = scene->mMaterials[0];

			string path = GetPathFromFilePath(full_path);

			// Difuse -------------------
			aiString file;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &file);
			path += file.C_Str();

			App->LoadFile(path.c_str());
		}

		// -------------------------------------------
		// RELEASE -----------------------------------
		// -------------------------------------------
		if(scene != nullptr)
			aiReleaseImport(scene);

		// CUSTOM GAME OBJECT BEHAVEOUR FOR THIS ASSIGNMENT
		if (ret)
		{
			vector<GameObject*> gobjects = App->gameobj->GetListGameObjects();

			float max_size = 0;
			for (vector<GameObject*>::iterator it = gobjects.begin(); it != gobjects.end(); ++it)
			{
				ComponentMesh* cmesh = (ComponentMesh*)(*it)->FindComponentByType(MESH);

				if (cmesh != nullptr)
				{
					float size = cmesh->GetMesh()->GetBBox().Size().Length();

					if (size > max_size)
						max_size = size;
				}
			}

			App->camera->GetEditorCamera()->Focus(float3(0, 0, 0), max_size*1.2f);
		}
		// ------------------------------------------------
	}
	
	if(!ret)
	{
		LOG_OUTPUT("Error loading scene %s", full_path);
		ret = false;
	}

	return ret;
}

void GeometryLoader::UnloadFile(Mesh* mesh)
{
	for (vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		if ((*it) == mesh)
		{
			(*it)->CleanUp();
			delete *it;
			meshes.erase(it);
			break;
		}
	}
}

void GeometryLoader::UnloadAllFiles()
{
	for (vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end();)
	{
		(*it)->CleanUp();
		delete *it;
		it = meshes.erase(it);
	}
}

Mesh::Mesh(float* _vertices, uint _num_vertices, uint* _indices, uint _num_indices, float* _uvs, uint _num_uvs, const char* filename)
{
	if (_num_vertices > 0)
	{
		// Vertices
		vertices = new float[_num_vertices * 3];
		memcpy(vertices, _vertices, sizeof(float) * _num_vertices * 3);
		num_vertices = _num_vertices;

		if (_num_indices > 0)
		{
			// Indices
			indices = new uint[_num_indices];
			memcpy(indices, _indices, sizeof(uint) * _num_indices);
			num_indices = _num_indices;
		}

		if (_num_uvs > 0)
		{
			// UVs
			uvs = new float[_num_uvs * 3];
			memcpy(uvs, _uvs, sizeof(float) * _num_uvs * 3);
			num_uvs = _num_uvs;
		}

		file_name = filename;

		// Bbox
		bbox.SetNegativeInfinity();
		bbox.Enclose((float3*)vertices, _num_vertices);
	}
}

void Mesh::CleanUp()
{
	// Unload from vram
	UnloadFromMemory();

	// Vertices
	id_vertices = 0;
	num_vertices = 0;
	if (vertices != nullptr)
		RELEASE_ARRAY(vertices);

	// Indices
	id_indices = 0;
	num_indices = 0;
	if (indices != nullptr)
		RELEASE_ARRAY(indices);

	// UVs
	id_uv = 0;
	num_uvs = 0;
	if (uvs != nullptr)
		RELEASE_ARRAY(uvs);
}

uint Mesh::GetIdVertices()
{
	return id_vertices;
}

uint Mesh::GetNumVertices()
{
	return num_vertices;
}

uint Mesh::GetIdIndices()
{
	return id_indices;
}

uint Mesh::GetNumIndices()
{
	return num_indices;
}

uint Mesh::GetIdUV()
{
	return id_uv;
}

uint Mesh::GetNumUVs()
{
	return num_uvs;
}

AABB Mesh::GetBBox()
{
	return bbox;
}

const char * Mesh::GetFilename()
{
	return file_name;
}

float * Mesh::GetVertices()
{
	return vertices;
}

uint * Mesh::GetIndices()
{
	return indices;
}

void Mesh::LoadToMemory()
{
	if(id_vertices == 0 && vertices != nullptr)
		id_vertices = App->renderer3D->LoadBuffer(vertices, num_vertices * 3);

	if(id_indices == 0 && indices != nullptr)
		id_indices = App->renderer3D->LoadBuffer(indices, num_indices);

	if(id_uv == 0 && uvs != nullptr)
		id_uv = App->renderer3D->LoadBuffer(uvs, num_uvs * 3);
}

void Mesh::UnloadFromMemory()
{
	if (id_vertices != 0)
	{
		App->renderer3D->UnloadBuffer(id_vertices, num_vertices * 3);
		id_vertices = 0;
	}
	
	if (id_indices != 0)
	{
		App->renderer3D->UnloadBuffer(id_indices, num_indices);
		id_indices = 0;
	}

	if (id_uv != 0)
	{
		App->renderer3D->UnloadBuffer(id_uv, num_uvs * 3);
		id_uv = 0;
	}
}

bool MeshImporter::Import(const char * file, const char * path, std::string & output_file)
{
	bool ret = false;

	return ret;
}

bool MeshImporter::Import(const void * buffer, uint size, std::string & output_file)
{
	bool ret = false;

	return ret;
}

bool MeshImporter::Load(const char * exported_file, Mesh * resource)
{
	bool ret = false;

	return ret;
}

bool MeshImporter::Save(const char * path, vector<Mesh*> meshes)
{
	bool ret = false;

	for (vector<Mesh*>::iterator mesh = meshes.begin(); mesh != meshes.end(); ++mesh)
	{
		uint ranges[2] = { (*mesh)->GetNumVertices(), (*mesh)->GetNumIndices() };
		uint size = sizeof(ranges) + sizeof(uint) * (*mesh)->GetNumIndices() + sizeof(float) * (*mesh)->GetNumVertices() * 3;
		char* data = new char[size]; // Allocate
		char* cursor = data;
		uint bytes = sizeof(ranges); // First store ranges
		memcpy(cursor, ranges, bytes);

		cursor += bytes; // Store indices
		bytes = sizeof(uint) * (*mesh)->GetNumIndices();
		memcpy(cursor, (*mesh)->GetIndices(), bytes);

		cursor += bytes; // Store vertices
		bytes = sizeof(float) * (*mesh)->GetNumVertices();
		memcpy(cursor, (*mesh)->GetVertices(), bytes);

		//const char* name = (*mesh)->GetFilename();

		//fopen
		if (App->file_system->SaveFile(path, data, "Test", "susto") == false)
		{
			return false;
		}
		//fclose

		ret = true;
	}
	


	return ret;
}
