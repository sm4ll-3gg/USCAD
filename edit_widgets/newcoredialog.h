#ifndef NEWCOREDIALOG_H
#define NEWCOREDIALOG_H

#include "declarations.h"

#include <QDialog>
#include <QVector>
#include <QLineEdit>

namespace Ui {
class NewCoreDialog;
}

class NewCoreDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewCoreDialog(QWidget *parent = Q_NULLPTR);
    NewCoreDialog(const Core& data, QWidget* parent = Q_NULLPTR);
    ~NewCoreDialog();

    Core    data() const;

    bool    isValid() const;

public slots:
    void    accept() Q_DECL_OVERRIDE;

private: // Methods
    void    initUi();

    void    setData(const Core& data);

    bool    isFieldTextValid(QLineEdit* field) const;
    double  fieldData(QLineEdit* field) const;

private:
    Ui::NewCoreDialog*  ui;
};

#endif // NEWCOREDIALOG_H
