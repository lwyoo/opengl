#include "squircle.h"
#include <QtQuick/qquickwindow.h>

Squircle::Squircle()
    : m_t(0)
    , m_renderer(nullptr)
{
    setFlag(ItemHasContents);
    connect(this, &QQuickItem::windowChanged, this, &Squircle::handleWindowChanged);
}

qreal Squircle::t() const
{
    return m_t;
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

QString Squircle::texture() const
{
    return m_texture;
}

void Squircle::setTexture(const QString& texture)
{
    m_texture = texture;
}

QSize Squircle::getViewportSize() const
{
    return m_viewportSize;
}

QQuickFramebufferObject::Renderer* Squircle::createRenderer() const
{
    SquircleRenderer* pRender = new SquircleRenderer(this);
    connect(window(), &QQuickWindow::beforeRendering, pRender, &SquircleRenderer::sync, Qt::DirectConnection);
    return dynamic_cast<QQuickFramebufferObject::Renderer*>(pRender);
}

void Squircle::componentComplete()
{
    QQuickFramebufferObject::componentComplete();
}

void Squircle::cleanup()
{
    if (m_renderer) {
        delete m_renderer;
        m_renderer = nullptr;
    }
}

void Squircle::handleWindowChanged(QQuickWindow* win)
{
    if (win) {
        connect(win, &QQuickWindow::sceneGraphInvalidated, this, &Squircle::cleanup, Qt::DirectConnection);
        win->setClearBeforeRendering(false);
    }
}
