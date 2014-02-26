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
