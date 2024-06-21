#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <msdfgen/msdfgen.h>
#include <msdfgen/msdfgen-ext.h>
#include "msdfgen/ext/import-font.h"
#include "msdfgen/ext/save-png.h"
#include "msdfgen/core/export-svg.h"
#include "myopenglwidget.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    renderAText();
    MyOpenglWidget *widget = new MyOpenglWidget;
    this->setCentralWidget(widget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

using namespace msdfgen;

int MainWindow::renderAText()
{
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
                Bitmap<float, 3> msdf1(32, 32);
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
    return 0;
}
