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

signals:
    void    sgNodeLoadChanged(int node, double f) const;

private slots:
    void    nodeLoadChanged(QTableWidgetItem* item) const;

private: // Methods
    void    addTableItem(QTableWidget* table, int row, int column,
                         const QVariant& data, bool editable = false);

    void    addNode();

private:
    Ui::SidebarWidget *ui;
};

#endif // SIDEBARWIDGET_H
