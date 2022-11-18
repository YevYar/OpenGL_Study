#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

namespace vertex
{
	class Buffer;
	class VertexBufferLayout;

	class VertexArray
	{
		public:
			VertexArray();
			~VertexArray();

			void bind() const noexcept;
			void unbind() const noexcept;

			void addBuffer(const Buffer& vbo, const VertexBufferLayout& layout) const noexcept;
			void enableAttribute(unsigned int index) const noexcept;
			void disableAttribute(unsigned int index) const noexcept;

		private:
			unsigned int m_rendererId = 0;
			
	};
}

#endif
