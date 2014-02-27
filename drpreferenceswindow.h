#ifndef DRPREFERENCESWINDOW_H
#define DRPREFERENCESWINDOW_H

#include <QMainWindow>
#include <QSettings>


namespace Ui {
class DRPreferencesWindow;
}

class DRPreferencesWindow : public QMainWindow
{
    Q_OBJECT

public:
    static DRPreferencesWindow*         instance(QWidget* parent = 0);
    static void                         drop();

    void                                show();
    QSettings                           *settings;

private slots:
    void on_nickLabel_editingFinished();

private:
    explicit                            DRPreferencesWindow(QWidget *parent = 0);
                                        ~DRPreferencesWindow();

private:
    Ui::DRPreferencesWindow *ui;
};

#endif // DRPREFERENCESWINDOW_H
