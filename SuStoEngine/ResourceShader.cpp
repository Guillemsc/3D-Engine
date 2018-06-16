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

void ResourceShader::SetData(const char* vertex_code, const char* fragment_code)
{	
	UpdateVertexCode(vertex_code);
	UpdateFragmentCode(fragment_code);
}

void ResourceShader::UpdateVertexCode(const char * code)
{
	if (code != nullptr)
	{
		vertex_code = code;

		CompileVertexShader();
	}
}

void ResourceShader::UpdateFragmentCode(const char * code)
{
	if (code != nullptr)
	{
		fragment_code = code;

		CompileFragmentShader();
	}
}

std::string ResourceShader::GetVertexCode()
{
	return vertex_code;
}

std::string ResourceShader::GetFragmentCode()
{
	return fragment_code;
}

uint ResourceShader::GetVertexId() const
{
	return vertex_id;
}

uint ResourceShader::GetFragmentId() const
{
	return fragment_id;
}

bool ResourceShader::GetVertexCompiles() const
{
	return vertex_compiles;
}

bool ResourceShader::GetFragmentCompiles() const
{
	return fragment_compiles;
}

bool ResourceShader::GetProgramLinked() const
{
	return program_linked;
}

uint ResourceShader::GetProgramId() const
{
	return program_id;
}

void ResourceShader::UseProgram()
{
	if(program_id > 0)
		App->renderer3D->UseShaderProgram(program_id);
}

void ResourceShader::CompileVertexShader()
{
	vertex_compiles = false;

	if (vertex_id > 0)
	{
		App->renderer3D->DeleteShader(vertex_id);
		vertex_id = 0;
	}

	vertex_id = App->renderer3D->CreateVertexShader(vertex_code.c_str());

	if (vertex_id != 0)
	{
		CONSOLE_LOG("Vertex shader compilation Success :)");
		vertex_compiles = true;
	}

	if (CanLinkProgram())
		LinkProgram();
}

void ResourceShader::CompileFragmentShader()
{
	fragment_compiles = false;

	if (fragment_id > 0)
	{
		App->renderer3D->DeleteShader(fragment_id);
		fragment_id = 0;
	}

	fragment_id = App->renderer3D->CreateFragmentShader(fragment_code.c_str());

	if (vertex_id != 0)
	{
		CONSOLE_LOG("Fragment shader compilation Success :)");
		fragment_compiles = true;
	}

	if (CanLinkProgram())
		LinkProgram();
}

bool ResourceShader::CanLinkProgram()
{
	bool ret = false;

	if (vertex_compiles && fragment_compiles)
	{
		if (vertex_id > 0 && fragment_id > 0)
		{
			ret = true;
		}
	}

	return ret;
}

void ResourceShader::LinkProgram()
{
	if (program_id > 0)
	{
		App->renderer3D->DeleteProgram(program_id);
		program_id = 0;
		program_linked = false;
	}

	if (CanLinkProgram())
	{
		program_id = App->renderer3D->CreateShaderProgram();

		App->renderer3D->AttachShaderToProgram(program_id, vertex_id);
		App->renderer3D->AttachShaderToProgram(program_id, fragment_id);

		if (App->renderer3D->LinkProgram(program_id) == false)
		{
			CONSOLE_LOG("Error while linking shader program, check errors above.");

			App->renderer3D->DeleteProgram(program_id);
			program_id = 0;
			program_linked = false;
		}
		else
		{
			CONSOLE_LOG("Shader Program %d created :)", program_id);
			program_linked = true;
		}
	}
}
