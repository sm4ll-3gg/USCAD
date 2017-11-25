#ifndef NEWCOREDIALOG_H
#define NEWCOREDIALOG_H

#include <QDialog>

namespace Ui {
class NewCoreDialog;
}

class NewCoreDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewCoreDialog(QWidget *parent = nullptr);

private:
    Ui::NewCoreDialog*  ui;
};

#endif // NEWCOREDIALOG_H
