#ifndef BUFFER_H
#define BUFFER_H

#include <memory>
#include <vector>

#include "generalTypes.h"

namespace vertex
{
	class VertexBufferLayout;

	/**
	 * Enum to represent 'target' parameter of [glBindBuffer](https://docs.gl/gl3/glBindBuffer).
	 */
	enum class BufferTarget : unsigned int
	{
		ARRAY_BUFFER = 0x8892, COPY_READ_BUFFER = 0x8F36, COPY_WRITE_BUFFER = 0x8F37, ELEMENT_ARRAY_BUFFER = 0x8893,
		PIXEL_PACK_BUFFER = 0x88EB, PIXEL_UNPACK_BUFFER = 0x88EC, TEXTURE_BUFFER = 0x8C2A, TRANSFORM_FEEDBACK_BUFFER = 0x8C8E,
		UNIFORM_BUFFER = 0x8A11
	};

	/**
	 * Enum to represent 'usage' parameter of [glBufferData](https://docs.gl/gl3/glBufferData).
	 */
	enum class BufferDataUsage : unsigned int
	{
		STREAM_DRAW = 0x88E0, STREAM_READ = 0x88E1, STREAM_COPY = 0x88E2, STATIC_DRAW = 0x88E4,
		STATIC_READ = 0x88E1, STATIC_COPY = 0x88E6, DYNAMIC_DRAW = 0x88E8, DYNAMIC_READ = 0x88E9,
		DYNAMIC_COPY = 0x88EA
	};

	/*!
	 * \brief Buffer is a wrapper for OpenGL buffer object. 	
	 */
	class Buffer
	{
		public:
			/**
			 * \brief Constructs new Buffer object with passed arguments and
			 * generates new 1 buffer in OpenGL state machine.
			 * 
			 * Buffer data specified by data parameter is not immediately loaded in generated OpenGL buffer.
			 * Data is loaded in generated OpenGL buffer in first call of method bind(). 
			 * 
			 * \param target - target to bind buffer to
			 * \param data - data, which must be set in OpenGL buffer
			 * \param usage - usage of data
			 * \param bufferLayout - pointer on VertexBufferLayout.
			 * Use only for buffer, which is intended to be vertex buffer object
			 * 
			 * \throw exceptions::GLRecAcquisitionException 
			 */
			Buffer(BufferTarget target, ArrayData data, BufferDataUsage usage,
				std::shared_ptr<VertexBufferLayout> bufferLayout = nullptr);
			/**
			 * \brief Destructor, which deletes buffer object in OpenGL state machine.
			 */
			~Buffer();

			/**
			 * \brief Copy constructor, which constructs new Buffer as copy of other Buffer.
			 * 
			 * However new 1 buffer in OpenGL state machine is generated.
			 * Deep copy of m_data is not made (see copy constructor of ArrayData).
			 *  
			 * Buffer data is not immediately loaded in generated OpenGL buffer.
			 * Data is loaded in generated OpenGL buffer in first call of method bind().
			 * 
			 * \throw exceptions::GLRecAcquisitionException
			 */
			Buffer(const Buffer& obj);

			/**
			 * \brief Move constructor, which constructs new Buffer as copy of other Buffer.
			 *
			 * New 1 buffer in OpenGL state machine is not generated.
			 *
			 * If data of moved Buffer was not loaded early,
			 * data is loaded in OpenGL buffer in first call of method bind().
			 */
			Buffer(Buffer&& obj) noexcept;

			/**
			 * \brief Copy assignment operator.
			 * 
			 * Copies state of other Buffer. References on pointed OpenGL buffers of objects are not changed.
			 * Deep copy of m_data is not made (see copy constructor of ArrayData).
			 * 
			 * Data is loaded in OpenGL buffer of assignment destination Buffer in first call of method bind().
			 */
			Buffer& operator=(const Buffer& obj);

			/**
			 * \brief Move assignment operator.
			 *
			 * Copies state of other Buffer. Reference on pointed OpenGL buffer of assignment destination Buffer is not changed.
			 * 
			 * Data is loaded in OpenGL buffer of assignment destination Buffer in first call of method bind().
			 */
			Buffer& operator=(Buffer&& obj) noexcept;

			/**
			 * \brief Wrapper for [glBindBuffer](https://docs.gl/gl3/glBindBuffer).
			 * 
			 * Also sets Buffer data in OpenGL buffer if it was not set before.
			 */
			void bind() const noexcept;
			void unbind() const noexcept;

			/**
			 * \brief Sets new Buffer data and loads it in OpenGL buffer.
			 * 
			 * The buffer is immediately bound (see bind()) and data is loaded.
			 */
			void setData(ArrayData data);

			/**
			 * \brief Returns data of the Buffer.
			 * 
			 * It is not guaranteed that data of the Buffer is loaded in OpenGL buffer.
			 * 
			 * Loading of data is done by:
			 * - first call of bind() after object creation or assignment,
			 * - calling of setData(). 
			 */
			const ArrayData& getData() const noexcept;

		private:
			void genBuffer();
			void updateData() const noexcept;

		private:
			/**
			 * \brief Id of referenced OpenGL buffer.
			 */
			unsigned int m_rendererId = 0;
			BufferTarget m_target = BufferTarget::ARRAY_BUFFER;
			ArrayData m_data;
			BufferDataUsage m_usage = BufferDataUsage::STATIC_DRAW;
			std::shared_ptr<VertexBufferLayout> m_layout = nullptr;
			mutable bool m_isDataSet = false;
		
	};
}

#endif
