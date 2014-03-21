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
#include "drserverinfodialog.h"
#include "ui_drserverinfodialog.h"
#include "dr.h"




#pragma mark -

DRServerInfoDialog::DRServerInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DRServerInfoDialog)
{
    ui->setupUi(this);

    this->connection = NULL;
}

DRServerInfoDialog::~DRServerInfoDialog()
{
    delete ui;
}




#pragma mark -

void DRServerInfoDialog::close() {
    DRServerInfoDialog::close();

    this->ui->bannerButton->setIcon(QIcon());
    this->ui->descriptionLabel->setText("");
    this->ui->uptimeLabel->setText("");
}





#pragma mark -

void DRServerInfoDialog::setConnection(DRServerConnection *connection) {
    if(connection->connected && connection->server != NULL) {
        if(!connection->serverName.isNull())
            this->setWindowTitle(connection->serverName);

        if(!connection->server->banner.isNull())
            this->ui->bannerButton->setIcon(connection->server->banner);

        if(!connection->server->description.isNull())
            this->ui->descriptionLabel->setText(connection->server->description);

        qDebug() << connection->server->startTime.toString();

        if(!connection->server->startTime.isNull())
            this->ui->uptimeLabel->setText(DR::humanReadableElapsedTimeSinceDate(connection->server->startTime));

        if(connection->url != NULL)
            this->ui->urlLabel->setText(connection->URL());

        if(connection->server->filesCount > 0)
            this->ui->filesLabel->setText(QString::number(connection->server->filesCount));
        else
            this->ui->filesLabel->setText("No files");

        this->ui->sizeLabel->setText(DR::humanReadableStringForSizeInBytes(connection->server->filesSize));

        this->ui->versionLabel->setText(connection->server->versionString());

        this->ui->protocolLabel->setText(QString("%1 %2").arg(
            WSTOQS(wi_p7_socket_remote_protocol_name(connection->p7_socket)),
            WSTOQS(wi_p7_socket_remote_protocol_version(connection->p7_socket))
        ));

        if(WI_P7_ENCRYPTION_ENABLED(wi_p7_socket_options(connection->p7_socket))) {
            this->ui->cipherLabel->setText(QString("%1 %2 bits").arg(
                WSTOQS(wi_cipher_name(wi_p7_socket_cipher(connection->p7_socket))),
                QString::number(wi_cipher_bits(wi_p7_socket_cipher(connection->p7_socket)))
            ));
        } else {
            this->ui->cipherLabel->setText("None");
        }

        if(WI_P7_COMPRESSION_ENABLED(wi_p7_socket_options(connection->p7_socket))) {
            QString compressionString;
            compressionString.sprintf("Yes, compression ratio %0.2f", wi_p7_socket_compression_ratio(connection->p7_socket));
            this->ui->compressionLabel->setText(compressionString);
        } else {
            this->ui->compressionLabel->setText("No");
        }
    }
}
