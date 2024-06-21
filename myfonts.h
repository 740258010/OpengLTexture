#ifndef MYFONTS_H
#define MYFONTS_H

#include <stdlib.h>
#include <filesystem>
#include <base.h>
#include <wdtextture.h>

using namespace Engine;
namespace Engine
{

    class MyFonts
    {
        public:
            MyFonts();
            ~MyFonts();

            MyFonts(const char * fontfile);
//            MyFonts(const std::filesystem::path& font);

            const MSDFData* GetMSDFData() const { return m_Data; }
            Ref<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }

            static Ref<MyFonts> GetDefault();

        private:
            MSDFData* m_Data;
            Ref<Texture2D> m_AtlasTexture;
    };
}

#endif // MYFONTS_H
