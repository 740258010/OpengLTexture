
#include <stdlib.h>
#include <filesystem>
#include <base.h>
#include <mtexture.h>
#include <qimage.h>

using namespace Engine;
namespace Engine
{
    class MyFonts
    {
        public:
            ~MyFonts();

            MyFonts(const char * fontfile);
//            MyFonts(const std::filesystem::path& font);

            const MSDFData* GetMSDFData() const { return m_Data; }
            MTexture* GetAtlasTexture();

            static Ref<MyFonts> GetDefault();

            const msdfgen::BitmapConstRef<msdfgen::byte, 3> &getBitMap() const;

            const TextureSpecification &getSpec() const;

    private:
            MSDFData* m_Data;
            MTexture *m_AtlasTexture;
            TextureSpecification spec;

            msdfgen::BitmapConstRef<msdfgen::byte,3> bitMap;
    };
}


