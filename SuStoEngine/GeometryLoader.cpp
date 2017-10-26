#include "GeometryLoader.h"
#include "App.h"
#include "ModuleRenderer3D.h"
#include "Functions.h"
#include "GameObject.h"
#include "ModuleGameObject.h"
#include "ComponentMesh.h"
#include "ModuleCamera3D.h"
#include "ModuleFileSystem.h"
#include <filesystem>
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

	MeshImporter importer;
	importer.Load(App->file_system->library_mesh_path.c_str());

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

vector<Mesh*>* GeometryLoader::GetMeshesVector()
{
	return &meshes;
}

//void GeometryLoader::RecursiveLoadMesh(aiMesh * aimesh, aiNode * node)
//{
//	bool valid = true;
//
//	Mesh* mesh = new Mesh();
//
//	if (!aimesh->HasFaces())
//	{
//		LOG_OUTPUT("WARNING, geometry has no faces!");
//		valid = false;
//	}
//
//	if (valid)
//	{
//		// VERTICES
//		float* vertices = new float(aimesh->mNumVertices * 3);
//		memcpy(vertices, aimesh->mVertices, sizeof(float) * aimesh->mNumVertices * 3);
//
//		// INDICES
//		uint* indices = new uint[aimesh->mNumFaces * 3];
//
//		for (uint i = 0; i < aimesh->mNumFaces && valid; ++i)
//		{
//			if (aimesh->mFaces[i].mNumIndices == 3)
//			{
//				memcpy(&indices[i * 3], aimesh->mFaces[i].mIndices, 3 * sizeof(uint));
//			}
//			else
//			{
//				LOG_OUTPUT("WARNING, geometry face with != 3 indices!");
//				valid = false;
//			}
//		}
//
//		mesh->SetFaces(vertices, aimesh->mNumVertices, indices, aimesh->mNumFaces);
//		
//		delete[] vertices;
//		delete[] indices;
//	}
//
//	if (valid && aimesh->HasTextureCoords(0))
//	{
//		// UVS
//		float* uvs = new float[aimesh->mNumVertices * 3];
//		memcpy(uvs, (float*)aimesh->mTextureCoords[0], sizeof(float) * aimesh->mNumVertices * 3);
//
//		mesh->SetUvs(uvs, aimesh->mNumVertices);
//
//		delete[] uvs;
//	}
//}

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

