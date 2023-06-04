#ifndef APP_RENDERER_SCENE_OBJECT_H
#define APP_RENDERER_SCENE_OBJECT_H

#include <memory>
#include <vector>

#include "helpers/macros.h"
#include "shaderProgram.h"
#include "textureUnit.h"
#include "vertexArray.h"

namespace app::renderer
{
/**
 * \brief SceneObject is a base abstract class for all classes, which represents something, what can be rendered.
 */
class SceneObject
{
    public:
        SceneObject() = default;
        OGLS_DEFAULT_COPYABLE_MOVABLE(SceneObject)
        /**
         * \brief Constructs new SceneObject using a vertex array object and a shader program.
         *
         * \param vao           - a vertex array object, which contains necessary vertex configuration.
         * \param shaderProgram - a shader program, which is used for rendering.
         */
        SceneObject(std::shared_ptr<ogls::oglCore::vertex::VertexArray>   vao,
                    std::shared_ptr<ogls::oglCore::shader::ShaderProgram> shaderProgram);
        virtual ~SceneObject() noexcept = default;

        /**
         * \brief Sets a configuration of texture units, which is used for rendering of the object.
         *
         * \param texturesConfiguration - a configuration of texture units.
         */
        virtual void setTexturesConfiguration(ogls::oglCore::texture::TexturesConfiguration texturesConfiguration);

        /**
         * \brief Renders something on the scene.
         */
        virtual void render() = 0;

    protected:
        /**
         * \brief Applies set textures configuration to be used while the following rendering.
         *
         * See ogls::oglCore::texture::applyTexturesConfiguration().
         */
        void applyTexturesConfiguration();

    protected:
        /**
         * \brief Shader program, which is used to render this object.
         */
        std::shared_ptr<ogls::oglCore::shader::ShaderProgram> m_shaderProgram = nullptr;
        /**
         * \brief Configuration of texture units, which is used to render this object.
         */
        ogls::oglCore::texture::TexturesConfiguration         m_texturesConfiguration;
        /**
         * \brief Vertex array object, which is used to render this object.
         */
        std::shared_ptr<ogls::oglCore::vertex::VertexArray>   m_vao = nullptr;

};  // SceneObject

}  // namespace app::renderer

#endif
