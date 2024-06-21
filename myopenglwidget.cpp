#include "myopenglwidget.h"
#include <ft2build.h>
#include FT_FREETYPE_H

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

}

void MyOpenglWidget::initializeGL()
{
    initializeOpenGLFunctions();

    //文字VAO、VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    program.addShaderFromSourceFile(QOpenGLShader::Vertex,":/textVert.vsh");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment,":/textFshader.fsh");
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
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//     program.setUniformValue("worldToView",m_worldToView);
//   if(!list.isEmpty())
//   {
//       for (int i = 0; i < list.size()  ; i++ )
//       {
//           if(!textureMap.isEmpty() && !vaoMap.isEmpty() && textureMap.size() == vaoMap.size())
//           {
//               program.bind();
//               // bind Texture
//               glBindTexture(GL_TEXTURE_2D,textureMap.value(i) );
//               glBindVertexArray(vaoMap.value(i));
//               glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//           }
//       }
//   }

   QString dirStr = "../OpenglTextRender/fonts/arial.ttf";
   QByteArray ba = dirStr.toLatin1();
   char* font_file = ba.data();
   generateAtlas(font_file);


}

using namespace msdfgen;
bool MyOpenglWidget::generateAtlas(const char *fontFilename)
{
//    makeCurrent();
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
            for (GlyphGeometry &glyph : glyphs)
//                glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);
            glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);
            // TightAtlasPacker class computes the layout of the atlas.
            TightAtlasPacker packer;
            // Set atlas parameters:
            // setDimensions or setDimensionsConstraint to find the best value
            packer.setDimensionsConstraint(msdf_atlas::DimensionsConstraint::SQUARE);
            // setScale for a fixed size or setMinimumScale to use the largest that fits
            packer.setMinimumScale(48.0);
            // setPixelRange or setUnitRange
            packer.setPixelRange(1);
            packer.setMiterLimit(1.0);
            // Compute atlas layout - pack glyphs
            packer.pack(glyphs.data(), glyphs.size());
            // Get final atlas dimensions
            int width = 0, height = 0;
            packer.getDimensions(width, height);
            // The ImmediateAtlasGenerator class facilitates the generation of the atlas bitmap.
            ImmediateAtlasGenerator<
                float, // pixel type of buffer for individual glyphs depends on generator function
                3, // number of atlas color channels
                &msdfGenerator, // function to generate bitmaps for individual glyphs
                BitmapAtlasStorage<byte, 3> // class that stores the atlas bitmap
                // For example, a custom atlas storage class that stores it in VRAM can be used.
            > generator(width, height);
            // GeneratorAttributes can be modified to change the generator's default settings.
            GeneratorAttributes attributes;
            generator.setAttributes(attributes);
            generator.setThreadCount(3);
            // Generate atlas bitmap
            generator.generate(glyphs.data(), glyphs.size());
            // The atlas bitmap can now be retrieved via atlasStorage as a BitmapConstRef.
            // The glyphs array (or fontGeometry) contains positioning data for typesetting text.
//            success = submitAtlasBitmapAndLayout(generator.atlasStorage(), glyphs);
            // Cleanup


//            QList<VertexBuffer*> mlist = add_text_msdf2(fontGeometry,list,"helloWorld!",0,0);
//             qDebug()<<mlist.size()<<endl;
//            init(mlist);
//            for (int i = 0; i <mlist.size() ;i++ ) {
//                list.append(mlist.at(i));
//            }

            BitmapAtlasStorage<byte, 3> storage = generator.atlasStorage();
            RenderText(fontGeometry,program,"helloWorld!",0.0,0.0);


            msdfgen::destroyFont(font);
        }
        msdfgen::deinitializeFreetype(ft);
    }
//    doneCurrent();
    return success;
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

//                GLfloat vertices[] =
//                {
//                    x0, y1, 0,  s0, t1, // top right
//                    x1, y1, 0,  s1, t1, // bottom right
//                    x1, y0, 0,  s1, t0,
//                    x0, y0, 0,  s0, t0 // top left
//                }; // bottom left

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

                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    msdf1.width(),
                    msdf1.height(),
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    msdf1.operator float *()
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

