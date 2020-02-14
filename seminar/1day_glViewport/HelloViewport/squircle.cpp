#include "squircle.h"

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLShaderProgram>
#include <QtQuick/qquickwindow.h>

#include <QDebug>

Squircle::Squircle()
    : m_t(0)
    , m_renderer(nullptr)
{
    qDebug() << "construction";
    connect(this, &QQuickItem::windowChanged, this, &Squircle::handleWindowChanged);
}

void Squircle::setT(qreal t)
{
    if (t == m_t) {
        return;
    } else {
    }
    m_t = t;
    emit tChanged();
    if (window()) {
        window()->update();
    } else {
    }
}

void Squircle::handleWindowChanged(QQuickWindow* win)
{
    qDebug() << "handleWindowChanged";

    if (win) {
        qDebug() << " handleWindowChanged window is valid";
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

void Squircle::sync()
{
    qDebug() << "emit beforeSynchronizing : sync";
    if (!m_renderer) {
        m_renderer = new SquircleRenderer();
        connect(window(), &QQuickWindow::beforeRendering, m_renderer, &SquircleRenderer::paint, Qt::DirectConnection);
    }
    m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
    m_renderer->setT(m_t);
    m_renderer->setWindow(window());
}

//void SquircleRenderer::paint()
//{

//    // 일반 화면
//    //    glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());
//    glClearColor(m_t, 0, 0, 1);
//    glClear(GL_COLOR_BUFFER_BIT);

//    //    // 2분할  화면
//    //    glEnable(GL_SCISSOR_TEST);
//    //    glViewport(0, 0, m_viewportSize.width() / 2, m_viewportSize.height());
//    //    glScissor(0, 0, m_viewportSize.width() / 2, m_viewportSize.height());
//    //    glClearColor(m_t, 0, 0, 1);
//    //    glClear(GL_COLOR_BUFFER_BIT);

//    //    glViewport(m_viewportSize.width() / 2, 0, m_viewportSize.width() / 2, m_viewportSize.height());
//    //    glScissor(m_viewportSize.width() / 2, 0, m_viewportSize.width() / 2, m_viewportSize.height());
//    //    glClearColor(0, m_t, 0, 1);
//    //    glClear(GL_COLOR_BUFFER_BIT);

//    m_window->resetOpenGLState();
//}

void SquircleRenderer::paint()
{

    // 2분할  화면
    // NOTE : glViewport를 하나만 해도(glViewport 제거해도) 되는것 같은데 무슨 차이인가 ???
    // NOTE : 뷰포트를 설정하지 않아도 나옴

    glEnable(GL_SCISSOR_TEST);
    //    glViewport(0, 0, m_viewportSize.width() / 2, m_viewportSize.height());
    glScissor(0, 0, m_viewportSize.width() / 2, m_viewportSize.height());
    glClearColor(m_t, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    //    glViewport(m_viewportSize.width() / 2, 0, m_viewportSize.width() / 2, m_viewportSize.height());
    glScissor(m_viewportSize.width() / 2, 0, m_viewportSize.width() / 2, m_viewportSize.height());
    glClearColor(0, m_t, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    m_window->resetOpenGLState();
}
