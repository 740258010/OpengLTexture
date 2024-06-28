#include "myopenglwidget.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "filesystem"
#pragma once

float vertices[] = {
//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // 右上
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // 右下
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // 左下
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // 左上
};

unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};


MyOpenglWidget::MyOpenglWidget()
{
    QString dirStr = "../OpenglTextRender/fonts/arial.ttf";
    QByteArray ba = dirStr.toLatin1();
    char* font_file = ba.data();
//    generateAtlas(font_file);
//    font = new MyFonts(font_file);
    spec = new TextureSpecification;
    generateAtlas1(font_file);
}

MyOpenglWidget::~MyOpenglWidget()
{
    delete spec;
}

void MyOpenglWidget::initializeGL()
{
    initializeOpenGLFunctions();

    //初始化view矩阵
    view.setToIdentity();
    QVector3D camPos(0.0,0.0,farP);
    QVector3D camDirec(0.0,0.0,0.0);
    QVector3D camUp(0.0,1.0,0.0);
    view.lookAt(camPos,camDirec,camUp);
    model.setToIdentity();

    view.translate(0,0,90);

    //文字VAO、VBO
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);

       glGenVertexArrays(1, &VAO);
       glGenBuffers(1, &VBO);
       glGenBuffers(1, &EBO);

       glBindVertexArray(VAO);

       glBindBuffer(GL_ARRAY_BUFFER, VBO);
       glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
       glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

       // position attribute
       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
       glEnableVertexAttribArray(0);
//        color attribute
       glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
       glEnableVertexAttribArray(1);
//        texture coord attribute
       glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
       glEnableVertexAttribArray(1);

       glGenTextures(1, &texture1);
       glBindTexture(GL_TEXTURE_2D, texture1);
       // 设置纹理选项
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//       TextureSpecification spec = font->getSpec();


//       msdfgen::BitmapConstRef<float, 3> map;
//       for (int y = 0; y < bitmap1.height; ++y)
//       {
//           for (int x = 0; x < bitmap1.width; ++x)
//           {
//              const msdfgen::byte* p =  bitmap1(x,y);
//              const float* dd = map(x,y) ;
//              *dd = pixelByteToFloat(*p);
//           }
//       }

//       msdfgen::BitmapConstRef<float, 3> map;
//       const unsigned char *end = bitmap1.pixels+3*bitmap1.width*bitmap1.height;
//       for (const unsigned char *p = bitmap1.pixels; p < end; ++p)
//            float x = pixelByteToFloat(pixelFloatToByte(*p));


//       QString dirStr = "../OpenglTextRender/fonts/arial.ttf";
////        QString dirStr = "C:/Windows/Fonts/simfang.ttf";
//       QByteArray ba = dirStr.toLatin1();
//       char* font_file = ba.data();
//       generateAtlas1(font_file);
//       generateAtlas(font_file);
//       saveBmp(*bitmap3,"tp.bmp");
//        const float * datas2 = bitmap3->pixels;

//       msdfgen::Bitmap<float, 3> msdf(40, 40);
//       msdf_atlas::GlyphGeometry geo = font->GetMSDFData()->Glyphs.at(20);
//       msdfgen::Shape shape = geo.getShape();
//       msdfgen::Projection  pro = geo.getBoxProjection();
//       msdfgen::Range range = geo.getBoxRange();
//       msdfgen::SDFTransformation t(pro, range);
//       generateMSDF(msdf, shape, t);
//       msdfgen::BitmapConstRef<float, 3> bitMap =  msdf;
//       const float * datas = bitMap.pixels;



//       std::string path = "C:/Users/pczhaojue/Documents/OpenglTextRender1/outputdd.bmp";
////       std::string path = "C:/Users/pczhaojue/Documents/OpenglTextRender1/test.jpg";
////       QByteArray ba = dirStr.toLatin1();
////       char* font_file = ba.data();
//       int width, height, channels;
////       stbi_set_flip_vertically_on_load(1);
//       stbi_uc* data1 = nullptr;
//       {
//           FILE *f = stbi__fopen(path.c_str(), "rb");
//           if (!f)
//           {
//                qDebug() << "Unable to open file" <<endl;
//           }

