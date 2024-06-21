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
    Bitmap<float, 3> renderAText();
//    Bitmap<float, 3> renderATextf();
    bool generateAtlas(const char *fontFilename);

    bool submitAtlasBitmapAndLayout(BitmapAtlasStorage<byte, 3>, std::vector<GlyphGeometry>);

    QList<VertexBuffer*> add_text_msdf2(FontGeometry geo, QList<VertexBuffer*> list, const char* text, float xpos, float ypos);

    void RenderText(FontGeometry geo,QOpenGLShaderProgram &s, const char* text, float xpos, float ypos);
    void init(QList<VertexBuffer*>);
protected:
    void initializeGL() override;
    void resizeGL(int w, int h)override;
    void paintGL()override;

private:
    QMatrix4x4 proj;
    float fov = 45.0f;
    float nearP = 0.1f;
    float farP = 100.0f;
    QOpenGLShaderProgram program;

    QList<VertexBuffer*> list;
    QMap<int,unsigned int> vaoMap;
    QMap<int,unsigned int> textureMap;

    GLuint VAO,VBO,EBO,texture;
};

#endif // MYOPENGLWIDGET_H
