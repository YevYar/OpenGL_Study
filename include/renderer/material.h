#ifndef OGLS_RENDERER_MATERIAL_H
#define OGLS_RENDERER_MATERIAL_H

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "mathCore/matrix.h"
#include "mathCore/vector.h"
#include "openglCore/textureTypes.h"
#include "renderer/renderer.h"

namespace ogls::renderer
{
/**
 * \brief Defines the types, which can be used for shader variables.
 *
 * \note The list may be extended in the future.
 */
// clang-format off
#define OGLS_SHADER_VARIABLE_TYPES                                                                           \
    bool, int, unsigned int, float, double,                                                                  \
    ogls::mathCore::Vector<2, bool>, ogls::mathCore::Vector<3, bool>, ogls::mathCore::Vector<4, bool>,       \
    ogls::mathCore::Vector<2, int>, ogls::mathCore::Vector<3, int>, ogls::mathCore::Vector<4, int>,          \
    ogls::mathCore::Vector<2, unsigned int>, ogls::mathCore::Vector<3, unsigned int>,                        \
    ogls::mathCore::Vector<4, unsigned int>,                                                                 \
    ogls::mathCore::Vector<2, float>, ogls::mathCore::Vector<3, float>, ogls::mathCore::Vector<4, float>,    \
    ogls::mathCore::Vector<2, double>, ogls::mathCore::Vector<3, double>, ogls::mathCore::Vector<4, double>, \
    ogls::mathCore::Matrix<2, 2>, ogls::mathCore::Matrix<2, 3>, ogls::mathCore::Matrix<2, 4>,                \
    ogls::mathCore::Matrix<3, 2>, ogls::mathCore::Matrix<3, 3>, ogls::mathCore::Matrix<3, 4>,                \
    ogls::mathCore::Matrix<4, 2>, ogls::mathCore::Matrix<4, 3>, ogls::mathCore::Matrix<4, 4>,                \
    Material::Color

// clang-format on

/**
 * \brief Material represents a material used for rendering objects and defines how the Mesh looks like.
 *
 * This class encapsulates the properties and behavior of a material, including shader variables, textures, and
 * rendering methods.
 */
class Material
{
    public:
        /**
         * \brief Color represents RGBA color values.
         */
        struct Color final
        {
            public:
                float r = {0.0f};
                float g = {0.0f};
                float b = {0.0f};
                float a = {0.0f};

        };  // struct Color

    public:
        OGLS_DEFAULT_COPYABLE_MOVABLE(Material)
        /**
         * \brief Constructs a Material object.
         *
         * \note Material -s with the same pathToVertexShader and pathToFragmentShader are considered to be using the
         * same shader program. That is why Renderer will use the same shader program for them, it won't load shaders
         * twice from the same file paths.
         * \param pathToVertexShader   - the path to the vertex shader file.
         * \param pathToFragmentShader - the path to the fragment shader file.
         * \param shaderVariables      - the map of shader variable names to their values.
         * \param textures             - the map of texture names to their TextureData.
         */
        Material(std::string pathToVertexShader, std::string pathToFragmentShader,
                 std::unordered_map<std::string, std::variant<OGLS_SHADER_VARIABLE_TYPES>>       shaderVariables = {},
                 std::unordered_map<std::string, std::shared_ptr<oglCore::texture::TextureData>> textures =
                   {}) noexcept;
        virtual ~Material() = default;

