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





#pragma mark -

DRConnectDialog::DRConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DRConnectDialog)
{
    ui->setupUi(this);

    ui->progressBar->setHidden(true);
}




DRConnectDialog::~DRConnectDialog()
{
    delete ui;

}





#pragma mark -

void DRConnectDialog::accept()
{
    qDebug() << "Accept";

    wi_url_t            *url;
    wi_p7_socket_t		*socket;
    wi_string_t         *url_string;
    QString             address, login, password;

    address = ui->addressTextField->text();

    if(address == NULL || address.length() <= 0) {
        this->connectError(QString("The address field is madatory."));
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

    this->connection = DRConnectionsController::instance()->connectionForURL(url);
    ui->progressBar->setHidden(false);

    if(this->connection != NULL) {
        DRMainWindow *mainWindow = (DRMainWindow *)this->parent();

        mainWindow->selectConnection(this->connection);

        done(Accepted);
        return;
    }

    wi_log_info(WI_STR("Connecting initial socket..."));

    QThread* thread = new QThread;
    this->connection = new DRConnection(url);
    this->connection->moveToThread(thread);

    connect(this->connection, SIGNAL(connectError(QString)), this, SLOT(connectError(QString)));
    connect(thread, SIGNAL(started()), this->connection, SLOT(connect()));
    connect(this->connection, SIGNAL(connectSucceeded()), this, SLOT(connectSucceeded()));

    thread->start();
}





#pragma mark -

void DRConnectDialog::connectSucceeded() {
    DRConnectionsController::instance()->addConnection(this->connection);

    DRMainWindow *mainWindow = (DRMainWindow *)this->parent();
    mainWindow->show();

    ui->progressBar->setHidden(true);
    done(Accepted);
}



void DRConnectDialog::connectError(QString error) {
    QMessageBox msgBox;

    msgBox.setText("Connection Error");
    msgBox.setInformativeText(error);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    msgBox.exec();
}