////            HZ_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
//           data1 = stbi_load(path.c_str(), &width, &height, &channels, 0);
//           if (data1)
//           {
//               glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
//               glGenerateMipmap(GL_TEXTURE_2D);
//           }
//           else
//           {
//               qDebug() << "Failed to load texture" <<endl;
//           }
//       }
//       stbi_image_free(data1);

//       glTexImage2D(
//                   GL_TEXTURE_2D,
//                   0,
//                   GL_RGB,
//                   spec->Width,
//                   spec->Height,
//                   0,
//                   GL_RGB,
//                   GL_FLOAT,
//                   spec->pixels
//                   );



    program.addShaderFromSourceFile(QOpenGLShader::Vertex,":/textVert.vsh");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment,":/textFshader.fsh");
//    program.addShaderFromSourceFile(QOpenGLShader::Vertex,":/textfontvertex.vsh");
//    program.addShaderFromSourceFile(QOpenGLShader::Fragment,":/textfontshader.fsh");
}

void MyOpenglWidget::resizeGL(int w, int h)
{
    proj.setToIdentity();
    proj.perspective(
                    /* vertical angle */ fov,
                    /* aspect ratio */  w / float(h),
                    /* near */           nearP,
                    /* far */            farP);
}

void MyOpenglWidget::paintGL()
{
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glBindTexture(GL_TEXTURE_2D, texture1);
     m_worldToView =  proj * view * model;       // bind Texture
     program.bind();
     program.setUniformValue("worldToView",m_worldToView);

     glBindVertexArray(VAO);
     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

//     RenderText(font->GetMSDFData()->FontGeometry,"HelloWorld!",0,0);

//     update();
}

