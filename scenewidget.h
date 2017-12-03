#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include "declarations.h"

#include <QFrame>
#include <QMap>

class SceneWidget : public QFrame
{
    Q_OBJECT

    struct Params
    {
        qreal   length{0};
        qreal   heigth{0};
    };

public:
    explicit SceneWidget(QWidget *parent = Q_NULLPTR);

    void    addCore(const Core& core);
    void    editCore(int index, const Core& core);
    void    removeCore(int index);

    void    addCLoad(int node, double f);

    void    setHasLeftSupport(bool has);
    void    setHasRightSupport(bool has);

    const Core& core(int index) const { return cores[index]; }

protected:
    void    paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;

private: // Methods
    void    drawAxis(QPainter& painter);

    void    drawConstruction(QPainter& painter);
    void    drawCore(QPainter& painter, qreal length, qreal area, qreal X_BEGIN_POS);
    void    drawDistributedLoad(QPainter& painter, qreal length, const double Q, qreal X_BEGIN_POS);

    void    drawConcentratedLoads(QPainter& painter);
    void    drawConcentratedLoad(QPainter& painter,  const double F, qreal X_BEGIN_POS);

    void    drawLeftSupport(QPainter& painter, qreal xPos, qreal height);
    void    drawRightSupport(QPainter& painter, qreal xPos, qreal height);

    qreal   calcScale(const Params& params) const;
    Params  calcParams() const;

private:
    QVector<Core>   cores{};
    QVector<qreal>  cLoads{0};

    bool                hasLeftSupport{false};
    bool                hasRightSupport{false};

    const qreal X_BEGIN_POS = 20;

    qreal   scale{30};
};

#endif // SCENEWIDGET_H