void MyOpenglWidget::RenderText(FontGeometry geo,QOpenGLShaderProgram &shader, const char *text, float xpos, float ypos)
{
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

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
            Bitmap<float, 3> msdf1(100, 100);
            msdfgen::Shape shape = glyph->getShape();
            msdfgen::Projection  pro = glyph->getBoxProjection();
            msdfgen::Range range = glyph->getBoxRange();
            SDFTransformation t(pro, range);
            generateMSDF(msdf1, shape, t);
//            QString data = "outputgg"+QString::number(num_glyph)+".bmp";
//            QByteArray ba = data.toLatin1();
//            char* file = ba.data();
//            saveBmp(msdf1,file);

            //为当前字符建立纹理
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                msdf1.width(),
                msdf1.height(),
                0,
                GL_RED,
                GL_FLOAT,
                msdf1.operator float *()
            );
            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

bool MyOpenglWidget::submitAtlasBitmapAndLayout(BitmapAtlasStorage<byte, 3> storage, std::vector<GlyphGeometry> vec)
{

    int width = 20, height = 20;
    BitmapRef<byte, 3> bitMapRef = storage.operator BitmapRef<byte, 3>();
    std::vector<uint32_t> data(width,height);
    for (int i = 0;i < width * height ; i++ )
    {
        data[i] = (uint32_t)bitMapRef.pixels[4*i]<<24
        |(uint32_t)bitMapRef.pixels[4*i + 1]<<16
        |(uint32_t)bitMapRef.pixels[4*i + 2]<<8
        |(uint32_t)bitMapRef.pixels[4*i + 3];
    }

//    size_t len = vec.size();
//    for (int i = 0; i < len; i ++)
//    {
        Bitmap<float, 3> msdf1(100, 100);

////        Bitmap<float, 4> msdf(100, 100);
//        GlyphGeometry geo = vec[i];
//        msdfgen::Shape shape = geo.getShape();
//        msdfgen::Projection  pro = geo.getBoxProjection();
//        msdfgen::Range range = geo.getBoxRange();
//        SDFTransformation t(pro, range);

//        MSDFGeneratorConfig config;
//        generateMSDF(msdf1, shape, t,config);
////                        generateMTSDF(bitMapRef, shape, t,config) ;

//        QString data = "outputgg"+QString::number(i)+".bmp";
//        QByteArray ba = data.toLatin1();
//        char* file = ba.data();
//        saveBmp(msdf1,file);

//    }
    return true;
}

Bitmap<float, 3> MyOpenglWidget::renderAText()
{
    Bitmap<float, 3> msdf1(32, 32);
    if (FreetypeHandle *ft = initializeFreetype()) {
        QString dirStr = "../OpenglTextRender/fonts/arial.ttf";

        QByteArray ba = dirStr.toLatin1();
        char* font_file = ba.data();

        if (FontHandle *font = loadFont(ft, font_file)) {
            Shape shape;
            if (loadGlyph(shape, font, 'D', FONT_SCALING_EM_NORMALIZED)) {
                shape.normalize();
                //                      max. angle
                edgeColoringSimple(shape, 3.0);
                //          output width, height
//                Bitmap<float, 3> msdf1(32, 32);
                //                            scale, translation (in em's)
                SDFTransformation t(Projection(32.0, Vector2(0.125, 0.125)), Range(0.125));
                generateMSDF(msdf1, shape, t);
//                MSDFGeneratorConfig config;
//                generateMTSDF(msdf, shape, t,config) ;
//                generateSDF(msdf, shape, t,config);
//                savePng(msdf, "output.png");
                saveBmp(msdf1,"output.bmp");
                saveTiff(msdf1,"output.tiff");
            }
            destroyFont(font);
        }
        deinitializeFreetype(ft);
    }
    return msdf1;
}

//Bitmap<float, 3> MyOpenglWidget::renderATextf()
//{

//}
