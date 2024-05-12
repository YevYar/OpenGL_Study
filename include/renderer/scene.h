#ifndef OGLS_RENDERER_SCENE_H
#define OGLS_RENDERER_SCENE_H

#include <chrono>

#include "renderer/sceneObject.h"

namespace ogls::renderer
{
/**
 * \brief Scene is a base abstract class for all classes, which contains various objects and entities that are rendered
 * and updated together.
 */
class Scene
{
    public:
        /**
         * \brief Constructs the Scene with no elements.
         */
        Scene() = default;
        OGLS_DEFAULT_COPYABLE_MOVABLE(Scene)
        /**
         * \brief Constructs new Scene, which contains the passed objects of the type SceneObject.
         *
         * \param sceneObjects - objects to be added to the Scene.
         */
        explicit Scene(std::vector<std::shared_ptr<SceneObject>> sceneObjects) noexcept;
        virtual ~Scene() noexcept = default;

        /**
         * \brief Gets all SceneObject in the Scene.
         *
         * \return a constant reference to the vector of SceneObject pointers.
         */
        const std::vector<std::shared_ptr<SceneObject>>& getObjects() const noexcept;

        /**
         * \brief Adds the SceneObject to the Scene.
         *
         * \param object - the pointer to the SceneObject to add.
         */
        virtual void addObject(std::shared_ptr<SceneObject> object);
        /**
         * \brief Removes the SceneObject from the Scene.
         *
         * \param object - the pointer to the SceneObject to remove.
         */
        virtual void removeObject(std::shared_ptr<SceneObject> object);
        /**
         * \brief Updates the scene.
         *
         * This method is responsible for updating the state of all SceneObject in the Scene.
         *
         * \param deltaTime - the time elapsed since the last frame, in milliseconds.
         */
        virtual void update(std::chrono::milliseconds deltaTime);

    private:
        /**
         * \brief Renders the Scene.
         *
         * \param renderer - the object, which performs a rendering operation.
         */
        virtual void render(std::shared_ptr<Renderer> renderer) = 0;

    protected:
        /**
         * \brief The unique identifier of the Scene.
         *
         * 0 identifies an invalid empty Scene.
         */
        size_t                                    m_id = {0};
        /**
         * \brief The collection of added SceneObject in the Scene.
         */
        std::vector<std::shared_ptr<SceneObject>> m_objects;


        friend class Renderer;

};  // Scene

}  // namespace ogls::renderer

#endif
