#pragma once
#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>
#include <qsgrendernode.h>
#include <QtQuick/QQuickFramebufferObject>
#include <QOpenGLFramebufferObject>

class Squircle;
class QSGTexture;

class SquircleRenderer : public QObject, public QQuickFramebufferObject::Renderer//public QObject, public QSGRenderNode
{
    Q_OBJECT
public:
    SquircleRenderer(const Squircle *item);
    virtual ~SquircleRenderer() override;
    virtual QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override
    {
        QOpenGLFramebufferObjectFormat format;
        format.setSamples(1);
        return new QOpenGLFramebufferObject(size, format);
    }

    virtual void render() override;

    void setT(qreal t) { m_t = t; }
    void setViewportSize(const QSize &size) { m_viewportSize = size; }
    void setWindow(QQuickWindow *window) { m_window = window; }
    //virtual void render(const RenderState* stateRender) override;
public slots:
    void sync();
private:
    QSize m_viewportSize;
    qreal m_t;
    QQuickWindow *m_window;
    const Squircle * m_item;
    QSGTexture* m_texture;
    QOpenGLShaderProgram *m_program;
};

class Squircle : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
    Q_PROPERTY(QString texture READ texture WRITE setTexture)
public:
    Squircle();

    qreal t() const { return m_t; }
    void setT(qreal t);
    QString texture() const { return m_texture; }
    void setTexture(const QString& texture) { m_texture = texture; }

    QSize getViewportSize() const { return m_viewportSize; }
signals:
    void tChanged();

public:
    virtual Renderer* createRenderer() const override;
    virtual void componentComplete() override;


protected:
   /// virtual QSGNode* updatePaintNode(QSGNode* nodeSG, UpdatePaintNodeData*) override;
public slots:
    void cleanup();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    qreal m_t;
    QString m_texture;
    QSize m_viewportSize;
    SquircleRenderer *m_renderer;
};
