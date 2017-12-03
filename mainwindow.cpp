#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "declarations.h"

#include "edit_widgets/newcoredialog.h"

#include <QDebug>
#include <QFileDialog>
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

void MainWindow::importProject()
{

}

void MainWindow::exportProject()
{
    QString path = QFileDialog::getSaveFileName(this, "Сохранение проекта",
                                                QDir::homePath(), "*.scad");

    if(path.isEmpty())
        return;

    QJsonDocument project = serialize();

    QFile file(path);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox box(QMessageBox::Warning, "Ошибка сохранения проекта",
                        file.errorString(), QMessageBox::Ok, this);
        box.exec();

        qWarning() << "Save project error: " << file.errorString();
        return;
    }

    file.write(project.toJson());
    file.close();
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

void MainWindow::leftSupportToggled(bool checked)
{
    ui->srcFrame->setHasLeftSupport(checked);
}

void MainWindow::rightSupportToggled(bool checked)
{
    ui->srcFrame->setHasRightSupport(checked);
}

QJsonDocument MainWindow::serialize() const
{
    QJsonObject project;

    project.insert("cores", serializeCores());
    project.insert("cLoads", serializeLoads());
    project.insert("supports", serializeSupports());

    return QJsonDocument{project};
}

QJsonArray MainWindow::serializeCores() const
{
    QJsonArray cores{};

    for(int i = 0; i < coresModel->rowCount(); ++i)
        cores.append(serializeCore(i));

    return cores;
}

QJsonObject MainWindow::serializeCore(int row) const
{
    Core core = ui->srcFrame->core(row);

    QJsonObject coreJson{};

    coreJson.insert("L", core.length);
    coreJson.insert("A", core.area);
    coreJson.insert("E", core.elastic);
    coreJson.insert("S", core.strength);
    coreJson.insert("q", core.load);

    return coreJson;
}

QJsonArray MainWindow::serializeLoads() const
{
    QJsonArray loads{};

    for(int i = 0; i < loadsModel->rowCount(); ++i)
        loads.append(loadsModel->index(i, 0).data().toDouble());

    return loads;
}

QJsonObject MainWindow::serializeSupports() const
{
    QJsonObject supports{};

    supports.insert("left", ui->leftSuppAction->isChecked());
    supports.insert("right", ui->rightSuppAction->isChecked());

    return supports;
}
