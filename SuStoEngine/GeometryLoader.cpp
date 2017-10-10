#include "GeometryLoader.h"

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

bool GeometryLoader::LoadFile(const char * full_path) const
{
	bool ret = false;

	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; ++i)
		{

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
