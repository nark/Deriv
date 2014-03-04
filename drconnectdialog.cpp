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



#include <QDebug>
#include <QMessageBox>
#include <QtCore>
#include <wired/wired.h>
#include "drconnectdialog.h"
#include "ui_drconnectdialog.h"
#include "drmainwindow.h"
#include "drconnectionscontroller.h"
#include "main.h"
#include "dr.h"




#pragma mark -

DRConnectDialog::DRConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DRConnectDialog)
{
    ui->setupUi(this);

}




DRConnectDialog::~DRConnectDialog()
{
    delete ui;

}





#pragma mark -

void DRConnectDialog::accept()
{
    wi_url_t            *url;
    wi_p7_socket_t		*socket;
    wi_string_t         *url_string;
    QString             address, login, password;

    address = ui->addressTextField->text();

    if(address == NULL || address.length() <= 0) {
        QApplication::beep();
        return;
    }

    if(!address.contains(":"))
        address = QString("%1:%2").arg(address).arg(4871);

    login = ui->loginTextField->text();

    if(login == NULL || login.length() <= 0)
        login = "guest";

    password = ui->passwordTextField->text();

    if(password == NULL) {
        url_string = wi_string_with_format(
                    WI_STR("wiredp7://%s@%s"),
                    login.toStdString().c_str(),
                    address.toStdString().c_str());
    } else {
        url_string = wi_string_with_format(
                    WI_STR("wiredp7://%s:%s@%s"),
                    login.toStdString().c_str(),
                    password.toStdString().c_str(),
                    address.toStdString().c_str());
    }

    url = wi_url_with_string(url_string);

    DRMainWindow *mainWindow = (DRMainWindow *)this->parent();

    this->connection = DRConnectionsController::instance()->connectionForURL(url);

    if(this->connection == NULL) {
        this->connection = new DRConnection(url);

        DRConnectionsController::instance()->addConnection(this->connection);

    } else {
        if(this->connection->connected) {
            mainWindow->selectConnection(this->connection);
        }
        done(Accepted);
        return;
    }

    DRMainWindow::startProgress(QString("Connecting to %1").arg(this->connection->URLIdentifier()));
    DR::savePasswordForULRIdentifier(this->connection->URLPassword(), this->connection->URLIdentifier());

    this->connection->connect(mainWindow);

    done(Accepted);
}




