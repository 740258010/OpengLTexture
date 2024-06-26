#include "myfonts.h"

namespace Engine
{

    template<typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
    static Engine::TextureSpecification CreateAndCacheAtlas(const std::string& fontName, float fontSize, const std::vector<msdf_atlas::GlyphGeometry>& glyphs,
        const msdf_atlas::FontGeometry& fontGeometry, uint32_t width, uint32_t height)
    {
        msdf_atlas::GeneratorAttributes attributes;
        attributes.config.overlapSupport = true;
        attributes.scanlinePass = true;

        msdf_atlas::ImmediateAtlasGenerator<S, N, GenFunc, msdf_atlas::BitmapAtlasStorage<T, N>> generator(width, height);
        generator.setAttributes(attributes);
        generator.setThreadCount(8);
        generator.generate(glyphs.data(), (int)glyphs.size());

        msdfgen::BitmapConstRef<T, N> bitmap = (msdfgen::BitmapConstRef<T, N>)generator.atlasStorage();
        const float *data = bitmap.pixels;

        Engine::TextureSpecification spec;
        spec.Width = bitmap.width;
        spec.Height = height;
        spec.Format = Engine::ImageFormat::RGB8;
        spec.GenerateMips = false;
        spec.pixels  = (void*)data;
        return spec;
    }

    MyFonts::MyFonts(const char * fontfile)
        : m_Data(new MSDFData())
    {
        m_AtlasTexture = new MTexture(QOpenGLTexture::Target2D);
        msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
//        HZ_CORE_ASSERT(ft);

//        std::string fileString = filepath.string();

        // TODO(Yan): msdfgen::loadFontData loads from memory buffer which we'll need
//        msdfgen::FontHandle* font = msdfgen::loadFont(ft, fileString.c_str());
        msdfgen::FontHandle* font = msdfgen::loadFont(ft, fontfile);
        if (!font)
        {
//            HZ_CORE_ERROR("Failed to load font: {}", fileString);
            return;
        }

        struct CharsetRange
        {
            uint32_t Begin, End;
        };

        // From imgui_draw.cpp
        static const CharsetRange charsetRanges[] =
        {
//            { 0x0020, 0x00FF }

            {0x20,0x7F            }
        };

        msdf_atlas::Charset charset;
        for (CharsetRange range : charsetRanges)
        {
            for (uint32_t c = range.Begin; c <= range.End; c++)
                charset.add(c);
        }

        double fontScale = 1.0;
        m_Data->FontGeometry = msdf_atlas::FontGeometry(&m_Data->Glyphs);
        int glyphsLoaded = m_Data->FontGeometry.loadCharset(font, fontScale, charset);
//        HZ_CORE_INFO("Loaded {} glyphs from font (out of {})", glyphsLoaded, charset.size());


        double emSize = 40.0;

        msdf_atlas::TightAtlasPacker atlasPacker;
        // atlasPacker.setDimensionsConstraint()
        atlasPacker.setPixelRange(2.0);
        atlasPacker.setMiterLimit(1.0);
//        atlasPacker.setPadding(0);
        atlasPacker.setScale(emSize);
        int remaining = atlasPacker.pack(m_Data->Glyphs.data(), (int)m_Data->Glyphs.size());
//        HZ_CORE_ASSERT(remaining == 0);

        int width, height;
        atlasPacker.getDimensions(width, height);
        emSize = atlasPacker.getScale();

#define DEFAULT_ANGLE_THRESHOLD 3.0
#define LCG_MULTIPLIER 6364136223846793005ull
#define LCG_INCREMENT 1442695040888963407ull
#define THREAD_COUNT 8
        // if MSDF || MTSDF

        uint64_t coloringSeed = 0;
        bool expensiveColoring = false;
        if (expensiveColoring)
        {
            msdf_atlas::Workload([&glyphs = m_Data->Glyphs, &coloringSeed](int i, int threadNo) -> bool {
                unsigned long long glyphSeed = (LCG_MULTIPLIER * (coloringSeed ^ i) + LCG_INCREMENT) * !!coloringSeed;
                glyphs[i].edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
                return true;
                }, m_Data->Glyphs.size()).finish(THREAD_COUNT);
        }
        else {
            unsigned long long glyphSeed = coloringSeed;
            for (msdf_atlas::GlyphGeometry& glyph : m_Data->Glyphs)
            {
                glyphSeed *= LCG_MULTIPLIER;
                glyph.edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
            }
        }


       spec =  CreateAndCacheAtlas<float, float, 3, msdf_atlas::msdfGenerator>("Test", (float)emSize, m_Data->Glyphs, m_Data->FontGeometry, width, height);

//       saveBmp(data,"test.bmp");
#if 0
        msdfgen::Shape shape;
        if (msdfgen::loadGlyph(shape, font, 'C'))
        {
            shape.normalize();
            //                      max. angle
            msdfgen::edgeColoringSimple(shape, 3.0);
            //           image width, height
            msdfgen::Bitmap<float, 3> msdf(32, 32);
            //                     range, scale, translation
            msdfgen::generateMSDF(msdf, shape, 4.0, 1.0, msdfgen::Vector2(4.0, 4.0));
            msdfgen::savePng(msdf, "output.png");
        }
#endif

        msdfgen::destroyFont(font);
        msdfgen::deinitializeFreetype(ft);
    }

    MTexture* MyFonts::GetAtlasTexture()
    {
        return m_AtlasTexture;
    }

    MyFonts::~MyFonts()
    {
        delete m_Data;
        delete m_AtlasTexture;
    }


    Ref<MyFonts> MyFonts::GetDefault()
    {
        static Ref<MyFonts> DefaultFont;
        if (!DefaultFont)
            DefaultFont = CreateRef<MyFonts>("../OpenglTextRender/fonts/arial.ttf");

        return DefaultFont;
    }

    const msdfgen::BitmapConstRef<msdfgen::byte,3> &MyFonts::getBitMap() const
    {
        return bitMap;
    }

    const TextureSpecification &MyFonts::getSpec() const
    {
        return spec;
    }

}
