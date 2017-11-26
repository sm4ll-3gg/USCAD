#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "declarations.h"

#include "edit_widgets/newcoredialog.h"

#include <QTableWidgetItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->newCoreAction,  &QAction::triggered,  this,   &MainWindow::addCore);

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
    ui->srcFrame->addCore(core);
}

void MainWindow::addCLoad(int node, double f)
{
    ui->srcFrame->addCLoad(node, f);
}
