#pragma once
#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>

class SquircleRenderer : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    SquircleRenderer() : m_t(0), m_program(nullptr) { }
    virtual ~SquircleRenderer();

    void setT(qreal t) { m_t = t; }
    void setViewportSize(const QSize &size) { m_viewportSize = size; }
    void setWindow(QQuickWindow *window) { m_window = window; }
    int m_x;
    int m_y;
public slots:
    void paint();
private:
    QSize m_viewportSize;
    qreal m_t;

    QOpenGLShaderProgram *m_program;
    QQuickWindow *m_window;
};

class Squircle : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
public:
    Squircle();

    qreal t() const { return m_t; }
    void setT(qreal t);
signals:
    void tChanged();

public slots:
    void sync();
    void cleanup();

private slots:
    void handleWindowChanged(QQuickWindow *win);
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
private:
    qreal m_t;
    SquircleRenderer *m_renderer;
    int m_x;
    int m_y;
};
