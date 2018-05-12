#ifndef CHOICEDEVICEDIALOG_H
#define CHOICEDEVICEDIALOG_H

#include <QDialog>

namespace Ui {
    class ChoiceDeviceDialog;
}

class ChoiceDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChoiceDeviceDialog(QWidget *parent = 0);
    ~ChoiceDeviceDialog();

    void add_item( QString const& text, QVariant const& user_data );
    int current_index() const;

private:
    Ui::ChoiceDeviceDialog* ui;
};

#endif // CHOICEDEVICEDIALOG_H