using namespace msdfgen;
msdfgen::BitmapConstRef<float, 3>* MyOpenglWidget::generateAtlas(const char *fontFilename)
{
//    makeCurrent();
    msdfgen::BitmapConstRef<float, 3>* bitMap = new msdfgen::BitmapConstRef<float, 3>;
    bool success = false;
    // Initialize instance of FreeType library
    if (msdfgen::FreetypeHandle *ft = msdfgen::initializeFreetype()) {
        // Load font file
        if (msdfgen::FontHandle *font = msdfgen::loadFont(ft, fontFilename)) {            
            // Storage for glyph geometry and their coordinates in the atlas
            std::vector<GlyphGeometry> glyphs;
            // FontGeometry is a helper class that loads a set of glyphs from a single font.
            // It can also be used to get additional font metrics, kerning information, etc.
            FontGeometry fontGeometry(&glyphs);
            // Load a set of character glyphs:
            // The second argument can be ignored unless you mix different font sizes in one atlas.
            // In the last argument, you can specify a charset other than ASCII.
            // To load specific glyph indices, use loadGlyphs instead.
            fontGeometry.loadCharset(font, 1.0, Charset::ASCII);

            msdfgen::FontMetrics metrix = fontGeometry.getMetrics();
            // Apply MSDF edge coloring. See edge-coloring.h for other coloring strategies.
            const double maxCornerAngle = 3.0;
//            for (GlyphGeometry &glyph : glyphs)
//                glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);
//            glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);
            // TightAtlasPacker class computes the layout of the atlas.
            double emSize = 80;
            TightAtlasPacker packer;
            // Set atlas parameters:
            // setDimensions or setDimensionsConstraint to find the best value
            packer.setDimensionsConstraint(msdf_atlas::DimensionsConstraint::SQUARE);
            // setScale for a fixed size or setMinimumScale to use the largest that fits
//            packer.setMinimumScale(48.0);
            // setPixelRange or setUnitRange
            packer.setPixelRange(2);
            packer.setMiterLimit(1.0);
            packer.setScale(emSize);
            // Compute atlas layout - pack glyphs
            packer.pack(glyphs.data(), glyphs.size());
            // Get final atlas dimensions
            int width = 0, height = 0;
            packer.getDimensions(width, height);
            emSize = packer.getScale();

//            createAndCatchAtlas((float)emSize,&fontGeometry,&glyphs,width,height);
            // The ImmediateAtlasGenerator class facilitates the generation of the atlas bitmap.

            ImmediateAtlasGenerator<
                float, // pixel type of buffer for individual glyphs depends on generator function
                3, // number of atlas color channels
                &msdfGenerator, // function to generate bitmaps for individual glyphs
                BitmapAtlasStorage<float, 3> // class that stores the atlas bitmap
                // For example, a custom atlas storage class that stores it in VRAM can be used.
            > generator(width, height);
            // GeneratorAttributes can be modified to change the generator's default settings.
            GeneratorAttributes attributes;
            generator.setAttributes(attributes);
            generator.setThreadCount(4);
            // Generate atlas bitmap
            generator.generate(glyphs.data(), glyphs.size());


            // The atlas bitmap can now be retrieved via atlasStorage as a BitmapConstRef.
            // The glyphs array (or fontGeometry) contains positioning data for typesetting text.
//            success = submitAtlasBitmapAndLayout(generator.atlasStorage(), glyphs);
            // Cleanup

//            BitmapAtlasStorage<byte, 3> storage = generator.atlasStorage();
            *bitMap =  (msdfgen::BitmapConstRef<float, 3>)generator.atlasStorage();
//            RenderText(bitMap,fontGeometry,program,"AB",0.0,0.0);
//            saveBmp(*bitMap,"ff.bmp");

            //生成纹理
            glGenTextures(1, &texture1);
            glBindTexture(GL_TEXTURE_2D, texture1);
            // 设置纹理选项
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGB,
                bitMap->width,
                bitMap->height,
                0,
                GL_RGB,
                GL_FLOAT,
                bitMap->pixels
            );

            msdfgen::destroyFont(font);
        }
        msdfgen::deinitializeFreetype(ft);
    }
//    doneCurrent();
    return bitMap;
}

