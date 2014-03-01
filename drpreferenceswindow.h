/*
    Deriv is a cross-platform client for th Wired 2.0 protocol
    Copyright (C) 2014  Rafael Warnault, rw@read-write.fr

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#ifndef DRPREFERENCESWINDOW_H
#define DRPREFERENCESWINDOW_H

#include <QMainWindow>
#include <QSettings>



#define DRDefaultNick                   "DRDefaultNick"
#define DRDefaultStatus                 "DRDefaultStatus"
#define DRShowConnectAtStartup          "DRShowConnectAtStartup"



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

signals:
    void userNickDidChange(QString nick);
    void userStatusDidChange(QString status);

private slots:
    void on_nickField_editingFinished();
    void on_statusField_editingFinished();
    void on_showConnectAtStartupCheckBox_clicked();

private:
    explicit                            DRPreferencesWindow(QWidget *parent = 0);
                                        ~DRPreferencesWindow();

private:
    Ui::DRPreferencesWindow *ui;

    void setDefault();
    void loadSettings();
};

#endif // DRPREFERENCESWINDOW_H
