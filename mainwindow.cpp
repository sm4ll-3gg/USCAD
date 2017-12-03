#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "declarations.h"

#include "edit_widgets/newcoredialog.h"

#include <QDebug>
#include <QTableWidgetItem>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);

    coresModel = new QStandardItemModel(0, 5, this);
    coresModel->setHorizontalHeaderLabels({"L", "A", "E", "S", "q"});
    ui->sidebarWidget->setCoresTableModel(coresModel);

    loadsModel = new LoadsModel(0, 2, this);
    loadsModel->setHorizontalHeaderLabels({"Node", "F"});
    ui->sidebarWidget->setLoadsTableModel(loadsModel);

    connect(ui->newCoreAction,  &QAction::triggered,  this,   &MainWindow::addCore);

    connect(ui->leftSuppAction,  &QAction::toggled,    this,   &MainWindow::leftSupportToggled);
    connect(ui->rightSuppAction, &QAction::toggled,    this,   &MainWindow::rightSupportToggled);

    connect(ui->sidebarWidget,  &SidebarWidget::addCoreRequest,     this,  &MainWindow::addCore);
    connect(ui->sidebarWidget,  &SidebarWidget::editCoreRequest,    this,  &MainWindow::editCore);
    connect(ui->sidebarWidget,  &SidebarWidget::removeCoreRequest,  this,  &MainWindow::removeCore);

    connect(ui->sidebarWidget,  &SidebarWidget::sgNodeLoadChanged,  this,   &MainWindow::addCLoad);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addCore()
{
    NewCoreDialog* dialog = new NewCoreDialog(this);

    if(dialog->exec() == QDialog::Rejected || !dialog->isValid())
        return;

    Core core = dialog->data();

    ui->sidebarWidget->addCore(core);
   // ui->srcFrame->addCore(core);
}

void MainWindow::editCore(int index)
{
    NewCoreDialog* dialog = new NewCoreDialog(ui->srcFrame->core(index), this);

    if(dialog->exec() == QDialog::Rejected || !dialog->isValid())
        return;

    Core core = dialog->data();

    ui->sidebarWidget->editCore(index, core);
    ui->srcFrame->editCore(index, core);
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
    ui->srcFrame->removeCore(core);
}

void MainWindow::addCLoad(int node, double f)
{
    ui->srcFrame->addCLoad(node, f);
}

void MainWindow::leftSupportToggled(bool checked)
{
    ui->srcFrame->setHasLeftSupport(checked);
}

void MainWindow::rightSupportToggled(bool checked)
{
    ui->srcFrame->setHasRightSupport(checked);
}
