#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "declarations.h"

#include "processor/processor.h"

#include "edit_widgets/newcoredialog.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);

    coresModel = new QStandardItemModel(0, 5, this);
    coresModel->setHorizontalHeaderLabels({"L", "A", "E", "S", "q"});
    ui->sidebarWidget->setCoresTableModel(coresModel);
    ui->srcFrame->setCores(coresModel);

    loadsModel = new QStandardItemModel(0, 1, this);
    loadsModel->setHorizontalHeaderLabels({"F"});
    ui->sidebarWidget->setLoadsTableModel(loadsModel);
    ui->srcFrame->setCLoads(loadsModel);

    connect(ui->importAction,   &QAction::triggered,    this,   &MainWindow::importProject);
    connect(ui->exportAction,   &QAction::triggered,    this,   &MainWindow::exportProject);

    connect(ui->calculateAction, &QAction::triggered,   this,   &MainWindow::runProcessor);

    connect(ui->newCoreAction,  &QAction::triggered,    this,   &MainWindow::addCore);

    connect(ui->leftSuppAction,  &QAction::toggled,    this,   &MainWindow::leftSupportToggled);
    connect(ui->rightSuppAction, &QAction::toggled,    this,   &MainWindow::rightSupportToggled);

    connect(ui->sidebarWidget,  SIGNAL(dataChanged()),
            ui->srcFrame,       SLOT(repaint()));

    connect(ui->sidebarWidget,  &SidebarWidget::addCoreRequest,     this,  &MainWindow::addCore);
    connect(ui->sidebarWidget,  &SidebarWidget::editCoreRequest,    this,  &MainWindow::editCore);
    connect(ui->sidebarWidget,  &SidebarWidget::removeCoreRequest,  this,  &MainWindow::removeCore);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reset()
{
    coresModel->setRowCount(0);

    loadsModel->setRowCount(1);
    loadsModel->setData(loadsModel->index(0, 0), 0);

    ui->leftSuppAction->setChecked(false);
    ui->rightSuppAction->setChecked(false);
}

void MainWindow::addCore()
{
    NewCoreDialog* dialog = new NewCoreDialog(this);

    if(dialog->exec() == QDialog::Rejected || !dialog->isValid())
        return;

    Core core = dialog->data();

    ui->sidebarWidget->addCore(core);
}

void MainWindow::editCore(int index)
{
    NewCoreDialog* dialog = new NewCoreDialog(ui->srcFrame->core(index), this);

    if(dialog->exec() == QDialog::Rejected || !dialog->isValid())
        return;

    Core core = dialog->data();

    ui->sidebarWidget->editCore(index, core);
}

void MainWindow::removeCore(int core)
{
    QMessageBox msgBox(QMessageBox::Question,
                       "Удаление стержня",
                       "Вы точно хотите удалить стержень?\nЭто действие необратимо.",
                       QMessageBox::Yes | QMessageBox::No,
                       this);

    if(msgBox.exec() != QMessageBox::Yes)
        return;

    ui->sidebarWidget->removeCore(core);
}

void MainWindow::runProcessor()
{
    QVector<Core> cores(coresModel->rowCount());
    for (int i = 0; i < coresModel->rowCount(); i++)
        cores[i] = ui->srcFrame->core(i);

    QVector<double> loads(loadsModel->rowCount());
    for (int i = 0; i < loadsModel->rowCount(); i++)
        loads[i] = qvariant_cast<double>(loadsModel->index(i, 0).data());

    Processor processor(std::move(cores), std::move(loads),
                        ui->leftSuppAction->isChecked(), ui->rightSuppAction->isChecked(), this);
    processor.calculate();
}

void MainWindow::leftSupportToggled(bool checked)
{
    ui->srcFrame->setHasLeftSupport(checked);
    ui->srcFrame->repaint();
}

void MainWindow::rightSupportToggled(bool checked)
{
    ui->srcFrame->setHasRightSupport(checked);
    ui->srcFrame->repaint();
}