Engine::TextureSpecification* MyOpenglWidget::generateAtlas1(const char *fontFilename)
{

    //    makeCurrent();
    msdfgen::BitmapConstRef<float, 3>* bitMap = new msdfgen::BitmapConstRef<float, 3>;
    bool success = false;
    // Initialize instance of FreeType library
    if (msdfgen::FreetypeHandle *ft = msdfgen::initializeFreetype()) {
        // Load font file
        if (msdfgen::FontHandle *font = msdfgen::loadFont(ft, fontFilename)) {
            // Storage for glyph geometry and their coordinates in the atlas
            std::vector<GlyphGeometry> glyphs;
            // FontGeometry is a helper class that loads a set of glyphs from a single font.
            // It can also be used to get additional font metrics, kerning information, etc.
            FontGeometry fontGeometry(&glyphs);
            // Load a set of character glyphs:
            // The second argument can be ignored unless you mix different font sizes in one atlas.
            // In the last argument, you can specify a charset other than ASCII.
            // To load specific glyph indices, use loadGlyphs instead.
            fontGeometry.loadCharset(font, 1.0, Charset::ASCII);

            msdfgen::FontMetrics metrix = fontGeometry.getMetrics();
            // Apply MSDF edge coloring. See edge-coloring.h for other coloring strategies.
            const double maxCornerAngle = 3.0;
            //            for (GlyphGeometry &glyph : glyphs)
            //                glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);
            //            glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);
            // TightAtlasPacker class computes the layout of the atlas.
            double emSize = 80;
            TightAtlasPacker packer;
            // Set atlas parameters:
            // setDimensions or setDimensionsConstraint to find the best value
            packer.setDimensionsConstraint(msdf_atlas::DimensionsConstraint::SQUARE);
            // setScale for a fixed size or setMinimumScale to use the largest that fits
            //            packer.setMinimumScale(48.0);
            // setPixelRange or setUnitRange
            packer.setPixelRange(2);
            packer.setMiterLimit(1.0);
            packer.setScale(emSize);
            // Compute atlas layout - pack glyphs
            packer.pack(glyphs.data(), glyphs.size());
            // Get final atlas dimensions
            int width = 0, height = 0;
            packer.getDimensions(width, height);
            emSize = packer.getScale();

            //            createAndCatchAtlas((float)emSize,&fontGeometry,&glyphs,width,height);
            // The ImmediateAtlasGenerator class facilitates the generation of the atlas bitmap.

            ImmediateAtlasGenerator<
                    float, // pixel type of buffer for individual glyphs depends on generator function
                    3, // number of atlas color channels
                    &msdfGenerator, // function to generate bitmaps for individual glyphs
                    BitmapAtlasStorage<float, 3> // class that stores the atlas bitmap
                    // For example, a custom atlas storage class that stores it in VRAM can be used.
                    > generator(width, height);
            // GeneratorAttributes can be modified to change the generator's default settings.
            GeneratorAttributes attributes;
            generator.setAttributes(attributes);
            generator.setThreadCount(4);
            // Generate atlas bitmap
            generator.generate(glyphs.data(), glyphs.size());


            msdfgen::BitmapConstRef<float, 3> bitmap = (msdfgen::BitmapConstRef<float, 3>)generator.atlasStorage();
            const float *data = bitmap.pixels;


            spec->Width = bitmap.width;
            spec->Height = height;
            spec->Format = Engine::ImageFormat::RGB8;
            spec->GenerateMips = false;
            spec->pixels  = (void*)data;

            msdfgen::destroyFont(font);
        }
        msdfgen::deinitializeFreetype(ft);
    }
    //    doneCurrent();
    return spec;
}

