#include "squirclerenderer.h"
#include "squircle.h"
#include <QSGTextureProvider>
//#include <QtGui/QOpenGLContext>
//#include <QtGui/QOpenGLShaderProgram>
#include <QDebug>
#include <QtGui/QOpenGLFunctions>
#include <QtQuick/qquickwindow.h>

SquircleRenderer::SquircleRenderer(const Squircle* item)
    : m_t(0)
    , m_item(item)
    , m_program(nullptr)
{
    QString strFile = item->texture();
    // QSGTextureProvider
    QImage image(strFile);

    //qquickwindow
    m_texture = item->window()->createTextureFromImage(image);
    GLint tempM;
    GLint tempn;
    glGetIntegerv(GL_MAJOR_VERSION, &tempM);
    glGetIntegerv(GL_MINOR_VERSION, &tempn);

    qDebug() << "GL_MAJOR_VERSION : " << tempM << "GL_MINOR_VERSION : " << tempn;
}

SquircleRenderer::~SquircleRenderer()
{
    delete m_program;
    delete m_texture;
}

QOpenGLFramebufferObject* SquircleRenderer::createFramebufferObject(const QSize& size)
{
    QOpenGLFramebufferObjectFormat format;
    format.setSamples(1);
    return new QOpenGLFramebufferObject(size, format);
}

void SquircleRenderer::render()
{
    //    qDebug() << "m_t : " << m_t;
    QOpenGLFunctions* oglFunctions = QOpenGLContext::currentContext()->functions();
    if (m_program == nullptr) {
        m_program = new QOpenGLShaderProgram();
        m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,
            "attribute highp vec4 vertices;"
            "attribute lowp vec2 a_Tex;\n"
            "uniform highp mat4 matrix;"
            "varying vec2 vTexCoord;\n"
            "void main() {"
            "vTexCoord = a_Tex;"
            "        gl_Position = matrix * vertices;"
            //   "   gl_Position = vertices;"
            "}");
        m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,
            "varying vec2 vTexCoord;\n"
            "uniform sampler2D texRGB;\n"
            "void main() {"
            "    gl_FragColor = texture2D(texRGB, vTexCoord);"
            "}");

        m_program->bindAttributeLocation("vertices", 0);
        m_program->bindAttributeLocation("a_Tex", 1);

        m_program->link();
    }

    m_program->bind();
    int m_matrixUniform = m_program->uniformLocation("matrix");
    QMatrix4x4 modelview;
    QMatrix4x4 proj;

    modelview.setToIdentity();
    modelview.rotate(m_t * 360, QVector3D(1.0f, 0.0f, 0.0f));

    proj.setToIdentity();
    proj.ortho(0.0f, static_cast<float>(m_viewportSize.width()), 0.0f, static_cast<float>(m_viewportSize.height()), -100.0f, 100.0f);
    QMatrix4x4 trv = proj * modelview;
    m_program->setUniformValue(m_matrixUniform, trv);
    m_program->enableAttributeArray(0);
    m_program->enableAttributeArray(1);
    oglFunctions->glEnable(GL_BLEND);
    oglFunctions->glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    float values[] = {
        0,
        0,

        512,
        0,

        0,
        512,

        512,
        512,
    };

    float texcoord[] = {
        0, 0,

        1, 0,

        0, 1,

        1, 1
    };

    m_program->setAttributeArray(0, GL_FLOAT, values, 2);
    m_program->setAttributeArray(1, GL_FLOAT, texcoord, 2);
    oglFunctions->glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());
    oglFunctions->glClearColor(1, 0, 0, 1);
    oglFunctions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    const int texUnformID = oglFunctions->glGetUniformLocation(m_program->programId(), "texRGB");

    oglFunctions->glUniform1i(texUnformID, 0);
    if (m_texture)
        m_texture->bind(); //opengl bind? call
    //범위 밖의 좌표를 지정하면
    //GL_REPEAT: 텍스처의 기본 동작입니다. 이미지를 반복합니다.
    //GL_MIRRORED_REPEAT: GL_REPEAT와 같지만 반복할때마다 이미지를 반대로 뒤집습니다.
    //GL_CLAMP_TO_EDGE: 0과 1 사이의 좌표를 고정합니다. 결과적으로 큰 좌표가 가장자리에 고정되어 가장자리의 패턴이 늘어나게 됩니다.
    //GL_CLAMP_TO_BORDER: 범위 밖의 좌표에 사용자가 지정한 테두리 색이 지정됩니다.
    //    oglFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //    oglFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    //    oglFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //    oglFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    //    oglFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    //    oglFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    oglFunctions->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    m_program->disableAttributeArray(0);
    m_program->disableAttributeArray(1);
    m_program->release();
}
//void SquircleRenderer::render()
//{
//    m_t = 1.0;
//    QOpenGLFunctions* oglFunctions = QOpenGLContext::currentContext()->functions();
//    if (m_program == nullptr) {
//        m_program = new QOpenGLShaderProgram();
//        m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,
//            "attribute highp vec4 vertices;"
//            "attribute lowp vec2 a_Tex;\n"
//            "uniform highp mat4 matrix;"
//            "varying vec2 vTexCoord;\n"
//            "void main() {"
//            "vTexCoord = a_Tex;"
//            "        gl_Position = matrix * vertices;"
//            //   "   gl_Position = vertices;"
//            "}");
//        m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,
//            "varying vec2 vTexCoord;\n"
//            "uniform sampler2D texRGB;\n"
//            "void main() {"
//            "    gl_FragColor = texture2D(texRGB, vTexCoord);"
//            "}");

