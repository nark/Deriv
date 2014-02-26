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

    //mainWindow->setConnection(this->connection);
    mainWindow->show();

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