//void MyOpenglWidget::add_text_msdf2(Font *font, VertexBuffer *vb, const char* text, float xpos, float ypos)
QList<VertexBuffer*> MyOpenglWidget::add_text_msdf2(FontGeometry geo, QList<VertexBuffer*> list, const char* text, float xpos, float ypos)
{
//    makeCurrent();
    list.clear();
    int font_size = 24;
    msdfgen::FontMetrics fontMetrics = geo.getMetrics();
    double fsScale = 1.0/(fontMetrics.ascenderY-fontMetrics.descenderY);
    size_t num_glyph = 0;
    for (const char *cp = text; *cp; ++cp)
    {
        if (*cp == '\r')
            continue;
        const GlyphGeometry *glyph = geo.getGlyph(*cp);
        if (glyph)
        {
//            if (!glyph->isWhitespace())
//            {
                double pl, pb, pr, pt;
                double il, ib, ir, it;
                glyph->getQuadPlaneBounds(pl, pb, pr, pt);
                glyph->getQuadAtlasBounds(il, ib, ir, it);

                float x0  = xpos + pl * font_size; // 24.0
                float y0  = ypos - pt * font_size;
                float x1  = xpos + pr * font_size;
                float y1  = ypos - pb * font_size;;

                // UV Coordinates
                float s0 = il / 100;
                float t0 = it / 100;
                float s1 = ir / 100;
                float t1 = ib / 100;
//                float s0 = il / font->atlasWidth;
//                float t0 = it / font->atlasHeight;
//                float s1 = ir / font->atlasWidth;
//                float t1 = ib / font->atlasHeight;

//                GLuint ioff = num_glyph * 4;

//                GLuint indices[] = {
//                    0+ioff, 1+ioff, 3+ioff, // first triangle
//                    1+ioff, 2+ioff, 3+ioff
//                }; // Second triangle

//                QVector<int> *indic = new QVector<int>;
//                indic->append(0);
//                indic->append(1);
//                indic->append(3);
//                indic->append(1);
//                indic->append(2);
//                indic->append(3);
////                indic->append(0+ioff);
////                indic->append(1+ioff);
////                indic->append(3+ioff);
////                indic->append(1+ioff);
////                indic->append(2+ioff);
////                indic->append(3+ioff);

                GLfloat vertices[] =
                {
                    x0, y1, 0,  s0, t1, // top right
                    x1, y1, 0,  s1, t1, // bottom right
                    x1, y0, 0,  s1, t0,
                    x0, y0, 0,  s0, t0 // top left
                }; // bottom left

//                QVector<GLfloat> *vert= new QVector<GLfloat>;;
//                vert->append(x0);
//                vert->append(y1);
//                vert->append(0);
//                vert->append(s0);
//                vert->append(t1);

//                vert->append(x1);
//                vert->append(y1);
//                vert->append(0);
//                vert->append(s1);
//                vert->append(t1);

//                vert->append(x1);
//                vert->append(y0);
//                vert->append(0);
//                vert->append(s1);
//                vert->append(t0);

//                vert->append(x0);
//                vert->append(y0);
//                vert->append(0);
//                vert->append(s0);
//                vert->append(t0);


                Bitmap<float, 3> msdf1(100, 100);

//                GlyphGeometry geo = vec[i];
                msdfgen::Shape shape = glyph->getShape();
                msdfgen::Projection  pro = glyph->getBoxProjection();
                msdfgen::Range range = glyph->getBoxRange();
                SDFTransformation t(pro, range);

        //        MSDFGeneratorConfig config;
                generateMSDF(msdf1, shape, t);
//                saveBmp(msdf1,"output.bmp");
                msdfgen::BitmapConstRef<float, 3> bitMap =  msdf1;
                const float * datas = bitMap.pixels;

                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    msdf1.width(),
                    msdf1.height(),
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    datas
                );
                glGenerateMipmap(GL_TEXTURE_2D);

                VertexBuffer *buffer = new VertexBuffer;
                GLuint VAO;
//                vb->push_back(vertices, 20, indices, 6);
//                buffer->dindices = *indic;
//                buffer->dvertices = *vert;
                buffer->VAO = VAO;
                list.append(buffer);
//            }
            double advance = glyph->getAdvance();
            xpos+= advance * font_size;
            num_glyph++;

        }
    }
//    doneCurrent();
    return list;
}

void MyOpenglWidget::RenderText(FontGeometry geo,const char *text, float xpos, float ypos)
{
    qDebug()<<"123";
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    int font_size = 24;
    msdfgen::FontMetrics fontMetrics = geo.getMetrics();
    double fsScale = 1.0/(fontMetrics.ascenderY-fontMetrics.descenderY);
    size_t num_glyph = 0;
    for (const char *cp = text; *cp; ++cp)
    {
        if (*cp == '\r')
            continue;
        const GlyphGeometry *glyph = geo.getGlyph(*cp);
        if (glyph)
        {
            msdfgen::Bitmap<float, 3> msdf(40, 40);

            msdfgen::Shape shape = glyph->getShape();
            msdfgen::Projection  pro = glyph->getBoxProjection();
            msdfgen::Range range = glyph->getBoxRange();
            msdfgen::SDFTransformation t(pro, range);
            generateMSDF(msdf, shape, t);
            msdfgen::BitmapConstRef<float, 3> bitMap =  msdf;
            const float * datas = bitMap.pixels;
            //为当前字符建立纹理
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                bitMap.width,
                bitMap.height,
                0,
                GL_RED,
                GL_FLOAT,
                datas
            );
            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            // Now store character for later use

            textureMap.insert(num_glyph,texture);

            double pl, pb, pr, pt;
            double il, ib, ir, it;
            glyph->getQuadPlaneBounds(pl, pb, pr, pt);
            glyph->getQuadAtlasBounds(il, ib, ir, it);

            float x0  = xpos + pl ; // 24.0
            float y0  = ypos - pt ;
            float x1  = xpos + pr ;
            float y1  = ypos - pb ;;
//            GLfloat vertices[] =
//            {
//                x0, y1, 0,  s0, t1, // top right
//                x1, y1, 0,  s1, t1, // bottom right
//                x1, y0, 0,  s1, t0,
//                x0, y0, 0,  s0, t0 // top left
//            }; // bottom left
            // 当前字符的VBO
            GLfloat vertices[6][4] = {
                { x1,y0,   0.0, 0.0 },
                { x0,y0,   0.0, 1.0 },
                { x0,y1,   1.0, 1.0 },

                { x1,y0,   0.0, 0.0 },
                { x0,y1,   1.0, 1.0 },
                { x1,y1,   1.0, 0.0 }
            };
            // 在方块上绘制字形纹理
            glBindTexture(GL_TEXTURE_2D, texture);
            // 更新当前字符的VBO
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // 绘制方块
            glDrawArrays(GL_TRIANGLES, 0, 6);

            double advance = glyph->getAdvance();
            xpos+= advance;
            num_glyph++;

        }
    }
