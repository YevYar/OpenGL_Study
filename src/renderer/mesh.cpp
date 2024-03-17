#include "mesh.h"

#include <atomic>

namespace ogls::renderer
{
namespace
{
    /**
     * \brief The counter of created Mesh.
     */
    auto meshCounter = std::atomic<size_t>{1};

}  // namespace

// TODO: Check m_id
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) noexcept :
    m_id{meshCounter.fetch_add(1)}, m_indices{std::move(indices)}, m_vertices{std::move(vertices)}
{
}

Mesh Mesh::loadMeshFromFile(std::string_view fullFileName)
{
    return Mesh{};
}

size_t Mesh::getId() const noexcept
{
    return m_id;
}

const std::vector<unsigned int>& Mesh::getIndices() const noexcept
{
    return m_indices;
}

const std::vector<Mesh::Vertex>& Mesh::getVertices() const noexcept
{
    return m_vertices;
}

void Mesh::render()
{
}

}  // namespace ogls::renderer
