#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include "declarations.h"

#include <QFrame>
#include <QMap>

class SceneWidget : public QFrame
{
    Q_OBJECT
public:
    explicit SceneWidget(QWidget *parent = Q_NULLPTR);

    void    addCore(const Core& core);
    void    addCLoad(int node, double f);

protected:
    void    paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;

private: // Methods
    void    drawAxis(QPainter& painter);

    void    drawConstruction(QPainter& painter);
    void    drawCore(QPainter& painter, qreal length, qreal area, qreal X_BEGIN_POS);
    void    drawDistributedLoad(QPainter& painter, qreal length, const double Q, qreal X_BEGIN_POS);

    void    drawConcentratedLoads(QPainter& painter);
    void    drawConcentratedLoad(QPainter& painter,  const double F, qreal X_BEGIN_POS);

private:
    QVector<Core>       cores{};
    QMap<int, double>   cLoads{};

    const qreal X_BEGIN_POS = 20;

    const int   SCALE = 30;
};

#endif // SCENEWIDGET_H
