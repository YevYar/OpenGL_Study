#ifndef APP_RENDERER_RENDERER_H
#define APP_RENDERER_RENDERER_H

#include <memory>

#include "helpers/macros.h"
#include "sceneObject.h"

namespace app::renderer
{
	class Renderer
	{
		// TYPES
		private:
			struct Impl;

		// METHODS
		public:
			Renderer();

			NOT_COPYABLE_MOVABLE(Renderer)

			virtual ~Renderer() = default;

			virtual void draw();

		protected:
			Renderer(std::unique_ptr<Impl> impl);

		// DATA
		protected:
			std::unique_ptr<Impl> m_impl;
		
	};
}

#endif
