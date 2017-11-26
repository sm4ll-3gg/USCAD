#include "scenewidget.h"

#include <QPaintEvent>
#include <QPainter>
#include <QRect>

SceneWidget::SceneWidget(QWidget *parent)
    : QFrame(parent)
{

}

void SceneWidget::addCore(const Core &core)
{
    cores.append(core);
    repaint();
}

void SceneWidget::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawAxis(painter);
    drawConstruction(painter);
}

void SceneWidget::drawAxis(QPainter &painter)
{
    painter.setPen(QPen(Qt::gray, 2, Qt::DashDotLine));

    qreal yPos = rect().height() / 2;
    painter.drawLine(QPointF(0, yPos), QPointF(rect().width(), yPos));
}

void SceneWidget::drawConstruction(QPainter &painter)
{
    qreal xBeginPos = 10;
    for(Core core : cores)
    {
        drawCore(painter, core.length, core.area, xBeginPos);

        if(core.load != 0)
            drawDistributedLoad(painter, core.length * mToPixScale, xBeginPos);

        xBeginPos += core.length * mToPixScale;
    }
}

void SceneWidget::drawCore(QPainter &painter, qreal length, qreal area, qreal xBeginPos)
{
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(Qt::black, 4, Qt::SolidLine));

    qreal yBeginPos = rect().height() / 2 - area * mToPixScale / 2;

    painter.drawRect(xBeginPos, yBeginPos, length * mToPixScale, area * mToPixScale);
}

void SceneWidget::drawDistributedLoad(QPainter &painter, qreal length, qreal xBeginPos)
{
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine));

    qreal xEndPos = xBeginPos + length;

    qreal yAxisPos = rect().height() / 2;
    painter.drawLine(xBeginPos, yAxisPos, xEndPos, yAxisPos);

    qreal deltaX = 5;
    qreal deltaY = 5;
    qreal step = 10;

    qreal xPos = xBeginPos + step;
    while (xPos <= xEndPos)
    {
        QPointF arrow[3];
        arrow[0] = QPointF(xPos - deltaX, yAxisPos - deltaY);
        arrow[1] = QPointF(xPos, yAxisPos);
        arrow[2] = QPointF(xPos - deltaX, yAxisPos + deltaY);

        painter.drawPolyline(arrow, 3);

        xPos += step;
    }
}
