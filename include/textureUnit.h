#ifndef TEXTURE_UNIT_H
#define TEXTURE_UNIT_H

#include <functional>
#include <memory>
#include <vector>

namespace texture
{
    class Texture;

    class TextureUnit
    {
        public:
            TextureUnit();

            static void unbind();
            static void updateEvery(const std::vector<std::shared_ptr<TextureUnit>>& textureUnits,
                std::function<void()> updater);

            void addTexture(std::shared_ptr<Texture> texture);
            void bind();

        private:
            static TextureUnit* activeTU = nullptr;

    };
}

#endif
