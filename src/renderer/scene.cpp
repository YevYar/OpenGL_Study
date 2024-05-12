#include "scene.h"

namespace ogls::renderer
{
namespace
{
    /**
     * \brief The counter of created Scene.
     */
    auto sceneCounter = std::atomic<size_t>{1};

}  // namespace

Scene::Scene(std::vector<std::shared_ptr<SceneObject>> sceneObjects) noexcept :
    m_id{sceneCounter.fetch_add(1)}, m_objects{std::move(sceneObjects)}
{
}

const std::vector<std::shared_ptr<SceneObject>>& Scene::getObjects() const noexcept
{
    return m_objects;
}

void Scene::addObject(std::shared_ptr<SceneObject> object)
{
    m_objects.push_back(object);
}

void Scene::removeObject(std::shared_ptr<SceneObject> object)
{
    auto it = std::find(m_objects.begin(), m_objects.end(), object);

    if (it != m_objects.end())
    {
        m_objects.erase(it);
    }
}

void Scene::update(std::chrono::milliseconds deltaTime)
{
    for (auto obj : m_objects)
    {
        obj->update(deltaTime);
    }
}

}  // namespace ogls::renderer
