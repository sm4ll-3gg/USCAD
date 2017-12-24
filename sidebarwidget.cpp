#include "sidebarwidget.h"
#include "ui_sidebarwidget.h"

#include <QDebug>
#include <QMenu>

SidebarWidget::SidebarWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SidebarWidget)
{
    ui->setupUi(this);

    ui->coresTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->coresTable, &QTableWidget::customContextMenuRequested,
            this,           &SidebarWidget::coresContextMenu);

    connect(ui->coreNumSpin,    SIGNAL(valueChanged(int)),
            this,               SLOT  (calcCoreChanged(int)));
    connect(ui->calcButton, &QPushButton::clicked, this, &SidebarWidget::calcButtonClicked);
    connect(ui->stressType, &QComboBox::currentTextChanged, [=](const QString& text){
        SceneWidget::StressType type = SceneWidget::StressType::NX;

        if (text == "Nx")
            type = SceneWidget::StressType::NX;
        else if (text == "Ux")
            type = SceneWidget::StressType::UX;
        else if (text == "Sx")
            type = SceneWidget::StressType::SX;

        emit sgSwitchStressType(type);
    });
}

SidebarWidget::~SidebarWidget()
{
    delete ui;
}

void SidebarWidget::setCoresTableModel(QStandardItemModel *model)
{
    coresModel = model;
    connect(coresModel, &QStandardItemModel::dataChanged, this, &SidebarWidget::dataChanged);

    ui->coresTable->setModel(model);
}

void SidebarWidget::setLoadsTableModel(QStandardItemModel *model)
{
    loadsModel = model;
    connect(loadsModel, &QStandardItemModel::itemChanged, this, &SidebarWidget::nodeLoadChanged);
    connect(loadsModel, &QStandardItemModel::dataChanged, this, &SidebarWidget::dataChanged);

    addNode();

    ui->loadsTable->setModel(model);
}

void SidebarWidget::addCore(const Core &core)
{
    addNode();

    int row = coresModel->rowCount();

    coresModel->insertRow(row);

    setRowData(row, core);

    ui->coreNumSpin->setRange(1, row + 1);
}

void SidebarWidget::editCore(int index, const Core &core)
{
    setRowData(index, core);
}

void SidebarWidget::removeCore(int index)
{
    coresModel->removeRow(index);
    loadsModel->removeRow(index + 1);

    emit dataChanged();
}

int SidebarWidget::pointCount() const
{
    return ui->partCountSpin->value();
}

void SidebarWidget::nodeLoadChanged(QStandardItem* item) const
{
    int node = item->row();
    double f = item->data(Qt::DisplayRole).toDouble();

    emit sgNodeLoadChanged(node, f);
}

void SidebarWidget::calcButtonClicked() const
{    
    if(coresModel->rowCount() == 0)
        return;

    int core = ui->coreNumSpin->value() - 1;
    int pos  = ui->pointSpin->value();

    emit sgPointCalcRequested(core, pos);
}

void SidebarWidget::calcCoreChanged(int core) const
{
    double length = qvariant_cast<double>(coresModel->index(core - 1, 0).data());

    ui->pointSpin->setMaximum(length);
}

void SidebarWidget::coresContextMenu(const QPoint &point)
{
    if(ui->coresTable->indexAt(point).isValid())
        onDataPlaceMenu(point);
    else
        onEmptyPlaceMenu(point);
}

void SidebarWidget::setModelItemData(QStandardItemModel *model, int row, int column,
                                     const QVariant &data)
{
    model->setItem(row, column, new QStandardItem(data.toString()));
}

void SidebarWidget::addNode()
{
    int row = loadsModel->rowCount();

    loadsModel->insertRow(row);
    setModelItemData(loadsModel, row, 0, 0);
}

void SidebarWidget::setRowData(int row, const Core &core)
{
    setModelItemData(coresModel, row, 0, core.length);
    setModelItemData(coresModel, row, 1, core.area);
    setModelItemData(coresModel, row, 2, core.elastic);
    setModelItemData(coresModel, row, 3, core.strength);
    setModelItemData(coresModel, row, 4, core.load);
}

void SidebarWidget::onEmptyPlaceMenu(const QPoint& point)
{
    QMenu* menu = new QMenu(this);

    QAction* createCore = new QAction("Добавить стержень");
    connect(createCore, &QAction::triggered, this, &SidebarWidget::addCoreRequest);
    menu->addAction(createCore);

    menu->exec(ui->coresTable->viewport()->mapToGlobal(point));

    menu->deleteLater();
}

void SidebarWidget::onDataPlaceMenu(const QPoint& point)
{
    int row = ui->coresTable->indexAt(point).row();

    QMenu* menu = new QMenu(this);

    QAction* createCore = new QAction("Добавить стержень");
    connect(createCore, &QAction::triggered, this, &SidebarWidget::addCoreRequest);
    menu->addAction(createCore);


    QAction* editCore = new QAction("Редактировать");
    connect(editCore, &QAction::triggered, [this, row] () { emit editCoreRequest(row);});
    menu->addAction(editCore);

    QAction* removeCore = new QAction("Удалить");
    connect(removeCore, &QAction::triggered, [this, row] () { emit removeCoreRequest(row);});
    menu->addAction(removeCore);

    menu->exec(ui->coresTable->viewport()->mapToGlobal(point));

    menu->deleteLater();
}
