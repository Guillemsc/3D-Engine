#ifndef __RESOURCE_SHADER_H__
#define __RESOURCE_SHADER_H__

#include "Resource.h"
#include <string>
#include "GeometryMath.h"
#include "GameObjectAbstractor.h"

class GameObject;

enum ResourceShaderType
{
	ST_NULL,
	ST_VERTEX,
	ST_FRAGMENT,
};

class ResourceShader : public Resource
{
public:
	ResourceShader(std::string unique_id);
	virtual ~ResourceShader();

	void CleanUp();

	void SetData(ResourceShaderType shader_type, const char* code);

	void UpdateCode(const char* code);

	ResourceShaderType GetShaderType() const;
	std::string GetCode();
	uint GetShaderId() const;
	bool GetCompiles() const;

private:
	void CompileShader();

private:
	ResourceShaderType shader_type = ResourceShaderType::ST_NULL;

	std::string shader_code;
	uint		shader_id = 0;
	bool		compiles = false;

};

#endif // !__RESOURCE_SHADER_H__