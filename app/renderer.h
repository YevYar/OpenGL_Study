#ifndef APP_RENDERER_RENDERER_H
#define APP_RENDERER_RENDERER_H

#include <memory>

#include "helpers/macros.h"

/**
 * \namespace app
 * \brief app namespace contains types and functions of demo application.
 */
/**
 * \namespace app::renderer
 * \brief renderer namespace contains types and functions to provide basic graphical objects or their managers.
 */
namespace app::renderer
{
/**
 * \brief Renderer is a controller/script of what must be rendered per each render loop iteration.
 *
 * User should create own class inherited from Renderer and define in a render() all necessary stuff, which must be
 * done per every render loop iteration.
 */
class Renderer
{
    private:
        /**
         * \brief Impl contains private data and methods of Renderer.
         */
        struct Impl;

    public:
        /**
         * \brief Constructs new Renderer object and initialize necessary OpenGL stuff.
         */
        Renderer();
        OGLS_NOT_COPYABLE_MOVABLE(Renderer)
        virtual ~Renderer() noexcept = default;

        /**
         * \brief Performs rendering.
         *
         * This method must be overridden to define all stuff, which must be rendered on the screen.
         * It must be called per every render loop iteration.
         */
        virtual void render();

    private:
        /**
         * \brief Pointer to implementation.
         */
        std::unique_ptr<Impl> m_impl;

};  // class Renderer

}  // namespace app::renderer

#endif
