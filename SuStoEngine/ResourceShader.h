#ifndef __RESOURCE_SHADER_H__
#define __RESOURCE_SHADER_H__

#include "Resource.h"
#include <string>
#include "GeometryMath.h"
#include "GameObjectAbstractor.h"

class GameObject;

class ResourceShader : public Resource
{
public:
	ResourceShader(std::string unique_id);
	virtual ~ResourceShader();

	void CleanUp();

	void SetData(const char* vertex_code, const char* fragment_code);

	void UpdateVertexCode(const char* code);
	void UpdateFragmentCode(const char* code);

	std::string GetVertexCode();
	std::string GetFragmentCode();

	uint GetVertexId() const;
	uint GetFragmentId() const;

	bool GetVertexCompiles() const;
	bool GetFragmentCompiles() const;

	bool GetProgramLinked() const;
	uint GetProgramId() const;
	void UseProgram();

private:
	void CompileVertexShader();
	void CompileFragmentShader();
	bool CanLinkProgram();
	void LinkProgram();

private:
	std::string vertex_code;
	std::string fragment_code;

	uint		vertex_id = 0;
	uint		fragment_id = 0;

	bool		vertex_compiles = false;
	bool		fragment_compiles = false;

	uint		program_id = 0;
	bool		program_linked = false;

};

#endif // !__RESOURCE_SHADER_H__