        /**
         * \brief Retrieves the Color associated with a shader variable.
         *
         * It's equal to std::get<Color>(getShaderVariableValue(shaderVariableName)).
         *
         * \param shaderVariableName - the name of the shader variable.
         * \see getShaderVariableValue().
         * \return the Color associated with the shader variable.
         * \throw std::out_of_range (if the variable with a such name doesn't exist),
         * std::bad_variant_access (if the variable with a such name isn't a Color).
         */
        Color                                    getColor(std::string_view shaderVariableName) const;
        /**
         * \brief Returns the path to the fragment shader file.
         */
        std::string_view                         getPathToFragmentShader() const noexcept;
        /**
         * \brief Returns the path to the vertex shader file.
         */
        std::string_view                         getPathToVertexShader() const noexcept;
        /**
         * \brief Retrieves the value of the shader variable by name.
         *
         * \param shaderVariableName - the name of the shader variable.
         * \return the shader variable's value.
         * \throw std::out_of_range.
         */
        std::variant<OGLS_SHADER_VARIABLE_TYPES> getShaderVariableValue(std::string_view shaderVariableName) const;
        /**
         * \brief Returns the map of shader variable names and their values.
         */
        const std::unordered_map<std::string, std::variant<OGLS_SHADER_VARIABLE_TYPES>>&
                                                       getShaderVariables() const noexcept;
        /**
         * \brief Retrieves the texture by name.
         *
         * \param textureName - the name of the texture.
         * \return the TextureData associated with the texture name.
         * \throw std::out_of_range.
         */
        std::shared_ptr<oglCore::texture::TextureData> getTextureData(std::string_view textureName) const;
        /**
         * \brief Returns the map of texture names and their TextureData.
         */
        const std::unordered_map<std::string, std::shared_ptr<oglCore::texture::TextureData>>&
                     getTextures() const noexcept;
        /**
         * \brief May be used to call some specific graphics API functions to render this Material.
         *
         * It may take textures from a Renderer object to set specific texture parameters.
         */
        virtual void render();
        /**
         * \brief Sets the Ñolor associated with a shader variable.
         *
         * It's equal to setShaderVariableValue(shaderVariableName, color).
         *
         * \param shaderVariableName - the name of the shader variable.
         * \param color              - the Color to set.
         * \see setShaderVariableValue().
         */
        void         setColor(std::string_view shaderVariableName, Color color);
        /**
         * \brief Sets the value of a shader variable.
         *
         * \param name  - the name of the shader variable.
         * \param value - the value to set.
         */
        void         setShaderVariableValue(std::string_view                         shaderVariableName,
                                            std::variant<OGLS_SHADER_VARIABLE_TYPES> value);
        /**
         * \brief Sets the data of the texture by its name.
         *
         * \param textureName - the name of the texture.
         * \param data        - the TextureData object to set.
         */
        void         setTextureData(std::string_view textureName, std::shared_ptr<oglCore::texture::TextureData> data);

    protected:
        /**
         * \brief Cleans the list of shader variables and textures, which must be updated by Renderer.
         */
        virtual void setUpdatedState();

    protected:
        /**
         * \brief The unique identifier of the Material.
         *
         * It can be used by Renderer to group Material -s, which use the same shader program.
         * The Renderer sets this value itself. The Material shouldn't set this value itself.
         * However, Material may use this ID to retrieve some objects (for example, textures) from Renderer
         * to set some material parameters depending on the used graphical API.
         *
         * 0 identifies an invalid empty Material.
         */
        size_t m_id = {0};

    private:
        /**
         * \brief Path to the fragment shader file.
         */
        std::string                                                                     m_pathToFragmentShader;
        /**
         * \brief Path to the vertex shader file.
         */
        std::string                                                                     m_pathToVertexShader;
        /**
         * \brief Map of shader variables.
         */
        std::unordered_map<std::string, std::variant<OGLS_SHADER_VARIABLE_TYPES>>       m_shaderVariables;
        /**
         * \brief List of shader variables whose values must be updated by the Renderer on the next render.
         */
        std::vector<std::string>                                                        m_shaderVariablesToUpdate;
        /**
         * \brief Map of textures, which are used by Material.
         */
        // TODO: Replace oglCore::texture::TextureTarget and oglCore::texture::TextureData
        // with some general for OpenGL and Vulkan types.
        std::unordered_map<std::string, std::shared_ptr<oglCore::texture::TextureData>> m_textures;
        /**
         * \brief List of textures whose data must be updated by the Renderer on the next render.
         */
        std::vector<std::string>                                                        m_texturesToUpdate;


        friend class Renderer;

};  // class Material

}  // namespace ogls::renderer

#endif
