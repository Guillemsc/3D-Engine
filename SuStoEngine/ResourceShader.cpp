#include "ResourceShader.h"
#include "Functions.h"
#include "App.h"
#include "ModuleRenderer3D.h"

ResourceShader::ResourceShader(std::string unique_id) : Resource(unique_id, ResourceType::RT_SHADER)
{
}

ResourceShader::~ResourceShader()
{
}

void ResourceShader::CleanUp()
{
}

void ResourceShader::SetData(ResourceShaderType _shader_type, const char * _code)
{
	shader_type = _shader_type;
	
	UpdateCode(_code);
}

void ResourceShader::UpdateCode(const char * code)
{
	if (code != nullptr)
	{
		shader_code = code;

		CompileShader();
	}
}

ResourceShaderType ResourceShader::GetShaderType() const
{
	return ResourceShaderType();
}

std::string ResourceShader::GetCode()
{
	return shader_code;
}

uint ResourceShader::GetShaderId() const
{
	return uint();
}

bool ResourceShader::GetCompiles() const
{
	return false;
}

void ResourceShader::CompileShader()
{
	compiles = false;

	if (shader_id > 0)
	{
		App->renderer3D->DeleteShader(shader_id);
		shader_id = 0;
	}

	switch (shader_type)
	{
	case ResourceShaderType::ST_VERTEX:
		shader_id = App->renderer3D->CreateVertexShader(shader_code.c_str());
		break;

	case ResourceShaderType::ST_FRAGMENT:
		shader_id = App->renderer3D->CreateFragmentShader(shader_code.c_str());
		break;
	}

	if (shader_id != 0)
	{
		CONSOLE_LOG("Shader compilation Success :)");
		compiles = true;
	}
}
