#ifndef UNIFORMS_H
#define UNIFORMS_H

#include <glad/glad.h>
#include <string>
#include <type_traits>

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
			BaseUniform(std::string name) : m_name{ std::move(name) } { }

		protected:
			int m_location = -1;
			std::string m_name;

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
			Uniform(std::string name) :
				BaseUniform{ std::move(name) },
				m_setter { reinterpret_cast<ConcreteUniformSetter>(getUniformSetter(typeid(T).name(), Count)) }
			{ }

			void setData(const T* data) const
			{
				if (m_location < 0)
				{
					throw std::exception("Uniform is not attached to a shader program.");
				}

				m_setter(m_location, Count, data);
			}

		private:
			const unsigned int count = Count;			
			const ConcreteUniformSetter m_setter = nullptr;			

	};
}

#endif
