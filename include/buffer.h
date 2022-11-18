#ifndef BUFFER_H
#define BUFFER_H

#include "generalTypes.h"

namespace vertex
{
	enum class BufferTarget : unsigned int
	{
		ARRAY_BUFFER = 0x8892, COPY_READ_BUFFER = 0x8F36, COPY_WRITE_BUFFER = 0x8F37, ELEMENT_ARRAY_BUFFER = 0x8893,
		PIXEL_PACK_BUFFER = 0x88EB, PIXEL_UNPACK_BUFFER = 0x88EC, TEXTURE_BUFFER = 0x8C2A, TRANSFORM_FEEDBACK_BUFFER = 0x8C8E,
		UNIFORM_BUFFER = 0x8A11
	};

	enum class BufferDataUsage : unsigned int
	{
		STREAM_DRAW = 0x88E0, STREAM_READ = 0x88E1, STREAM_COPY = 0x88E2, STATIC_DRAW = 0x88E4,
		STATIC_READ = 0x88E1, STATIC_COPY = 0x88E6, DYNAMIC_DRAW = 0x88E8, DYNAMIC_READ = 0x88E9,
		DYNAMIC_COPY = 0x88EA
	};

	class Buffer
	{
		public:
			Buffer(BufferTarget target, const ArrayData& data, BufferDataUsage usage);
			~Buffer();

			void bind() const noexcept;
			void unbind() const noexcept;

			void setData(const ArrayData& data);
			const ArrayData& getData() const noexcept;

		private:
			unsigned int m_rendererId = 0;
			BufferTarget m_target = BufferTarget::ARRAY_BUFFER;
			ArrayData m_data;
			BufferDataUsage m_usage = BufferDataUsage::STATIC_DRAW;
		
	};
}

#endif
