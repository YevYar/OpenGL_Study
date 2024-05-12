#ifndef OGLS_RENDERER_MESH_H
#define OGLS_RENDERER_MESH_H

#include <string_view>
#include <variant>

#include "helpers/macros.h"
#include "mathCore/point.h"
#include "mathCore/vector.h"
#include "renderer/renderer.h"

namespace ogls::renderer
{
/**
 * \brief Defines the types, which can be used as type of vertices data.
 *
 * \note The list may be extended in the future.
 */
#define OGLS_VERTICES_DATA_TYPES signed char, unsigned char, short, unsigned short, int, unsigned int, float, double

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
        struct Vertex final
        {
            public:
                /**
                 * \brief The normal vector of the Vertex, used for lighting calculations.
                 */
                mathCore::Vec3                                                                       normal;
                /**
                 * \brief The other data, which must be passed to the Vertex.
                 */
                std::unordered_map<std::string, std::vector<std::variant<OGLS_VERTICES_DATA_TYPES>>> otherData;
                /**
                 * \brief The position of the Vertex in 3D space.
                 */
                mathCore::Point<float>                                                               position;
                /**
                 * \brief The texture coordinates of the Vertex, used for texture mapping.
                 */
                mathCore::Point<float>                                                               texCoords;

        };  // struct Vertex

    public:
        static Mesh loadMeshFromFile(std::string_view fullFileName);

        /**
         * \brief Constructs an invalid Mesh that doesn't represent any geometry.
         */
        Mesh() = default;
        OGLS_DEFAULT_COPYABLE_MOVABLE(Mesh)
        /**
         * \brief Constructs new Mesh using the vector of Vertex and indices.
         *
         * It will have a unique ID, even if another Mesh with the same vertices and indices has been created previously.
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

    private:
        /**
         * \brief May be used to call some specific graphics API functions to render this Mesh.
         *
         * \param renderer - the object, which performs a rendering operation.
         */
        virtual void render(std::shared_ptr<Renderer> renderer);

    protected:
        /**
         * \brief The unique identifier of the Mesh.
         *
         * 0 identifies an invalid empty Mesh.
         */
        size_t m_id = {0};

    private:
        /**
         * \brief The indices defining the connectivity of the Vertex to form triangles.
         */
        std::vector<unsigned int> m_indices;
        /**
         * \brief The vector of Vertex defining the geometry of the mesh.
         */
        std::vector<Vertex>       m_vertices;


        friend class Renderer;

};  // class Mesh

}  // namespace ogls::renderer

#endif
