#include "uniforms.h"

#include <cstring>

using namespace shader;

BaseUniformSetter shader::getUniformSetter(const char* typeName, unsigned int count) noexcept
{
	if (strcmp(typeName, "float") == 0)
	{
		switch (count)
		{
		    case 1: return reinterpret_cast<BaseUniformSetter>(glUniform1fv);
		    case 2: return reinterpret_cast<BaseUniformSetter>(glUniform2fv);
		    case 3: return reinterpret_cast<BaseUniformSetter>(glUniform3fv);
		    case 4: return reinterpret_cast<BaseUniformSetter>(glUniform4fv);
		}
	}
	if (strcmp(typeName, "double") == 0)
	{
		switch (count)
		{
		    case 1: return reinterpret_cast<BaseUniformSetter>(glUniform1dv);
		    case 2: return reinterpret_cast<BaseUniformSetter>(glUniform2dv);
		    case 3: return reinterpret_cast<BaseUniformSetter>(glUniform3dv);
		    case 4: return reinterpret_cast<BaseUniformSetter>(glUniform4dv);
		}
	}
	if (strcmp(typeName, "int") == 0)
	{
		switch (count)
		{
		    case 1: return reinterpret_cast<BaseUniformSetter>(glUniform1iv);
		    case 2: return reinterpret_cast<BaseUniformSetter>(glUniform2iv);
		    case 3: return reinterpret_cast<BaseUniformSetter>(glUniform3iv);
		    case 4: return reinterpret_cast<BaseUniformSetter>(glUniform4iv);
		}
	}
	if (strcmp(typeName, "unsigned int") == 0)
	{
		switch (count)
		{
		    case 1: return reinterpret_cast<BaseUniformSetter>(glUniform1uiv);
		    case 2: return reinterpret_cast<BaseUniformSetter>(glUniform2uiv);
		    case 3: return reinterpret_cast<BaseUniformSetter>(glUniform3uiv);
		    case 4: return reinterpret_cast<BaseUniformSetter>(glUniform4uiv);
		}
	}

	return nullptr;
}

BaseUniform::BaseUniform(int location, std::string name) :
	m_location{ location }, m_name{ std::move(name) }
{
	if (m_location < 0)
	{
		throw exceptions::GLRecAcquisitionException("Uniform is not attached to a shader program.");
	}
}
