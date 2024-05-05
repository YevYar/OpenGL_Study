#include "sceneObject.h"

namespace ogls::renderer
{
SceneObject::SceneObject(std::shared_ptr<Mesh> mesh, Material material, Size size,
                         mathCore::Point<int> globalPosition) noexcept :
    m_material{std::move(material)},
    m_mesh{std::move(mesh)}
{
}

//------ ACCESSORS TO DIMENSIONAL PROPERTIES

int SceneObject::getHeight() const noexcept
{
    return 0;
}

int SceneObject::getLength() const noexcept
{
    return 0;
}

SceneObject::Size SceneObject::getSize() const noexcept
{
    return Size{};
}

int SceneObject::getWidth() const noexcept
{
    return 0;
}

void SceneObject::setHeight(int height) noexcept
{
}

void SceneObject::setLength(int length) noexcept
{
}

void SceneObject::setSize(Size size) noexcept
{
}

void SceneObject::setSize(int length, int height, int width) noexcept
{
}

void SceneObject::setWidth(int width) noexcept
{
}

//------ ACCESSORS TO POSITION

mathCore::Point<int> SceneObject::getPositionInGlobal() const noexcept
{
    return mathCore::Point<int>{};
}

mathCore::Point<int> SceneObject::getPositionInParent() const noexcept
{
    return mathCore::Point<int>{};
}

void SceneObject::setPositionInGlobal(mathCore::Point<int> position)
{
}

void SceneObject::setPositionInParent(mathCore::Point<int> position)
{
}

//------ ACCESSORS TO VISUAL PROPERTIES

Material& SceneObject::getMaterial() noexcept
{
    return m_material;
}

const Material& SceneObject::getMaterial() const noexcept
{
    return m_material;
}

std::shared_ptr<Mesh> SceneObject::getMesh() const noexcept
{
    return m_mesh;
}

void SceneObject::setMaterial(Material material) noexcept
{
    m_material = std::move(material);
}

void SceneObject::setMesh(std::shared_ptr<Mesh> mesh) noexcept
{
    m_mesh = mesh;
}

}  // namespace ogls::renderer
