#ifndef SQUIRCLE_H
#define SQUIRCLE_H

#include "squirclerenderer.h"
#include <QQuickFramebufferObject>

class Squircle : public QQuickFramebufferObject {
    Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
    Q_PROPERTY(QString texture READ texture WRITE setTexture)
public:
    Squircle();

    qreal t() const;
    void setT(qreal t);
    QString texture() const;
    void setTexture(const QString& texture);

    QSize getViewportSize() const;
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
    void handleWindowChanged(QQuickWindow* win);

private:
    qreal m_t;
    QString m_texture;
    QSize m_viewportSize;
    SquircleRenderer* m_renderer;
};

#endif // SQUIRCLE_H