//        m_program->bindAttributeLocation("vertices", 0);
//        m_program->bindAttributeLocation("a_Tex", 1);

//        m_program->link();
//    }

//    m_program->bind();
//    int m_matrixUniform = m_program->uniformLocation("matrix");
//    QMatrix4x4 modelview;
//    QMatrix4x4 proj;

//    modelview.setToIdentity();
//    proj.setToIdentity();
//    proj.ortho(0.0f, static_cast<float>(m_viewportSize.width()), 0.0f, static_cast<float>(m_viewportSize.height()), -100.0f, 100.0f);
//    QMatrix4x4 trv = proj * modelview;
//    m_program->setUniformValue(m_matrixUniform, trv);
//    m_program->enableAttributeArray(0);
//    m_program->enableAttributeArray(1);
//    oglFunctions->glEnable(GL_BLEND);
//    oglFunctions->glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

//    //    float values[] = {
//    //        0,
//    //        0,

//    //        512,
//    //        0,

//    //        0,
//    //        512,

//    //        512,
//    //        512,
//    //    };

//    //    float texcoord[] = {
//    //        0, 0,

//    //        1, 0,

//    //        0, 1,

//    //        1, 1
//    //    };
//    //    float values[] = {
//    //        0,
//    //        0,

//    //        512,
//    //        0,

//    //        512 / 2,
//    //        512,

//    //    };
//    //    float texcoord[] = {
//    //        0,
//    //        0,

//    //        1,
//    //        0,

//    //        0.5,
//    //        1,

//    //    };
//    float values[] = {

//        0,
//        512,

//        512,
//        512,
//        512 / 2,
//        0,

//    };

//    float texcoord[] = {

//        0,
//        1,

//        1,
//        1,

//        0.5,
//        0,
//    };

//    m_program->setAttributeArray(0, GL_FLOAT, values, 2); // vertex shader : vertices
//    m_program->setAttributeArray(1, GL_FLOAT, texcoord, 2); // vertex shader : a_Tex
//    //    oglFunctions->glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());
//    oglFunctions->glClearColor(1, 0, 0, 1);
//    oglFunctions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    const int texUnformID = oglFunctions->glGetUniformLocation(m_program->programId(), "texRGB");

//    oglFunctions->glUniform1i(texUnformID, 0);
//    if (m_texture)
//        m_texture->bind(); //opengl bind? call

//    //    oglFunctions->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//    oglFunctions->glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

//    m_program->disableAttributeArray(0);
//    m_program->disableAttributeArray(1);
//    m_program->release();
//}

void SquircleRenderer::setT(qreal t)
{
    m_t = t;
    //    update();
}

void SquircleRenderer::setViewportSize(const QSize& size)
{
    m_viewportSize = size;
}

void SquircleRenderer::setWindow(QQuickWindow* window)
{
    m_window = window;
}

void SquircleRenderer::sync()
{
    //    qDebug() << Q_FUNC_INFO << m_item->t();
    this->setT(m_item->t());
    this->m_viewportSize = QSize(m_item->width(), m_item->height());
}
