#include "GeometryLoader.h"
#include "App.h"
#include "ModuleRenderer3D.h"
#include "IDGenerator.h"

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

	LoadFile("warrior.FBX");

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

	// detach log stream
	aiDetachAllLogStreams();

	return ret;
}

bool GeometryLoader::LoadFile(const char * full_path)
{
	bool ret = false;

	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		LOG_OUTPUT("LOADING %d MESHES", scene->mNumMeshes);

		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; ++i)
		{
			Mesh new_mesh;

			// copy vertices
			aiMesh* current_mesh = scene->mMeshes[i];

			new_mesh.num_vertices = current_mesh->mNumVertices;
			new_mesh.vertices = new float[new_mesh.num_vertices * 3];
			memcpy(new_mesh.vertices, current_mesh->mVertices, sizeof(float) * new_mesh.num_vertices * 3);

			LOG_OUTPUT("New mesh with %d vertices", new_mesh.num_vertices);

			// copy faces
			if (current_mesh->HasFaces())
			{
				new_mesh.num_indices = current_mesh->mNumFaces * 3;
				new_mesh.indices = new uint[new_mesh.num_indices]; // assume each face is a triangle
				for (uint i = 0; i < current_mesh->mNumFaces; ++i)
				{
					if (current_mesh->mFaces[i].mNumIndices != 3)
					{
						LOG_OUTPUT("WARNING, geometry face with != 3 indices!");
						assert(current_mesh->mFaces[i].mNumIndices != 3, "GUILLEM LA XUPA, AH Y NO HAY 3 INDICES");
					}
					else
						memcpy(&new_mesh.indices[i * 3], current_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
				}
			}
			LOG_OUTPUT("New mesh with %d indices", new_mesh.num_indices);

			// Add mesh to vector<Mesh> meshes
			meshes.push_back(new_mesh);
		}



		aiReleaseImport(scene);
		ret = true;
	}
	else
		LOG_OUTPUT("Error loading scene %s", full_path);

	return ret;
}

Mesh::Mesh(uint _id_vertices, uint _num_indices, uint * _indices, uint _id_indices, uint _num_vertices, float * _vertices)
{
	id_vertices = _id_vertices;
	num_indices = _num_indices;
	indices = _indices;
	id_indices = _id_indices;
	num_vertices = _num_vertices;
	vertices = _vertices;
}

void Mesh::LoadToMemory()
{
	id_vertices = App->renderer3D->LoadBuffer(vertices, num_vertices);
	id_indices = App->renderer3D->LoadBuffer(indices, num_indices);
}

void Mesh::Draw()
{
}
