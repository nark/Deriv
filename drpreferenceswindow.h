#ifndef DRPREFERENCESWINDOW_H
#define DRPREFERENCESWINDOW_H

#include <QMainWindow>

namespace Ui {
class DRPreferencesWindow;
}

class DRPreferencesWindow : public QMainWindow
{
    Q_OBJECT

public:
    static DRPreferencesWindow*         instance(QWidget* parent = 0);
    static void                         drop();

private slots:
    void on_lineEdit_editingFinished();

private:
    explicit                            DRPreferencesWindow(QWidget *parent = 0);
                                        ~DRPreferencesWindow();

private:
    Ui::DRPreferencesWindow *ui;
};

#endif // DRPREFERENCESWINDOW_H
