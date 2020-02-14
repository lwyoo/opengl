#include "squirclerenderer.h"
#include <QQuickWindow>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLShaderProgram>
SquircleRenderer::SquircleRenderer()
    : m_t(0)
    , m_program(nullptr)
{
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
    //    impl......
}
