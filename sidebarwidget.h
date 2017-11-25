#ifndef SIDEBARWIDGET_H
#define SIDEBARWIDGET_H

#include <QWidget>

namespace Ui {
class SidebarWidget;
}

class SidebarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SidebarWidget(QWidget *parent = 0);
    ~SidebarWidget();

private:
    Ui::SidebarWidget *ui;
};

#endif // SIDEBARWIDGET_H
