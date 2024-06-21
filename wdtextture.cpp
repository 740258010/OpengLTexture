#include "wdtextture.h"
#include "opengltexture.h"
namespace Engine {

    Ref<Texture2D> Texture2D::Create(const TextureSpecification& specification)
    {

        return  CreateRef<OpenglTexture>(specification);

    }

    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {

        return CreateRef<OpenglTexture>(path);

    }

}
