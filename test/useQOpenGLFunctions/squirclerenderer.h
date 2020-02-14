#ifndef SQUIRCLERENDERER_H
#define SQUIRCLERENDERER_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QQuickItem>

class SquircleRenderer : public QObject, protected QOpenGLFunctions {
    Q_OBJECT

public:
    SquircleRenderer();
    virtual ~SquircleRenderer();

    void setT(qreal t);

    void setViewportSize(const QSize& size);

    void setWindow(QQuickWindow* window);

public slots:
    void paint();

private:
    QSize m_viewportSize;
    qreal m_t;
    QOpenGLShaderProgram* m_program;
    QQuickWindow* m_window;
};

#endif // SQUIRCLERENDERER_H
