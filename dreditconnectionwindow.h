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


#ifndef DREDITCONNECTIONWINDOW_H
#define DREDITCONNECTIONWINDOW_H

#include <QMainWindow>
#include "drconnection.h"

namespace Ui {
class DREditConnectionWindow;
}

class DREditConnectionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DREditConnectionWindow(QWidget *parent = 0);
    ~DREditConnectionWindow();

    void setConnection(DRConnection *connection);

private:
    Ui::DREditConnectionWindow *ui;
    DRConnection *connection;
};

#endif // DREDITCONNECTIONWINDOW_H
