#include "newcoredialog.h"
#include "ui_newcoredialog.h"

NewCoreDialog::NewCoreDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::NewCoreDialog)
{
    ui->setupUi(this);

    initUi();
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
            isFieldTextValid(ui->loadEdit);
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

    ui->lengthEdit->setValidator(doubleValidator);
    ui->areaEdit->setValidator(doubleValidator);
    ui->elasticEdit->setValidator(doubleValidator);
    ui->strengthEdit->setValidator(doubleValidator);

    ui->loadEdit->setValidator(new QDoubleValidator{});
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
