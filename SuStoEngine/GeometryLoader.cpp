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
	Assimp::DefaultLogger::create();
	const uint severity = Assimp::Logger::Debugging | Assimp::Logger::Info | Assimp::Logger::Err | Assimp::Logger::Warn;
	Assimp::DefaultLogger::get()->attachStream(new AssimpLogger(), severity);

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

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

	// Unload all meshes
	UnloadAllFiles();

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
	bool valid = true;

	if (node->mNumMeshes == 0)
		valid = false;

	aiMesh* aimesh = nullptr;
	Mesh* mesh = nullptr;
	if (valid)
	{
		int mesh_index = node->mMeshes[0];
		aimesh = scene->mMeshes[mesh_index];

		mesh = new Mesh();
		mesh->SetUniqueId(GetUniqueIdentifierRandom());

		if (!aimesh->HasFaces())
		{
			LOG_OUTPUT("WARNING, geometry has no faces!");
			valid = false;
		}
	}

	// VERTICES && INDICES
	if (valid)
	{
		float* vertices = new float[aimesh->mNumVertices * 3];
		memcpy(vertices, aimesh->mVertices, sizeof(float) * aimesh->mNumVertices * 3);

		uint* indices = new uint[aimesh->mNumFaces * 3];

		for (uint i = 0; i < aimesh->mNumFaces && valid; ++i)
		{
			if (aimesh->mFaces[i].mNumIndices == 3)
			{
				memcpy(&indices[i * 3], aimesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
			else
			{
				LOG_OUTPUT("WARNING, geometry face with != 3 indices!");
				valid = false;
			}
		}

		mesh->SetFaces(vertices, aimesh->mNumVertices, indices, aimesh->mNumFaces*3);
		
		RELEASE_ARRAY(vertices);
		RELEASE_ARRAY(indices);
	}

	// UVS
	if (valid && aimesh->HasTextureCoords(0))
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
	if (valid)
	{
		aiVector3D aitranslation;
		aiVector3D aiscaling;
		aiQuaternion airotation;

		if (node != nullptr)
		{
			node->mTransformation.Decompose(aiscaling, airotation, aitranslation);
			position = float3(aitranslation.x, aitranslation.y, aitranslation.z);
			scale = float3(aiscaling.x, aiscaling.y, aiscaling.z);
			rotation = Quat(airotation.x, airotation.y, airotation.z, airotation.w);

			mesh->SetTransform(
				float3(position.x, position.y, position.z),
				Quat(rotation.x, rotation.y, rotation.w, rotation.z), 
				float3(scale.x, scale.y, scale.z));
		}
		else
			valid = false;
	}

	// GENERAL BBOX
	if (valid)
	{
		AABB mesh_with_scale = mesh->GetBBox();
		mesh_with_scale.Scale(position, scale);

		total_abb.Enclose(mesh_with_scale);
	}

	// MATERIALS
	Texture* texture = nullptr;
	if (valid)
	{
		aiMaterial* material = scene->mMaterials[0];

		string path = GetPathFromFilePath(full_path);

		// Difuse -------------------
		aiString file;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		path += file.C_Str();

		texture = App->texture->LoadTexture(path.c_str());
	}

	// CREATE GAME OBJECT
	GameObject* go = nullptr;
	if (valid && parent != nullptr)
	{
		go = App->gameobj->Create();
		
		string name = aimesh->mName.C_Str();
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
		mesh->LoadToMemory();

		if (texture != nullptr)
		{
			go->AddComponent(MATERIAL);
			ComponentMaterial* cmaterial = (ComponentMaterial*)go->GetComponent(MATERIAL);
			cmaterial->SetTexture(texture);
		}
	}

	// Select parent
	GameObject* pare = nullptr;

	if (valid)
		pare = go;
	else
		pare = parent;

	// RECURSE
	for (int i = 0; i < node->mNumChildren; i++)
	{
		RecursiveLoadMesh(scene, node->mChildren[i], full_path, total_abb, pare);
	}

	if (!valid)
	{
		RELEASE(mesh);
	}
	else
		mesh_importer->Save(App->file_system->GetLibraryMeshPath().c_str(), mesh);
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

MeshImporter * GeometryLoader::GetMeshImporter()
{
	return mesh_importer;
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

void Mesh::SetUniqueId(double _id)
{
    unique_id = _id;
}

void Mesh::SetFaces(float * _vertices, uint _num_vertices, uint * _indices, uint _num_indices)
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

		CalcMeshBBox();
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

float Mesh::GetDiagonal()
{
	return bbox.Diagonal().Length();
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

	CalcMeshBBox();
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

const double Mesh::GetUniqueId() const
{
	return unique_id;
}

void Mesh::CalcMeshBBox()
{
	bbox.SetNegativeInfinity();

	if (vertices != nullptr && num_vertices > 0)
	{
		bbox.Enclose((vec*)vertices, num_vertices);
	}
}

Mesh* MeshImporter::Load(const char * filepath)
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

	// Copy unique id
	double* id = new double;
	uint bytes = sizeof(double);
	memcpy(id, cursor, bytes);
	cursor += bytes;

	// Copy the ranges
	uint ranges[3];		// ranges[0] = Vertices, ranges[1] = Indices, ranges[2] = Uvs
	bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	cursor += bytes;

	// Store indices
	uint indices[9999];
	bytes = sizeof(uint) * ranges[1];
	memcpy(indices, cursor, bytes);
	cursor += bytes;

	// Store vertices
	float vertices[9999];
	bytes = sizeof(float) * ranges[0] * 3;
	memcpy(vertices, cursor, bytes);
	cursor += bytes;

	// Store UVs
	float uvs[9999];
	bytes = sizeof(float) * ranges[2] * 3;
	memcpy(uvs, cursor, bytes);
	cursor += bytes;

	// Create mesh --------------
	Mesh* new_mesh = new Mesh();
	new_mesh->SetFaces(vertices, ranges[0], indices, ranges[1]);
	new_mesh->SetUvs(uvs, ranges[2]);
	new_mesh->SetUniqueId(double(*id));
	new_mesh->LoadToMemory();
	App->geometry->GetMeshesVector()->push_back(new_mesh);

	LOG_OUTPUT("New mesh with %d vertices", ranges[0] * 3);
	LOG_OUTPUT("New mesh with %d indices", ranges[1]);

	RELEASE(id);
	if (new_mesh != nullptr) {
		return new_mesh;
	}

	return nullptr;
}

bool MeshImporter::Save(const char * path, Mesh* mesh)
{
	bool ret = true;
	
	string name = GetFilenameWithoutExtension(mesh->GetFilename().c_str());
	name += "_";
	name += std::to_string(App->id->NewId("mesh"));

	double* id = new double;
	*id = mesh->GetUniqueId();

	uint ranges[3] = { mesh->GetNumVertices(), mesh->GetNumIndices(), mesh->GetNumUVs() };
	uint size = sizeof(double) + sizeof(ranges) + 
		sizeof(uint) * mesh->GetNumIndices() + 
		sizeof(float) * mesh->GetNumVertices() * 3 + 
		sizeof(float) * mesh->GetNumUVs() * 3;
	
	// Allocate data
	char* data = new char[size];
	char* cursor = data;

	// Store unique id
	uint bytes = sizeof(double);
	memcpy(cursor, id, bytes);
	cursor += bytes;

	// Store ranges
	bytes = sizeof(ranges);
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

	RELEASE(id);
	RELEASE_ARRAY(data);
	
	return ret;
}

void MeshImporter::ImportAllMeshes()
{
	vector<string> paths = App->file_system->GetFilesInPath(App->file_system->GetLibraryMeshPath().c_str(), "sustomesh");

	for (vector<string>::iterator it = paths.begin(); it != paths.end(); it++)
	{
		Mesh* new_mesh = Load((*it).c_str());

		/*if (new_mesh != nullptr)
		{
			GameObject* new_gameobject = App->gameobj->Create();
			new_gameobject->AddComponent(ComponentType::MESH);
			ComponentMesh* mesh_comp = (ComponentMesh*) new_gameobject->GetComponent(ComponentType::MESH);
			mesh_comp->SetMesh(new_mesh);

		}*/
	}
}

