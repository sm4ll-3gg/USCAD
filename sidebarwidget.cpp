#include "sidebarwidget.h"
#include "ui_sidebarwidget.h"

SidebarWidget::SidebarWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SidebarWidget)
{
    ui->setupUi(this);
}

SidebarWidget::~SidebarWidget()
{
    delete ui;
}