//    update();
}

void MyOpenglWidget::init(QList<VertexBuffer*> list)
{
//    makeCurrent();
    if(list.isEmpty())return;
    for (int i = 0; i < list.size() ;i++ )
    {
        VertexBuffer* buffer = list.at(i);
        QVector<int> indi = buffer->dindices;

        QVector<GLfloat> verts = buffer->dvertices;

        qDebug()<<"sizeof ( indi)/sizeof (int) "<<sizeof ( indi)/sizeof (int) <<endl;
        qDebug()<<"sizeof ( indi) "<<sizeof ( indi) <<endl;
        for (int j = 0;j <indi.size();j++ )
        {
            qDebug()<<indi[j]<<endl;
        }

//        for (int i = 0;i <sizeof ( verts)/sizeof (GLfloat) ;i++ )
//        {
//            qDebug()<<verts[i]<<endl;
//        }


        unsigned int VBO, VAO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, verts.size(), verts.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indi.size(), indi.data(), GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // c texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    //    // texture coord attribute
    //    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //    glEnableVertexAttribArray(2);


        // load and create a texture
        // -------------------------
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        vaoMap.insert(i,VAO);
        textureMap.insert(i,texture);
    }
    //    doneCurrent();
}

msdfgen::BitmapConstRef<byte, 3> MyOpenglWidget::createAndCatchAtlas(FontGeometry* geo, std::vector<GlyphGeometry> *glyphs,float width,float height)
{
    msdf_atlas::GeneratorAttributes attributes;
    attributes.config.overlapSupport = true;
    attributes.scanlinePass = true;

    // The ImmediateAtlasGenerator class facilitates the generation of the atlas bitmap.
    ImmediateAtlasGenerator<
        float, // pixel type of buffer for individual glyphs depends on generator function
        3, // number of atlas color channels
        &msdfGenerator, // function to generate bitmaps for individual glyphs
        BitmapAtlasStorage<byte, 3> // class that stores the atlas bitmap
        // For example, a custom atlas storage class that stores it in VRAM can be used.
    > generator(width, height);
    // GeneratorAttributes can be modified to change the generator's default settings.
//    GeneratorAttributes attributes;
    generator.setAttributes(attributes);
    generator.setThreadCount(8);
    // Generate atlas bitmap
    generator.generate(glyphs->data(),(int)glyphs->size());

    msdfgen::BitmapConstRef<byte, 3> bitMap =  (msdfgen::BitmapConstRef<byte, 3>)generator.atlasStorage();
    return bitMap;
}

