#ifndef DOCKWIDGETFFT_H
#define DOCKWIDGETFFT_H

#include <QDockWidget>

namespace Ui {
    class DockWidgetFft;
}

class DockWidgetFft : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockWidgetFft(QWidget *parent = 0);
    ~DockWidgetFft();

private:
    Ui::DockWidgetFft *ui;
};

#endif // DOCKWIDGETFFT_H
