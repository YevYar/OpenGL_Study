#ifndef OGLS_RENDERER_SCENE_OBJECT_H
#define OGLS_RENDERER_SCENE_OBJECT_H

#include <chrono>

#include "mathCore/transformMatrix.h"
#include "renderer/material.h"
#include "renderer/mesh.h"

namespace ogls::renderer
{
/**
 * \brief SceneObject is a base abstract class for all classes, which represents something, what can be rendered.
 */
class SceneObject
{
    public:
        /**
         * \brief Size represents the size in pixels of 3 dimensions of the SceneObject.
         */
        struct Size final
        {
            public:
                /**
                 * \brief The size in pixels of the dimension along the X axis.
                 */
                size_t length = {0};
                /**
                 * \brief The size in pixels of the dimension along the Y axis.
                 */
                size_t height = {0};
                /**
                 * \brief The size in pixels of the dimension along the Z axis.
                 */
                size_t width  = {0};

        };  // struct Size

    public:
        /**
         * \brief Constructs an invalid SceneObject without Mesh and Material.
         */
        SceneObject() = default;
        OGLS_DEFAULT_COPYABLE_MOVABLE(SceneObject)
        /**
         * \brief Constructs new SceneObject using Mesh and Material.
         *
         * \param mesh           - a model (Mesh) of the object. It's passed as shared_ptr, because it's expected,
         * that one Mesh can be used to render several SceneObject -s using different Material -s.
         * \param material       - a visual property (Material) of the object.
         * \param size           - the size of the SceneObject in pixels.
         * \param globalPosition - the Point in global space, in which the SceneObject will be spawned.
         */
        SceneObject(std::shared_ptr<Mesh> mesh, Material material, Size size,
                    mathCore::Point<int> globalPosition = mathCore::Point<int>{0, 0, 0}) noexcept;
        virtual ~SceneObject() noexcept = default;

        //------ ACCESSORS TO DIMENSIONAL PROPERTIES

        /**
         * \brief Returns the height of the object in pixels.
         */
        int  getHeight() const noexcept;
        /**
         * \brief Returns the length of the object in pixels.
         */
        int  getLength() const noexcept;
        /**
         * \brief Returns the size of the object.
         */
        Size getSize() const noexcept;
        /**
         * \brief Returns the width of the object in pixels.
         */
        int  getWidth() const noexcept;
        /**
         * \brief Sets the height of the object in pixels.
         *
         * \param height - the height in pixels to set.
         */
        void setHeight(int height) noexcept;
        /**
         * \brief Sets the length of the object in pixels.
         *
         * \param length - the length in pixels to set.
         */
        void setLength(int length) noexcept;
        /**
         * \brief Sets the size of the object.
         *
         * \param size - the size to set.
         */
        void setSize(Size size) noexcept;
        /**
         * \brief Sets the size of the object.
         *
         * \param length - the length in pixels to set.
         * \param height - the height in pixels to set.
         * \param width  - the width in pixels to set.
         */
        void setSize(int length, int height, int width) noexcept;
        /**
         * \brief Sets the width of the object in pixels.
         *
         * \param width - the width in pixels to set.
         */
        void setWidth(int width) noexcept;

        //------ ACCESSORS TO POSITION

        /**
         * \brief Returns the position of the SceneObject in global coordinates.
         */
        mathCore::Point<int> getPositionInGlobal() const noexcept;
        /**
         * \brief Returns the position of the SceneObject relative to its parent.
         */
        mathCore::Point<int> getPositionInParent() const noexcept;
        /**
         * \brief Sets the position of the SceneObject in global coordinates.
         *
         * \param position - the new position of the SceneObject in global coordinates.
         */
        void                 setPositionInGlobal(mathCore::Point<int> position);
        /**
         * \brief Sets the position of the SceneObject relative to its parent.
         *
         * \param position - the new position of the SceneObject relative to its parent.
         */
        void                 setPositionInParent(mathCore::Point<int> position);

        //------ ACCESSORS TO VISUAL PROPERTIES

        /**
         * \brief Returns a reference to the Material associated with this object.
         */
        Material&             getMaterial() noexcept;
        /**
         * \brief Returns a constant reference to the Material associated with this object.
         */
        const Material&       getMaterial() const noexcept;
        /**
         * \brief Returns the Mesh associated with this object.
         */
        std::shared_ptr<Mesh> getMesh() const noexcept;
        /**
         * \brief Sets the Material associated with this object.
         *
         * \param material - the new Material to set.
         */
        void                  setMaterial(Material material) noexcept;
        /**
         * \brief Sets the Mesh associated with this object.
         *
         * \param mesh - the new Mesh to set.
         */
        void                  setMesh(std::shared_ptr<Mesh> mesh) noexcept;

        /**
         * \brief Updates the object.
         *
         * This method is responsible for updating the state of the SceneObject.
         *
         * \note Leave the implementation empty if no update is required.
         * \param deltaTime - the time elapsed since the last frame, in milliseconds.
         */
        virtual void update(std::chrono::milliseconds deltaTime) = 0;

    private:
        /**
         * \brief Renders something on the scene.
         *
         * \param renderer - the object, which performs a rendering operation.
         */
        virtual void render(std::shared_ptr<Renderer> renderer) = 0;

    protected:
        /**
         * \brief The unique identifier of the SceneObject.
         *
         * 0 identifies an invalid empty SceneObject.
         */
        size_t                    m_id = {0};
        /**
         * \brief The Material used to render the SceneObject.
         */
        Material                  m_material;
        /**
         * \brief The Mesh representing the geometry of the SceneObject.
         */
        std::shared_ptr<Mesh>     m_mesh = nullptr;
        /**
         * \brief The model matrix of the object.
         */
        mathCore::TransformMatrix m_modelMatrix;


        friend class Renderer;

};  // SceneObject

}  // namespace ogls::renderer

#endif
