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

#include "dreditconnectionwindow.h"
#include "ui_dreditconnectionwindow.h"
#include "dr.h"



#pragma mark -

DREditConnectionWindow::DREditConnectionWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DREditConnectionWindow)
{
    ui->setupUi(this);

    this->connection = NULL;
}

DREditConnectionWindow::~DREditConnectionWindow()
{
    delete ui;
}




#pragma mark -

void DREditConnectionWindow::setConnection(DRConnection *connection) {
    this->connection = connection;

    if(this->connection != NULL) {
        ui->addressTextField->setEnabled(!this->connection->connected);
        ui->loginTextField->setEnabled(!this->connection->connected);
        ui->passwordTextField->setEnabled(!this->connection->connected);

        QString addressString = QString(wi_string_cstring(wi_url_host(connection->url)));

        int port = wi_url_port(connection->url);
        if(port > 0 && port != 4871)
            addressString += ":" + QString::number(port);

        ui->addressTextField->setText(addressString);

        QString login = QString(wi_string_cstring(wi_url_user(connection->url)));
        ui->loginTextField->setText(login);

        QString password = DR::loadPasswordForULRIdentifier(this->connection->URLIdentifier());
        ui->passwordTextField->setText(password);
    }
}
