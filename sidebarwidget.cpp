#include "sidebarwidget.h"
#include "ui_sidebarwidget.h"

SidebarWidget::SidebarWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SidebarWidget)
{
    ui->setupUi(this);
}

SidebarWidget::~SidebarWidget()
{
    delete ui;
}

void SidebarWidget::addCore(const Core &core)
{
    addNode();

    int row = ui->coresTable->rowCount();

    ui->coresTable->insertRow(row);

    addTableItem(ui->coresTable, row, 0, core.length);
    addTableItem(ui->coresTable, row, 1, core.area);
    addTableItem(ui->coresTable, row, 2, core.elastic);
    addTableItem(ui->coresTable, row, 3, core.strength);
    addTableItem(ui->coresTable, row, 4, core.load);
}

void SidebarWidget::addTableItem(QTableWidget *table, int row, int column, const QVariant &data)
{
    QTableWidgetItem* item = new QTableWidgetItem(data.toString());
    table->setItem(row, column, item);
}

void SidebarWidget::addNode()
{
    int row = ui->loadsTable->rowCount();

    ui->loadsTable->insertRow(row);

    addTableItem(ui->loadsTable, row, 0, row + 1);
    addTableItem(ui->loadsTable, row, 1, 0);
}
