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


#ifndef DRCONNECTDIALOG_H
#define DRCONNECTDIALOG_H

#include <QDialog>
#include "drconnection.h"


namespace Ui {
class DRConnectDialog;
}

class DRConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DRConnectDialog(QWidget *parent = 0);
    ~DRConnectDialog();

public slots:
    void connectSucceeded();
    void connectError(QString error);

private:
    void accept();

    Ui::DRConnectDialog *ui;

    DRConnection *connection;
};

#endif // DRCONNECTDIALOG_H
