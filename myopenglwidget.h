#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLWidget>
#include <qmatrix4x4.h>
#include <QOpenGLShaderProgram>
#include <msdfgen/msdfgen.h>
#include <msdfgen/msdfgen-ext.h>
#include "msdfgen/ext/import-font.h"
#include "msdfgen/ext/save-png.h"
#include "msdfgen/core/export-svg.h"
#include <msdf-atlas-gen/msdf-atlas-gen.h>
#include "msdf-atlas-gen/TightAtlasPacker.h"
#include "myfonts.h"
#include "base.h"

using namespace msdf_atlas;

using namespace msdfgen;

struct VertexBuffer
{
    QVector<int> dindices;
    QVector<GLfloat> dvertices;
    GLuint VAO;
};

class MyOpenglWidget : public QOpenGLWidget,public QOpenGLFunctions_4_5_Core
{
public:
    MyOpenglWidget();
    ~MyOpenglWidget();
   void renderAText(const char *fontFilename);
//    Bitmap<float, 3> renderATextf();
    msdfgen::BitmapConstRef<float, 3>* generateAtlas(const char *fontFilename);

    bool submitAtlasBitmapAndLayout(BitmapAtlasStorage<byte, 3>, std::vector<GlyphGeometry>);

    QList<VertexBuffer*> add_text_msdf2(FontGeometry geo, QList<VertexBuffer*> list, const char* text, float xpos, float ypos);

    void RenderText(FontGeometry geo,const char *text, float xpos, float ypos);
    void init(QList<VertexBuffer*>);

    msdfgen::BitmapConstRef<byte, 3> createAndCatchAtlas(FontGeometry*,std::vector<GlyphGeometry>*,float,float);
    Engine::TextureSpecification* generateAtlas1(const char *fontFilename);
    void DrawString(const std::string& string,FontGeometry fontGeometry, const QMatrix4x4& transform);
protected:
    void initializeGL() override;
    void resizeGL(int w, int h)override;
    void paintGL()override;

private:
    QMatrix4x4 proj,model,view,m_worldToView;
    float fov = 45.0f;
    float nearP = 0.1f;
    float farP = 100.0f;
    QOpenGLShaderProgram program;

    QList<VertexBuffer*> list;
    QMap<int,unsigned int> vaoMap;
    QMap<int,unsigned int> textureMap;

    GLuint VAO,VBO,EBO,texture;
    MyFonts *font;

    GLuint texture1;
    QOpenGLTexture * texture2;
    TextureSpecification spec;

};

#endif // MYOPENGLWIDGET_H
