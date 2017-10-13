#include "GeometryLoader.h"
#include "App.h"
#include "ModuleRenderer3D.h"
#include "Functions.h"
#include "GameObject.h"
#include "ModuleGameObject.h"
#include "ComponentMesh.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

GeometryLoader::GeometryLoader(bool start_enabled) : Module(start_enabled)
{
}

GeometryLoader::~GeometryLoader()
{
}

bool GeometryLoader::Awake()
{
	bool ret = true;

	return ret;
}

bool GeometryLoader::Start()
{
	bool ret = true;

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	LoadFile("BakerHouse.fbx", true);

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

	UnloadAllFiles();

	// detach log stream
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
	bool ret = false;

	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);

	uint id_vertices = 0;
	uint id_indices = 0;
	uint id_uv = 0;

	if (scene != nullptr && scene->HasMeshes())
	{
		LOG_OUTPUT("LOADING %d MESHES", scene->mNumMeshes);

		// -------------------------------------------
		// LOAD MESH ---------------------------------
		// -------------------------------------------
		for (int i = 0; i < scene->mNumMeshes; ++i)
		{
			aiMesh* current_mesh = scene->mMeshes[i];

			uint* indices = new uint[current_mesh->mNumFaces * 3];

			// VERTICES ----------------
			id_vertices = App->renderer3D->LoadBuffer((float*)current_mesh->mVertices, current_mesh->mNumVertices * 3);

			// INDICES -----------------
			if (current_mesh->HasFaces())
			{
				// Assume each face is a triangle
				for (uint i = 0; i < current_mesh->mNumFaces; ++i)
				{
					if (current_mesh->mFaces[i].mNumIndices != 3)
					{
						LOG_OUTPUT("WARNING, geometry face with != 3 indices!");
						assert(current_mesh->mFaces[i].mNumIndices != 3, "WARNING, geometry face with != 3 indices!");
					}
					else
						memcpy(&indices[i * 3], current_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
				}

				id_indices = App->renderer3D->LoadBuffer(indices, current_mesh->mNumFaces * 3);

				delete[] indices;
			}

			// UVS ----------------------
			if (current_mesh->HasTextureCoords(0))
			{
				id_uv = App->renderer3D->LoadBuffer((float*)current_mesh->mTextureCoords[0], current_mesh->mNumVertices * 3);
			}

			// Save info
			Mesh* new_mesh = new Mesh(id_vertices, current_mesh->mNumVertices, id_indices, current_mesh->mNumFaces * 3, id_uv, current_mesh->mNumVertices);

			LOG_OUTPUT("New mesh with %d vertices", current_mesh->mNumVertices);
			LOG_OUTPUT("New mesh with %d indices", current_mesh->mNumFaces * 3);

			meshes.push_back(new_mesh);

			// Create GameObjects
			if (as_new_gameobject)
			{
				string name = GetFileName(full_path); name += "_"; name += std::to_string(i);

				GameObject* go = App->gameobj->Create();
				go->SetName(name.c_str());
				go->AddComponent(MESH);
				ComponentMesh* component = (ComponentMesh*)go->FindComponentByType(MESH);
				component->SetMesh(new_mesh);
			}
		}

		// -------------------------------------------
		// LOAD TEXTURES -----------------------------
		// -------------------------------------------
		aiMaterial* material = scene->mMaterials[0];

		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		App->LoadFile(path.C_Str());


		// -------------------------------------------
		// RELEASE -----------------------------------
		// -------------------------------------------
		aiReleaseImport(scene);
		ret = true;
	}
	else
		LOG_OUTPUT("Error loading scene %s", full_path);

	return ret;
}

void GeometryLoader::UnloadFile(Mesh* mesh)
{
	for (vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); it++)
	{
		if ((*it) == mesh)
		{
			// Unload from memory
			if ((*it)->GetIdVertices() != 0)
			{
				App->renderer3D->UnloadBuffer((*it)->GetIdVertices(), (*it)->GetNumVertices() * 3);
			}

			if ((*it)->GetIdIndices() != 0)
			{
				App->renderer3D->UnloadBuffer((*it)->GetIdIndices(), (*it)->GetNumIndices());
			}

			if ((*it)->GetIdUV() != 0)
			{
				App->renderer3D->UnloadBuffer((*it)->GetIdUV(), (*it)->GetNumUVs() * 3);
			}

			(*it)->CleanUp();
			meshes.erase(it);
			break;
		}
	}
}

void GeometryLoader::UnloadAllFiles()
{
	for (vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end();)
	{
		// Unload from memory
		if ((*it)->GetIdVertices() != 0)
		{
			App->renderer3D->UnloadBuffer((*it)->GetIdVertices(), (*it)->GetNumVertices() * 3);
		}

		if ((*it)->GetIdIndices() != 0)
		{
			App->renderer3D->UnloadBuffer((*it)->GetIdIndices(), (*it)->GetNumIndices());
		}

		if ((*it)->GetIdUV() != 0)
		{
			App->renderer3D->UnloadBuffer((*it)->GetIdUV(), (*it)->GetNumUVs() * 3);
		}

		(*it)->CleanUp();;
		it = meshes.erase(it);
	}
}

Mesh::Mesh(uint _id_vertices, uint _num_vertices, uint _id_indices, uint _num_indices, uint _id_uv, uint _num_uvs)
{
	num_vertices = _num_vertices;
	num_indices = _num_indices;

	id_vertices = _id_vertices; 
	id_indices = _id_indices;	

	id_uv = _id_uv;
	num_uvs = _num_uvs;
}

bool Mesh::operator==(Mesh mesh)
{
	bool ret = false;

	if (id_vertices == mesh.id_vertices && num_indices == mesh.num_indices
		&& id_indices == mesh.id_indices && num_vertices == mesh.num_vertices)
		ret = true;

	return ret;
}

void Mesh::CleanUp()
{
	id_vertices = 0; 
	num_indices = 0;

	id_indices = 0; 
	num_vertices = 0;
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
