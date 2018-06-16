#include "ResourceShaderLoader.h"
#include "App.h"
#include "JSONLoader.h"
#include "Functions.h"
#include "ResourceManager.h"

ResourceShaderLoader::ResourceShaderLoader() : 
ResourceLoader(ResourceType::RT_SHADER, App->file_system->GetLibraryShadersPath(), "ShaderLoader")
{

}

ResourceShaderLoader::~ResourceShaderLoader()
{
}

void ResourceShaderLoader::Start()
{
	CreateDefaultShaders();
}

Resource * ResourceShaderLoader::CreateResource(std::string new_uid)
{
	Resource* ret = nullptr;

	ret = new ResourceShader(new_uid);

	return ret;
}

bool ResourceShaderLoader::LoadFileToEngine(DecomposedFilePath decomposed_file_path, std::vector<Resource*>& resources)
{
	return false;
}

bool ResourceShaderLoader::RemoveAssetInfoFromEngine(DecomposedFilePath decomposed_file_path)
{
	return false;
}

void ResourceShaderLoader::ClearFromGameObject(Resource * resource, GameObject * go)
{
}

bool ResourceShaderLoader::ExportResourceToLibrary(Resource * resource)
{
	bool ret = false;

	if (resource != nullptr)
	{
		ResourceShader* resource_shad = (ResourceShader*)resource;

		string uid = resource->GetUniqueId();

		string meta_path = library_path + uid + ".meta";

		if (App->file_system->FileExists(meta_path.c_str()))
			App->file_system->FileDelete(meta_path.c_str());

		JSON_Doc* doc = App->json->CreateJSON(meta_path.c_str());

		if (doc != nullptr)
		{
			doc->SetString("vertex_code", resource_shad->GetVertexCode().c_str());
			doc->SetString("fragment_code", resource_shad->GetVertexCode().c_str());

			doc->Save();

			App->json->UnloadJSON(doc);

			ret = true;
		}
	}

	return ret;
}

bool ResourceShaderLoader::ExportResourceToAssets(Resource * resource)
{
	bool ret = false;

	return ret;
}

bool ResourceShaderLoader::ImportResourceFromLibrary(DecomposedFilePath d_filepath)
{
	bool ret = false;

	string meta_path = d_filepath.path + d_filepath.file_name + ".meta";

	JSON_Doc* doc = App->json->LoadJSON(meta_path.c_str());
	if (doc != nullptr)
	{
		std::string vertex_code = doc->GetString("vertex_code");
		std::string fragment_code = doc->GetString("fragment_code");

		ResourceShader* r_shader = (ResourceShader*)App->resource_manager->CreateNewResource(ResourceType::RT_SHADER, d_filepath.file_name);
	
		r_shader->SetData(vertex_code.c_str(), fragment_code.c_str());

		App->json->UnloadJSON(doc);
	}

	return ret;
}

bool ResourceShaderLoader::LoadAssetIntoScene(DecomposedFilePath decomposed_file_path)
{
	return false;
}

bool ResourceShaderLoader::IsAssetOnLibrary(DecomposedFilePath d_filepath, std::vector<std::string>& library_files_used)
{
	return false;
}

bool ResourceShaderLoader::RenameAsset(DecomposedFilePath decomposed_file_path, const char * new_name)
{
	return false;
}

void ResourceShaderLoader::CreateDefaultShaders()
{
	char vert_shader[512] =
		"#version 330 core\n \
		layout(location = 0) in vec3 position;\n \
		layout(location = 1) in vec3 texCoord;\n \
		layout(location = 2) in vec3 normals;\n \
		layout(location = 3) in vec4 color;\n\n \
		out vec4 ourColor;\n \
		out vec3 Normal;\n \
		out vec2 TexCoord;\n\n \
		uniform mat4 Model;\n \
		uniform mat4 view;\n \
		uniform mat4 projection;\n\n \
		void main()\n \
		{ \n \
			gl_Position = projection * view * Model * vec4(position, 1.0f);\n \
			ourColor = color;\n \
			TexCoord = texCoord.xy;\n \
		}";

	char frag_shader[512] =
		"#version 330 core\n \
		in vec4 ourColor;\n \
		in vec3 Normal;\n \
		in vec2 TexCoord;\n\n \
		out vec4 color;\n\n \
		uniform sampler2D ourTexture;\n\n \
		void main()\n \
		{\n \
			color = texture(ourTexture, TexCoord);\n \
		}";

	default_shader = (ResourceShader*)App->resource_manager->CreateNewResource(ResourceType::RT_SHADER);
	default_shader->SetData(vert_shader, frag_shader);

}

ResourceShader* ResourceShaderLoader::CreateShader(const char* vertex_code, const char* fragment_code)
{
	ResourceShader* ret = nullptr;

	if (vertex_code != nullptr && fragment_code != nullptr)
	{
		ret = (ResourceShader*)App->resource_manager->CreateNewResource(ResourceType::RT_SHADER);

		ret->SetData(vertex_code, fragment_code);

		ExportResourceToLibrary(ret);
	}

	return ret;
}

ResourceShader * ResourceShaderLoader::GetDefaultShader() const
{
	return default_shader;
}
