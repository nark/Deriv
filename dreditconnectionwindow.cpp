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
#include "drmainwindow.h"
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

void DREditConnectionWindow::setConnection(DRServerConnection *connection) {
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

        ui->autoConnectCheckBox->setChecked(this->connection->autoConnect);
        ui->autoReconnectCheckBox->setChecked(this->connection->autoReconnect);

    }
}


void DREditConnectionWindow::on_buttonBox_accepted()
{
    if(this->isFormValid()) {
        wi_url_t            *url;
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

        this->connection->setURL(url);
        this->connection->autoConnect = ui->autoConnectCheckBox->isChecked();
        this->connection->autoReconnect = ui->autoReconnectCheckBox->isChecked();

        if(password != NULL) {
            DR::savePasswordForULRIdentifier(password, this->connection->URLIdentifier());
        }

        DRMainWindow::instance()->saveConnections();

        this->close();
    } else {
        QApplication::beep();
    }
}



bool DREditConnectionWindow::isFormValid() {
    if(!ui->addressTextField->text().isEmpty() && !ui->loginTextField->text().isEmpty())
        return true;

    return false;
}

void DREditConnectionWindow::on_buttonBox_rejected()
{
    this->close();
}
