#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void    addCore(); // Дублирование данных, если будет не лень переписать, используя модель
    void    addCLoad(int node, double f);

private: // Methods
    void    addCoresTableItem(int row, int column, QVariant data);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
