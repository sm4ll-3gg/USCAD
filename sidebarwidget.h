#ifndef SIDEBARWIDGET_H
#define SIDEBARWIDGET_H

#include "declarations.h"

#include <QTableWidget>
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

    void    addCore(const Core& core);

private: // Methods
    void    addTableItem(QTableWidget* table, int row, int column, const QVariant& data);

    void    addNode();

private:
    Ui::SidebarWidget *ui;
};

#endif // SIDEBARWIDGET_H
