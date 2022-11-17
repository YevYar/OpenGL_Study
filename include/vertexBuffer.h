#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

namespace vertex {
	enum class DataUsage
	{
		STREAM_DRAW = 0x88E0, STREAM_READ = 0x88E1, STREAM_COPY = 0x88E2, STATIC_DRAW = 0x88E4,
		STATIC_READ = 0x88E1, STATIC_COPY = 0x88E6, DYNAMIC_DRAW = 0x88E8, DYNAMIC_READ = 0x88E9,
		DYNAMIC_COPY = 0x88EA
	};

	struct VertexData
	{
		const void* data;
		unsigned int size;
	};

	class VertexBuffer {
		public:
			VertexBuffer(const VertexData& data, DataUsage usage);
			~VertexBuffer();

			void bind() const;
			void unbind() const;

			void setData(const VertexData& data);
			const VertexData& getData() const;

		private:
			unsigned int m_rendererId;
			VertexData m_data;
			DataUsage m_usage = DataUsage::STATIC_DRAW;
		
	};
}

#endif
