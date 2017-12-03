#ifndef SIDEBARWIDGET_H
#define SIDEBARWIDGET_H

#include "declarations.h"

#include <QTableWidget>
#include <QStandardItemModel>
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

    void    setCoresTableModel(QStandardItemModel* model);
    void    setLoadsTableModel(QStandardItemModel* model);

    void    addCore(const Core& core);
    void    editCore(int index, const Core& core);
    void    removeCore(int index);

signals:
    void    sgNodeLoadChanged(int node, double f) const;

    void    addCoreRequest();
    void    editCoreRequest(int core);
    void    removeCoreRequest(int core);

private slots:
    void    nodeLoadChanged(QStandardItem *item) const;

    void    coresContextMenu(const QPoint& point);

private: // Methods
    void    setModelItemData(QStandardItemModel* model, int row, int column, const QVariant& data);

    void    addNode();

    void    setRowData(int row, const Core& core);

    void    onEmptyPlaceMenu(const QPoint &point);
    void    onDataPlaceMenu(const QPoint& point);

private:
    QStandardItemModel* coresModel;
    QStandardItemModel* loadsModel;

    Ui::SidebarWidget *ui;
};

#endif // SIDEBARWIDGET_H
