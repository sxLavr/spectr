#ifndef TCPCONNECTIONDIALOG_H
#define TCPCONNECTIONDIALOG_H

#include <QDialog>

namespace Ui {
    class TcpConnectionDialog;
}

class TcpConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TcpConnectionDialog( QWidget* parent = 0 );
    ~TcpConnectionDialog();

    Ui::TcpConnectionDialog* _ui;

public slots:
    void slot_data_changed();

private:
    //Ui::TcpConnectionDialog* _ui;
};

#endif // TCPCONNECTIONDIALOG_H
