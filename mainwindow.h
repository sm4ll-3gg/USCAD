#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

// TODO: Фикс удаления стержня

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void    addCore();
    void    editCore(int index);
    void    removeCore(int core);

private slots:
    void    leftSupportToggled(bool checked);
    void    rightSupportToggled(bool checked);

private: // Methods
    void    addCoresTableItem(int row, int column, QVariant data);

private:
    QStandardItemModel* coresModel;
    QStandardItemModel* loadsModel;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
