#ifndef DRABOUTWINDOW_H
#define DRABOUTWINDOW_H

#include <QMainWindow>

namespace Ui {
class DRAboutWindow;
}

class DRAboutWindow : public QMainWindow
{
    Q_OBJECT

public:
    static DRAboutWindow*   instance(QWidget* parent = 0);
    static void             drop();

private:
    explicit                DRAboutWindow(QWidget *parent = 0);
                            ~DRAboutWindow();

    Ui::DRAboutWindow       *ui;
};

#endif // DRABOUTWINDOW_H
