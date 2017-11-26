#include "scenewidget.h"

#include <QPaintEvent>
#include <QPainter>
#include <QRect>

SceneWidget::SceneWidget(QWidget *parent)
    : QFrame(parent)
{}

void SceneWidget::addCore(const Core &core)
{
    cores.append(core);
    repaint();
}

void SceneWidget::addCLoad(int node, double f)
{
    cLoads[node] = f;
    repaint();
}

void SceneWidget::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawAxis(painter);
    drawConstruction(painter);
    drawConcentratedLoads(painter);
}

void SceneWidget::drawAxis(QPainter &painter)
{
    painter.setPen(QPen(Qt::gray, 2, Qt::DashDotLine));

    qreal yPos = rect().height() / 2;
    painter.drawLine(QPointF(0, yPos), QPointF(rect().width(), yPos));
}

void SceneWidget::drawConstruction(QPainter &painter)
{
    qreal pos = X_BEGIN_POS;
    for(Core core : cores)
    {
        drawCore(painter, core.length, core.area, pos);

        if(core.load != 0)
            drawDistributedLoad(painter, core.length * SCALE, core.load, pos);

        pos += core.length * SCALE;
    }
}

void SceneWidget::drawCore(QPainter &painter, qreal length, qreal area, qreal xBeginPos)
{
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(Qt::black, 4, Qt::SolidLine));

    qreal yBeginPos = rect().height() / 2 - area * SCALE / 2;

    painter.drawRect(xBeginPos, yBeginPos, length * SCALE, area * SCALE);
}

void SceneWidget::drawDistributedLoad(QPainter &painter, qreal length,
                                      const double Q, qreal xBeginPos)
{
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine));

    qreal xEndPos = xBeginPos + length;

    qreal yAxisPos = rect().height() / 2;
    painter.drawLine(xBeginPos, yAxisPos, xEndPos, yAxisPos);

    qreal deltaX = 5;
    qreal deltaY = 5;
    qreal step = 10;

    qreal xPos = xBeginPos + step;
    while (xPos < xEndPos)
    {
        qreal xOffset =  xPos - (Q > 0 ? deltaX : -deltaX);

        QPointF arrow[3];
        arrow[0] = QPointF(xOffset, yAxisPos - deltaY);
        arrow[1] = QPointF(xPos, yAxisPos);
        arrow[2] = QPointF(xOffset, yAxisPos + deltaY);

        painter.drawPolyline(arrow, 3);

        xPos += step;
    }
}

void SceneWidget::drawConcentratedLoads(QPainter &painter)
{
    qreal pos = X_BEGIN_POS;
    for(int node = 0; node <= cores.size(); ++node)
    {
        const double F = cLoads.value(node, 0);

        qreal endPos = pos;
        if(node < cores.size())
            endPos += cores[node].length * SCALE;

        if(F == 0)
        {
            pos = endPos;
            continue;
        }

        drawConcentratedLoad(painter, F, pos);
        pos = endPos;
    }
}

void SceneWidget::drawConcentratedLoad(QPainter &painter, const double F, qreal xBeginPos)
{
    painter.setBrush(Qt::SolidPattern);
    painter.setPen(QPen(Qt::black, 0, Qt::SolidLine));

    qreal yAxisPos = rect().height() / 2;

    qreal step = 15;

    QPointF triangle[3];
    triangle[0] = QPointF(xBeginPos, yAxisPos - step);
    triangle[1] = QPointF(xBeginPos + (F > 0 ? step : -step), yAxisPos);
    triangle[2] = QPointF(xBeginPos, yAxisPos + step);

    painter.drawPolygon(triangle, 3);
}