void MyOpenglWidget::DrawString(const std::string& string,FontGeometry fontGeometry, const QMatrix4x4& transform)
{
//    const auto& fontGeometry = font->GetMSDFData()->FontGeometry;
    const auto& metrics = fontGeometry.getMetrics();
    QOpenGLTexture* fontAtlas = font->GetAtlasTexture();

//    s_Data.FontAtlasTexture = fontAtlas;

    double x = 0.0;
    double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
    double y = 0.0;

    const float spaceGlyphAdvance = fontGeometry.getGlyph(' ')->getAdvance();

    for (size_t i = 0; i < string.size(); i++)
    {
        char character = string[i];
        if (character == '\r')
            continue;

        if (character == '\n')
        {
            x = 0;
            y -= fsScale * metrics.lineHeight;
            continue;
        }

        if (character == ' ')
        {
            float advance = spaceGlyphAdvance;
            if (i < string.size() - 1)
            {
                char nextCharacter = string[i + 1];
                double dAdvance;
                fontGeometry.getAdvance(dAdvance, character, nextCharacter);
                advance = (float)dAdvance;
             }

            x += fsScale * advance;
            continue;
        }

        if (character == '\t')
        {
            // NOTE(Yan): is this right?
            x += 4.0f * (fsScale * spaceGlyphAdvance);
            continue;
        }

        auto glyph = fontGeometry.getGlyph(character);
        if (!glyph)
            glyph = fontGeometry.getGlyph('?');
        if (!glyph)
            return;

        double al, ab, ar, at;
        glyph->getQuadAtlasBounds(al, ab, ar, at);
        QVector2D texCoordMin((float)al, (float)ab);
        QVector2D texCoordMax((float)ar, (float)at);

        double pl, pb, pr, pt;
        glyph->getQuadPlaneBounds(pl, pb, pr, pt);
        QVector2D quadMin((float)pl, (float)pb);
        QVector2D quadMax((float)pr, (float)pt);

        quadMin *= fsScale, quadMax *= fsScale;
        quadMin += QVector2D(x, y);
        quadMax += QVector2D(x, y);

        float texelWidth = 1.0f / fontAtlas->width();
        float texelHeight = 1.0f / fontAtlas->height();
        texCoordMin *= QVector2D(texelWidth, texelHeight);
        texCoordMax *= QVector2D(texelWidth, texelHeight);

        // render here
//        fontAtlas->

//        s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMin, 0.0f, 1.0f);
//        s_Data.TextVertexBufferPtr->Color = textParams.Color;
//        s_Data.TextVertexBufferPtr->TexCoord = texCoordMin;
//        s_Data.TextVertexBufferPtr->EntityID = entityID;
//        s_Data.TextVertexBufferPtr++;

//        s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMin.x, quadMax.y, 0.0f, 1.0f);
//        s_Data.TextVertexBufferPtr->Color = textParams.Color;
//        s_Data.TextVertexBufferPtr->TexCoord = { texCoordMin.x, texCoordMax.y };
//        s_Data.TextVertexBufferPtr->EntityID = entityID;
//        s_Data.TextVertexBufferPtr++;

//        s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMax, 0.0f, 1.0f);
//        s_Data.TextVertexBufferPtr->Color = textParams.Color;
//        s_Data.TextVertexBufferPtr->TexCoord = texCoordMax;
//        s_Data.TextVertexBufferPtr->EntityID = entityID;
//        s_Data.TextVertexBufferPtr++;

//        s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMax.x, quadMin.y, 0.0f, 1.0f);
//        s_Data.TextVertexBufferPtr->Color = textParams.Color;
//        s_Data.TextVertexBufferPtr->TexCoord = { texCoordMax.x, texCoordMin.y };
//        s_Data.TextVertexBufferPtr->EntityID = entityID;
//        s_Data.TextVertexBufferPtr++;

//        s_Data.TextIndexCount += 6;
//        s_Data.Stats.QuadCount++;

        if (i < string.size() - 1)
        {
            double advance = glyph->getAdvance();
            char nextCharacter = string[i + 1];
            fontGeometry.getAdvance(advance, character, nextCharacter);

            x += fsScale * advance;
        }
    }
}
