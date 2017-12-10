#include "newcoredialog.h"
#include "ui_newcoredialog.h"

#include <QDebug>

NewCoreDialog::NewCoreDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::NewCoreDialog)
{
    ui->setupUi(this);

    initUi();
}

NewCoreDialog::NewCoreDialog(const Core &data, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::NewCoreDialog)
{
    ui->setupUi(this);

    initUi();
    setData(data);
}

NewCoreDialog::~NewCoreDialog()
{
    delete ui;
}

Core NewCoreDialog::data() const
{
    if(!isValid())
        return {};

    return Core(fieldData(ui->lengthEdit),
                fieldData(ui->areaEdit),
                fieldData(ui->elasticEdit),
                fieldData(ui->strengthEdit),
                fieldData(ui->loadEdit));
}

bool NewCoreDialog::isValid() const
{
    return  isFieldTextValid(ui->lengthEdit) &&
            isFieldTextValid(ui->areaEdit) &&
            isFieldTextValid(ui->elasticEdit) &&
            isFieldTextValid(ui->strengthEdit) &&
            isFieldTextValid(ui->loadEdit) &&
            ui->lengthEdit->text().toDouble() != 0 &&
            ui->areaEdit->text().toDouble() != 0;// &&
//            ui->elasticEdit->text().toDouble() != 0 &&
//            ui->strengthEdit->text().toDouble() != 0;
}

void NewCoreDialog::accept()
{
    if(!isValid())
    {
        ui->error->show();
        return;
    }

    ui->error->hide();

    setResult(QDialog::Accepted);
    emit accepted();

    hide();
}

void NewCoreDialog::initUi()
{
    ui->error->hide();

    QDoubleValidator* doubleValidator = new QDoubleValidator{};
    doubleValidator->setBottom(0);
    //doubleValidator->setLocale(QLocale::English);

    ui->lengthEdit->setValidator(doubleValidator);
    ui->areaEdit->setValidator(doubleValidator);
    ui->elasticEdit->setValidator(doubleValidator);
    ui->strengthEdit->setValidator(doubleValidator);

    ui->loadEdit->setValidator(new QDoubleValidator{});
}

void NewCoreDialog::setData(const Core &data)
{
    ui->lengthEdit->setText(QString::number(data.length));
    ui->areaEdit->setText(QString::number(data.area));
    ui->elasticEdit->setText(QString::number(data.elastic));
    ui->strengthEdit->setText(QString::number(data.strength));
    ui->loadEdit->setText(QString::number(data.load));
}

bool NewCoreDialog::isFieldTextValid(QLineEdit *field) const
{
    QString text = field->text();
    int pos = 0;

    return field->validator()->validate(text, pos) == QValidator::Acceptable;
}

double NewCoreDialog::fieldData(QLineEdit *field) const
{
    return QLocale::system().toDouble(field->text());
}
