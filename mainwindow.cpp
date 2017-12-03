#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "declarations.h"

#include "edit_widgets/newcoredialog.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonParseError>

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
    QString path = QFileDialog::getOpenFileName(this, "Открытие проекта",
                                                QDir::homePath(), "*.scad");

    if(path.isEmpty())
        return;

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        projectIOError("Ошибка при открытии проекта", file.errorString());
        return;
    }

    QByteArray data = file.readAll();
    QJsonParseError error{};
    QJsonDocument project = QJsonDocument::fromJson(data, &error);

    if(error.error != QJsonParseError::NoError)
    {
        projectIOError("Ошибка чтения файла проекта", error.errorString());
        return;
    }

    coresModel->setRowCount(0);
    loadsModel->setRowCount(0);

    deserialize(project.object());
}

void MainWindow::exportProject()
{
    QString name = QString("%1/project.scad").arg(QDir::homePath());
    QString path = QFileDialog::getSaveFileName(this, "Сохранение проекта",
                                                name, "*.scad");

    if(path.isEmpty())
        return;

    QJsonDocument project = serialize();

    QFile file(path);
    if(!file.open(QIODevice::WriteOnly))
    {
        projectIOError("Ошибка при сохранении проекта", file.errorString());
        return;
    }

    file.write(project.toJson());
    file.close();
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

void MainWindow::deserialize(const QJsonObject &project)
{
    disconnect(ui->sidebarWidget, SIGNAL(dataChanged()), ui->srcFrame, SLOT(repaint()));

    if(!deserializeCores(project.value("cores").toArray()) ||
            !deserializeLoads(project.value("cLoads").toArray()) ||
            !deserializeSupports(project.value("supports").toObject()))
        projectImportError();

    connect(ui->sidebarWidget, SIGNAL(dataChanged()), ui->srcFrame, SLOT(repaint()));
    ui->sidebarWidget->repaint();
}

bool MainWindow::deserializeCores(const QJsonArray &cores)
{
    for(QJsonValue val : cores)
    {
        QJsonObject core = val.toObject();
        if(core.isEmpty())
            return false;

        ui->sidebarWidget->addCore(deserializeCore(core));
    }

    return true;
}

Core MainWindow::deserializeCore(const QJsonObject &json) const
{
    Core core{};

    core.length = json.value("L").toDouble();
    core.area = json.value("A").toDouble();
    core.elastic = json.value("E").toDouble();
    core.strength = json.value("S").toDouble();
    core.load = json.value("q").toDouble();

    return core;
}

bool MainWindow::deserializeLoads(const QJsonArray &loads)
{
    loadsModel->setRowCount(0);
    for(QJsonValue val : loads)
    {
        if(!val.isDouble())
            return false;

        double load = val.toDouble();
        loadsModel->appendRow(new QStandardItem{QString::number(load)});
    }

    return true;
}

bool MainWindow::deserializeSupports(const QJsonObject &supports)
{
    if(QJsonValue val = supports.value("left"); val.isBool())
    {
        bool has = val.toBool();

        ui->leftSuppAction->setChecked(has);
        ui->srcFrame->setHasLeftSupport(has);
    }
    else
        return false;

    if(QJsonValue val = supports.value("right"); val.isBool())
    {
        bool has = val.toBool();

        ui->rightSuppAction->setChecked(has);
        ui->srcFrame->setHasRightSupport(has);
    }
    else
        return false;

    return true;
}

void MainWindow::projectImportError()
{
    reset();

    projectIOError("Ошибка чтения файла проекта",
                   "Файл проекта поврежден. Проект не может быть импортирован.");
}

void MainWindow::projectIOError(const QString& title, const QString &error)
{
    QMessageBox box(QMessageBox::Warning, title,
                    error, QMessageBox::Ok, this);
    box.exec();

    qWarning() << title << ": " << error;
}