Mesh::Mesh()
{
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

void Mesh::SetFaces(float * _vertices, uint _num_vertices, uint * _indices, uint _num_indices)
{
	if (num_vertices > 0)
	{
		// Vertices
		vertices = new float[_num_vertices * 3];
		memcpy(vertices, _vertices, sizeof(float) * _num_vertices * 3);
		num_vertices = _num_vertices;

		if (num_indices > 0)
		{
			// Indices
			indices = new uint[_num_indices];
			memcpy(indices, _indices, sizeof(uint) * _num_indices);
			num_indices = _num_indices;
		}
	}
}

void Mesh::SetUvs(float * _uvs, uint _num_uvs)
{
	if (_num_uvs > 0)
	{
		// UVs
		uvs = new float[_num_uvs * 3];
		memcpy(uvs, _uvs, sizeof(float) * _num_uvs * 3);
		num_uvs = _num_uvs;
	}
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

string Mesh::GetFilename()
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

float * Mesh::GetUVs()
{
	return uvs;
}

void Mesh::SetTransform(float3 _pos, Quat _rotation, float3 _scale)
{
	position = _pos;
	rotation = _rotation;
	scale = _scale;
}

float3 Mesh::GetPosition()
{
	return position;
}

Quat Mesh::GetRotation()
{
	return rotation;
}

float3 Mesh::GetScale()
{
	return scale;
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

bool MeshImporter::Load(const char * exported_file)
{
	bool ret = true;

	string path = exported_file;
	path += "*.sustomesh";

	// Find files in Directory
	WIN32_FIND_DATA search_data;

	HANDLE handle = FindFirstFile(path.c_str(), &search_data);
	char* m = search_data.cFileName;

	if (handle == INVALID_HANDLE_VALUE)
		return false;

	while (handle != INVALID_HANDLE_VALUE)
	{
		string mesh_path = exported_file;
		mesh_path += search_data.cFileName;

		// Open the file and get the size
		FILE* file = fopen(mesh_path.c_str(), "rb");
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
		uint ranges[3];		// ranges[0] = Vertices, ranges[1] = Indices, ranges[2] = Uvs
		uint bytes = sizeof(ranges);
		memcpy(ranges, cursor, bytes);

		// Store indices
		cursor += bytes;
		uint indices[9999];
		bytes = sizeof(uint) * ranges[1];
		memcpy(indices, cursor, bytes);

		// Store vertices
		cursor += bytes;
		float vertices[9999];
		bytes = sizeof(float) * ranges[0] * 3;
		memcpy(vertices, cursor, bytes);

		// Store UVs
		cursor += bytes;
		float Uvs[9999];
		bytes = sizeof(float) * ranges[2] * 3;
		memcpy(Uvs, cursor, bytes);

		// Create mesh --------------
			Mesh* new_mesh = new Mesh(vertices, ranges[0], indices, ranges[1], Uvs, ranges[2], GetFileNameFromFilePath(search_data.cFileName).c_str());
			new_mesh->LoadToMemory();
			App->geometry->GetMeshesVector()->push_back(new_mesh);

			LOG_OUTPUT("New mesh with %d vertices", ranges[0] * 3);
			LOG_OUTPUT("New mesh with %d indices", ranges[1]);

		// Create GameObjects
			string name = GetFileNameFromFilePath(search_data.cFileName);

			GameObject* go = App->gameobj->Create();
			go->SetName(name.c_str());
			go->AddComponent(MESH);
			ComponentMesh* component = (ComponentMesh*)go->FindComponentByType(MESH);
			component->SetMesh(new_mesh);

			GameObject* parent = nullptr;

		

			// Set mesh pos, rot and scale
			/*aiVector3D translation;
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
			go->transform->SetRotation(Quat(rotation.x, rotation.y, rotation.w, rotation.z));*/
			//go->transform->SetScale(float3(scaling.x, scaling.y, scaling.z));

		

		if (FindNextFile(handle, &search_data) == FALSE)
			break;
	}

	//Close the handle after use or memory/resource leak
	FindClose(handle);

	return ret;
}

bool MeshImporter::Save(const char * path, vector<Mesh*> meshes)
{
	bool ret = true;

	int i = 0;
	for (vector<Mesh*>::iterator mesh = meshes.begin(); mesh != meshes.end(); ++mesh)
	{
		string name = GetFilenameWithoutExtension((*mesh)->GetFilename().c_str());
		name += "_";
		name += std::to_string(i++);

		uint ranges[3] = { (*mesh)->GetNumVertices(), (*mesh)->GetNumIndices(), (*mesh)->GetNumUVs() };
		uint size = sizeof(ranges) + sizeof(uint) * (*mesh)->GetNumIndices() + sizeof(float) * (*mesh)->GetNumVertices() * 3 + sizeof(float) * (*mesh)->GetNumUVs() * 3 + sizeof(AABB);
		
		char* data = new char[size]; // Allocate
		char* cursor = data;
		uint bytes = sizeof(ranges); // First store ranges
		memcpy(cursor, ranges, bytes);

		cursor += bytes; // Store indices
		bytes = sizeof(uint) * (*mesh)->GetNumIndices();
		memcpy(cursor, (*mesh)->GetIndices(), bytes);

		cursor += bytes; // Store vertices
		bytes = sizeof(float) * (*mesh)->GetNumVertices() * 3;
		memcpy(cursor, (*mesh)->GetVertices(), bytes);

		cursor += bytes; // Store UVs
		bytes = sizeof(float) * (*mesh)->GetNumUVs() * 3;
		memcpy(cursor, (*mesh)->GetUVs(), bytes);

		//fopen
		if (App->file_system->SaveFile(path, data, name.c_str(), "sustomesh", size) == false)
		{
			return false;
		}
		//fclose
	}
	


	return ret;
}
