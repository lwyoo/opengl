#include "squircle.h"

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLShaderProgram>
#include <QtQuick/qquickwindow.h>

Squircle::Squircle()
    : m_t(0)
    , m_renderer(nullptr)
{
    connect(this, &QQuickItem::windowChanged, this, &Squircle::handleWindowChanged);
}

void Squircle::setT(qreal t)
{
    if (t == m_t)
        return;
    m_t = t;
    emit tChanged();
    if (window())
        window()->update();
}

void Squircle::handleWindowChanged(QQuickWindow* win)
{
    if (win) {
        connect(win, &QQuickWindow::beforeSynchronizing, this, &Squircle::sync, Qt::DirectConnection);
        connect(win, &QQuickWindow::sceneGraphInvalidated, this, &Squircle::cleanup, Qt::DirectConnection);
        win->setClearBeforeRendering(false);
    }
}

void Squircle::cleanup()
{
    if (m_renderer) {
        delete m_renderer;
        m_renderer = nullptr;
    }
}

SquircleRenderer::~SquircleRenderer()
{
    delete m_program;
}

void Squircle::sync()
{
    if (!m_renderer) {
        m_renderer = new SquircleRenderer();
        connect(window(), &QQuickWindow::beforeRendering, m_renderer, &SquircleRenderer::paint, Qt::DirectConnection);
    }
    m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
    m_renderer->setT(m_t);
    m_renderer->setWindow(window());
}

void SquircleRenderer::paint()
{
    if (!m_program) {
        initializeOpenGLFunctions();
        m_program = new QOpenGLShaderProgram();
        m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,
            "attribute highp vec4 vertices;"
            "uniform highp mat4 matrix;"
            "void main() {"
            "   gl_Position = matrix * vertices;"
            "}");
        m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,
            "void main() {"
            "    gl_FragColor = vec4(0, 1, 0, 1);"
            "}");

        m_program->bindAttributeLocation("vertices", 0);
        m_program->link();
    }
    m_program->bind();
    m_program->enableAttributeArray(0);
    int m_matrixUniform = m_program->uniformLocation("matrix");
    QMatrix4x4 modelview;
    QMatrix4x4 proj;

    modelview.setToIdentity();
    modelview.rotate(m_t * 360, QVector3D(0, 0, 1));
    proj.setToIdentity();
    proj.ortho(0.0f, static_cast<float>(m_viewportSize.width()), 0.0f, static_cast<float>(m_viewportSize.height()), -100.0f, 100.0f);
    QMatrix4x4 trv = proj * modelview;
    m_program->setUniformValue(m_matrixUniform, trv);
    float values[] = {
        10, 10,
        static_cast<float>(m_viewportSize.width()) - 10, 10,
        10, static_cast<float>(m_viewportSize.height()) - 10,
        static_cast<float>(m_viewportSize.width()) - 10, static_cast<float>(m_viewportSize.height()) - 10
    };

    //    unsigned int VBO;
    //    glGenBuffers(1, &VBO);
    //    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    m_program->setAttributeArray(0, GL_FLOAT, values, 2);
    glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    m_program->disableAttributeArray(0);
    m_program->release();

    // Not strictly needed for this example, but generally useful for when
    // mixing with raw OpenGL.
    m_window->resetOpenGLState();
}
