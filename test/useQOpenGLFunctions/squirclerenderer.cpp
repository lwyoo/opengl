#include "squirclerenderer.h"
#include <QDebug>
#include <QQuickWindow>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLShaderProgram>
#define TEST1 0 // viewport setting
#define TEST2 0 // two viewport & SCISSOR setting
#define TEST3 1 // use shader
SquircleRenderer::SquircleRenderer()
    : m_t(0)
    , m_program(nullptr)
{
    initializeOpenGLFunctions();
}

void SquircleRenderer::setT(qreal t)
{
    m_t = t;
}

void SquircleRenderer::setViewportSize(const QSize& size)
{
    m_viewportSize = size;
}

void SquircleRenderer::setWindow(QQuickWindow* window)
{
    m_window = window;
}

void SquircleRenderer::paint()
{
#if TEST1
    //뷰포트 설정 하여 그리는 영역 잡아주고
    glViewport(0, 0, 10, 100);
    //clear시 변경할 색상 설정 하고
    glClearColor(0, 1, 0, 1);
    //클리어 요청
    glClear(GL_COLOR_BUFFER_BIT);
    m_window->resetOpenGLState();
#endif

#if TEST2
    glEnable(GL_SCISSOR_TEST);
    glViewport(0, 0, 100, 100);
    glScissor(0, 0, 100, 100);
    glClearColor(0, 1, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_SCISSOR_TEST);
    glViewport(200, 0, 100, 100);
    glScissor(200, 0, 100, 100);
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    m_window->resetOpenGLState();
#endif
#if TEST3

    qDebug() << Q_FUNC_INFO << "m_t : " << m_t * 255;
    if (!m_program) {
        m_program = new QOpenGLShaderProgram();

        //add vertex shader!!!!!!!!!!!!!!!!!!!!!!11
        m_program->addShaderFromSourceCode(QOpenGLShader::Vertex,
            "attribute highp vec4 vertex;\n"
            "uniform highp mat4 matrix;\n"
            "void main(void)\n"
            "{\n"
            "   gl_Position = matrix * vertex;\n"
            "}"

        );
        m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,
            "uniform mediump vec4 color;\n"
            "void main(void)\n"
            "{\n"
            "   gl_FragColor = color;\n"
            "}");
    }
    m_program->link();
    m_program->bind();

    int vertexLocation = m_program->attributeLocation("vertex");
    int matrixLocation = m_program->uniformLocation("matrix");
    int colorLocation = m_program->uniformLocation("color");

    //    static GLfloat const triangleVertices[] = {
    //        60.0f,        10.0f,        0.0f,
    //        110.0f,        110.0f,        0.0f,
    //        10.0f,        110.0f,        0.0f,

    //        160.0f,        10.0f,        0.0f,
    //        210.0f,        110.0f,        0.0f,
    //        110.0f,        110.0f,        0.0f,

    //    };
    //    static GLfloat const triangleVertices[] = {
    //        60.0f,        10.0f,        0.0f,
    //        110.0f,        110.0f,        0.0f,
    //        10.0f,        110.0f,        0.0f,

    //    };
    static GLfloat const triangleVertices[] = {
        0.0f,
        0.0f,
        0.0f,
        51.0f,
        0.0f,
        0.0f,
        25.0f,
        51.0f,
        0.0f,

    };

    QColor color(0, m_t * 255, 0, 255);
    qDebug() << "red : " << color.green();

    // projection setting!!!!
    QMatrix4x4 pmvMatrix;
    pmvMatrix.ortho(0.0f, static_cast<float>(m_viewportSize.width()), 0.0f, static_cast<float>(m_viewportSize.height()), -100.0f, 100.0f);

    //vertext position setting!!!!
    m_program->enableAttributeArray(vertexLocation);
    m_program->setAttributeArray(vertexLocation, triangleVertices, 3);

    m_program->setUniformValue(matrixLocation, pmvMatrix);
    m_program->setUniformValue(colorLocation, color);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    //    glDrawArrays(GL_TRIANGLES, 3, 6);

    m_program->disableAttributeArray(vertexLocation);
    m_window->resetOpenGLState();

#endif
}
