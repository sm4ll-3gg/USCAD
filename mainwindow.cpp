#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "declarations.h"

#include "edit_widgets/newcoredialog.h"

#include <QMessageBox>
#include <QLabel>

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

    connect(ui->sidebarWidget, &SidebarWidget::sgPointCalcRequested, this, &MainWindow::pointCalc);

    connect(ui->sidebarWidget, &SidebarWidget::sgSwitchStressType,
            ui->srcFrame, &SceneWidget::setStressType);
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

    ui->srcFrame->setResult(QVector<QVector<Stress>>());
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

    processor.setCores(std::move(cores));
    processor.setLoads(std::move(loads));
    processor.setSupports(ui->leftSuppAction->isChecked(), ui->rightSuppAction->isChecked());
    processor.setPointCount(ui->sidebarWidget->pointCount());

    processor.prepare();

    QVector<QVector<Stress>> stress = processor.calculate(ui->sidebarWidget->pointCount());
    showCalcResults(stress);
    ui->srcFrame->setResult(stress);
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

void MainWindow::pointCalc(int core, double pos)
{
    runProcessor();

    Stress stress = processor.calcInPoint(core, pos);

    QString text = QString("Результаты расчета в точке %1 стержня №%2:\n"
                           "Nx = %3\n"
                           "Ux = %4\n"
                           "Sx = %5\n")
            .arg(pos)
            .arg(core)
            .arg(stress.nx)
            .arg(stress.ux)
            .arg(stress.sx);

    QMessageBox dialog(QMessageBox::Information, "Расчет в точке", text, QMessageBox::Ok, this);
    dialog.exec();
}

void MainWindow::showCalcResults(const QVector<QVector<Stress>> &result)
{
    QVBoxLayout* layout = new QVBoxLayout();

    for (int i = 0; i < result.size(); i++)
    {
        layout->addWidget(new QLabel(QString("Стержень №%1").arg(i + 1)));

        QTableWidget* table = new QTableWidget(result[i].size(), 5, this);
        table->setHorizontalHeaderLabels({"Pos", "Nx", "Ux", "Sx", "S"});

        for (int j = 0; j < result[i].size(); j++)
        {
            Stress stress = result[i][j];

            QTableWidgetItem* pos = new QTableWidgetItem(QString::number(stress.pos));
            table->setItem(j, 0, pos);

            QTableWidgetItem* nx = new QTableWidgetItem(QString::number(stress.nx));
            table->setItem(j, 1, nx);

            QTableWidgetItem* ux = new QTableWidgetItem(QString::number(stress.ux));
            table->setItem(j, 2, ux);

            QTableWidgetItem* sx = new QTableWidgetItem(QString::number(stress.sx));
            table->setItem(j, 3, sx);
        }

        layout->addWidget(table);
    }

    QDialog* dialog = new QDialog(this);
    dialog->setLayout(layout);

    dialog->exec();

    dialog->deleteLater();
}
