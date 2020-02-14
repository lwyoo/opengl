#include "squircle.h"

#include <QSGTextureProvider>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLShaderProgram>
#include <QtQuick/qquickwindow.h>

Squircle::Squircle()
    : m_t(0)
    , m_renderer(nullptr)
{
    setFlag(ItemHasContents);
    connect(this, &QQuickItem::windowChanged, this, &Squircle::handleWindowChanged);
}

void Squircle::setT(qreal t)
{
    if (t == m_t)
        return;
    m_t = t;
    //update();
    emit tChanged();
    if (window())
        window()->update();
}

void Squircle::handleWindowChanged(QQuickWindow* win)
{
    if (win) {
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

void Squircle::componentComplete()
{
    QQuickFramebufferObject::componentComplete();
}

SquircleRenderer::SquircleRenderer(const Squircle* item)
    : m_t(0)
    , m_item(item)
    , m_program(nullptr)
{
    QString strFile = item->texture();
    QImage image(strFile);

    m_texture = item->window()->createTextureFromImage(image);
}

SquircleRenderer::~SquircleRenderer()
{
    delete m_program;
    delete m_texture;
}

void SquircleRenderer::sync()
{
    this->m_viewportSize = QSize(m_item->width(), m_item->height());
}

QQuickFramebufferObject::Renderer* Squircle::createRenderer() const
{
    SquircleRenderer* pRender = new SquircleRenderer(this);
    connect(window(), &QQuickWindow::beforeRendering, pRender, &SquircleRenderer::sync, Qt::DirectConnection);
    return dynamic_cast<QQuickFramebufferObject::Renderer*>(pRender);
}

void SquircleRenderer::render()
{
    m_t = 1.0;
    QOpenGLFunctions* oglFunctions = QOpenGLContext::currentContext()->functions();
    if (m_program == nullptr) {
        m_program = new QOpenGLShaderProgram();
        m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,
            "attribute highp vec4 vertices;"
            "attribute lowp vec2 a_Tex;\n"
            "uniform highp mat4 matrix;"
            "varying vec2 vTexCoord;\n"
            "void main() {"
            "vTexCoord = a_Tex*2;"
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

        100,
        0,

        0,
        100,

        100,
        100,
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

    oglFunctions->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    m_program->disableAttributeArray(0);
    m_program->disableAttributeArray(1);
    m_program->release();
}

/*void SquircleRenderer::render(const RenderState* stateRender)
{
    m_t = 1.0;
    QOpenGLFunctions* oglFunctions = QOpenGLContext::currentContext()->functions();
    oglFunctions->glEnable(GL_SCISSOR_TEST);
    oglFunctions->glViewport(0, 0, m_viewportSize.width() / 2, m_viewportSize.height());
    oglFunctions->glScissor(0, 0, m_viewportSize.width() / 2, m_viewportSize.height());

    oglFunctions->glClearColor(m_t, 0, 0, 1);
    oglFunctions->glClear(GL_COLOR_BUFFER_BIT);

    oglFunctions->glViewport( m_viewportSize.width() / 2, 0, m_viewportSize.width() / 2, m_viewportSize.height());
    oglFunctions->glScissor( m_viewportSize.width() / 2, 0, m_viewportSize.width() / 2, m_viewportSize.height());

    oglFunctions->glClearColor(0, m_t, 0, 1);
    oglFunctions->glClear(GL_COLOR_BUFFER_BIT);
}


QSGNode* Squircle::updatePaintNode(QSGNode* nodeSG, UpdatePaintNodeData*)
{
    if (nodeSG == nullptr) {
        if (!m_renderer) {
            m_renderer = new SquircleRenderer(this);
            connect(window(), &QQuickWindow::beforeRendering, m_renderer, &SquircleRenderer::sync, Qt::DirectConnection);
        }
        nodeSG = dynamic_cast<QSGNode*>(m_renderer);
    }
    return nodeSG;
}
*/
