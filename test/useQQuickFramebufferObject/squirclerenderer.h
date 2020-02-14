#ifndef SQUIRCLERENDERER_H
#define SQUIRCLERENDERER_H

#include <QOpenGLFramebufferObject>
#include <QQuickFramebufferObject>
#include <QtGui/QOpenGLShaderProgram>

class Squircle;
class QSGTexture;

class SquircleRenderer : public QObject, public QQuickFramebufferObject::Renderer //public QObject, public QSGRenderNode
{
    Q_OBJECT
public:
    SquircleRenderer(const Squircle* item);
    virtual ~SquircleRenderer() override;
    virtual QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override;

    virtual void render() override;

    void setT(qreal t);
    void setViewportSize(const QSize& size);
    void setWindow(QQuickWindow* window);

public slots:
    void sync();

private:
    QSize m_viewportSize;
    qreal m_t;
    QQuickWindow* m_window;
    const Squircle* m_item;
    QSGTexture* m_texture;
    QOpenGLShaderProgram* m_program;
};

#endif // SQUIRCLERENDERER_H
