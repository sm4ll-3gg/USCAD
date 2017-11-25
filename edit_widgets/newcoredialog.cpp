#include "newcoredialog.h"
#include "ui_newcoredialog.h"

NewCoreDialog::NewCoreDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::NewCoreDialog)
{
    ui->setupUi(this);
}
