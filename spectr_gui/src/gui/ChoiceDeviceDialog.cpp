#include "ChoiceDeviceDialog.h"
#include "ui_ChoiceDeviceDialog.h"

ChoiceDeviceDialog::ChoiceDeviceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChoiceDeviceDialog)
{
    ui->setupUi(this);
}

ChoiceDeviceDialog::~ChoiceDeviceDialog()
{
    delete ui;
}

void ChoiceDeviceDialog::add_item( QString const& text, QVariant const& user_data )
{
    ui->cbDeviceList->addItem( text, user_data );
}

int ChoiceDeviceDialog::current_index() const
{
    return ui->cbDeviceList->currentIndex();
}
