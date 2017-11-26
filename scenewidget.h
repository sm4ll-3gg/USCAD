#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include "declarations.h"

#include <QFrame>

class SceneWidget : public QFrame
{
    Q_OBJECT
public:
    explicit SceneWidget(QWidget *parent = Q_NULLPTR);

    void    addCore(const Core& core);

protected:
    void    paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;

private: // Methods
    void    drawAxis(QPainter& painter);

    void    drawConstruction(QPainter& painter);
    void    drawCore(QPainter& painter, qreal length, qreal area, qreal xBeginPos);
    void    drawDistributedLoad(QPainter& painter, qreal length, qreal xBeginPos);

private:
    QVector<Core>   cores{};

    const int   mToPixScale = 30;
};

#endif // SCENEWIDGET_H
