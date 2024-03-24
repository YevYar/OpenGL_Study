#ifndef OGLS_RENDERER_MESH_H
#define OGLS_RENDERER_MESH_H

#include <string_view>

#include "helpers/macros.h"
#include "mathCore/point.h"
#include "mathCore/vector.h"

namespace ogls::renderer
{
/**
 * \brief Mesh is a base class for all classes, which contain the Vertex data
 * (positions, normals, texture coordinates, etc.) that define the geometry of an object
 * and their connectivity to form triangles.
 */
class Mesh
{
    public:
        /**
         * \brief Vertex represents a vertex in 3D space with position, normal, and texture coordinates.
         */
        struct Vertex
        {
            public:
                constexpr virtual ~Vertex() noexcept = default;

            public:
                /**
                 * \brief The normal vector of the Vertex, used for lighting calculations.
                 */
                mathCore::Vec3    normal;
                /**
                 * \brief The position of the Vertex in 3D space.
                 */
                mathCore::Point<> position;
                /**
                 * \brief The texture coordinates of the Vertex, used for texture mapping.
                 */
                mathCore::Point<> texCoords;

        };  // struct Vertex

    public:
        static Mesh loadMeshFromFile(std::string_view fullFileName);

        Mesh() = default;
        OGLS_DEFAULT_COPYABLE_MOVABLE(Mesh)
        /**
         * \brief Constructs new Mesh using the vector of Vertex and indices.
         *
         * \param vertices - the vector of Vertex defining the geometry of the mesh.
         * \param indices  - the indices defining the connectivity of the Vertex to form triangles.
         */
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) noexcept;
        virtual ~Mesh() = default;

        /**
         * \brief Returns the unique identifier of the Mesh.
         */
        size_t                           getId() const noexcept;
        /**
         * \brief Returns the vector of indices of the Mesh defining the connectivity of the Vertex to form triangles.
         */
        const std::vector<unsigned int>& getIndices() const noexcept;
        /**
         * \brief Returns the vector of Vertex of the Mesh.
         */
        const std::vector<Vertex>&       getVertices() const noexcept;

        /**
         * \brief May be used to call some specific graphics API functions to render this Mesh.
         */
        virtual void render();

    protected:
        /**
         * \brief The indices defining the connectivity of the Vertex to form triangles.
         */
        std::vector<unsigned int> m_indices;
        /**
         * \brief The vector of Vertex defining the geometry of the mesh.
         */
        std::vector<Vertex>       m_vertices;

    private:
        /**
         * \brief The unique identifier of the Mesh.
         *
         * 0 identifies an invalid empty Mesh.
         */
        size_t m_id = {0};

};  // class Mesh

}  // namespace ogls::renderer

#endif
