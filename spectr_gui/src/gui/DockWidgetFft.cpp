#include "DockWidgetFft.h"
#include "ui_DockWidgetFft.h"

DockWidgetFft::DockWidgetFft(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DockWidgetFft)
{
    ui->setupUi(this);
}

DockWidgetFft::~DockWidgetFft()
{
    delete ui;
}
