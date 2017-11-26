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
            drawDistributedLoad(painter, core.load, xBeginPos);

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

void SceneWidget::drawDistributedLoad(QPainter &painter, int q, qreal xBeginPos)
{
    painter.setBrush(QBrush(QPixmap("arrow.png")));
    painter.setPen(QPen(Qt::black, 4));

    painter.drawLine(xBeginPos, rect().height() / 2, xBeginPos + q * mToPixScale, rect().height() / 2);
}
