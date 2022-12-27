#ifndef UNIFORMS_H
#define UNIFORMS_H

#include <glad/glad.h>
#include <iostream>
#include <string>
#include <type_traits>

#include "exceptions.h"
#include "helpers/macros.h"

namespace shader
{
	class ShaderProgram;
}

namespace uniforms
{
	using BaseUniformSetter = void (*)(GLint, GLsizei, const void*);

	BaseUniformSetter getUniformSetter(const char* typeName, unsigned int count) noexcept;

	class BaseUniform
	{
		public:
			virtual ~BaseUniform() = default;

			virtual void setData(const void* data) = 0;

		protected:
			BaseUniform(int location, std::string name);

			NOT_COPYABLE_MOVABLE(BaseUniform)
			
		protected:
			const int m_location = -1;
			const std::string m_name;			

			friend class shader::ShaderProgram;

	};

	template<typename T, unsigned int Count>
	class Uniform : public BaseUniform
	{
		static_assert(std::is_same_v<float, T> || std::is_same_v<double, T>
			|| std::is_same_v<int, T> || std::is_same_v<unsigned, T>,
			"A Uniform can be of the following types: float, double, int or unsigned int.");
		static_assert(Count >= 1 && Count <= 4, "Count must be in range [1, 4].");


		private:
			using ConcreteUniformSetter = void (*)(GLint, GLsizei, const T*);

		public:
			void setData(const void* data) override
			{
				GLCall(m_setter(m_location, Count, reinterpret_cast<const T*>(data)));
			}

		protected:
			Uniform(int location, std::string name) :
				BaseUniform{ location, std::move(name) },
				m_setter{ reinterpret_cast<ConcreteUniformSetter>(getUniformSetter(typeid(T).name(), Count)) }
			{
				if (m_setter == nullptr)
				{
					throw exceptions::GLRecAcquisitionException(
						"No uniform setter function for specified template arguments.");
				}
			}

		private:
			const unsigned int count = Count;			
			const ConcreteUniformSetter m_setter = nullptr;			

			friend class shader::ShaderProgram;

	};
}

#endif
