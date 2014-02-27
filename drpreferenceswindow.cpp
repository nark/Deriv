#include <QMutex>
#include <QSettings>
#include "drpreferenceswindow.h"
#include "ui_drpreferenceswindow.h"



static DRPreferencesWindow          *m_Instance = 0;



#pragma mark -

DRPreferencesWindow*  DRPreferencesWindow::instance(QWidget* parent) {
    static QMutex mutex;
    if (!m_Instance) {
        mutex.lock();

        if (!m_Instance)
            m_Instance = new DRPreferencesWindow(parent);

        mutex.unlock();
    }
    return m_Instance;
}




void DRPreferencesWindow::drop() {
    static QMutex mutex;
    mutex.lock();
    delete m_Instance;
    m_Instance = 0;
    mutex.unlock();
}





#pragma mark -

DRPreferencesWindow::DRPreferencesWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DRPreferencesWindow)
{
    ui->setupUi(this);

    this->settings = new QSettings(QString("read-write.fr"), QString("Deriv"));

    if(this->settings->contains("DRDefaultNick"))
        this->settings->setValue("DRDefaultNick", QString("Deriv"));

    this->settings->setValue("DRDefaultStatus", QString("Qt Client"));
    this->settings->setValue("DRShowConnectAtStartup", true);
}




DRPreferencesWindow::~DRPreferencesWindow()
{
    delete ui;
    delete settings, settings = NULL;
}



#pragma mark -

void DRPreferencesWindow::show() {
    ui->nickLabel->setText(this->settings->value("DRDefaultNick").toString());

    QMainWindow::show();
}



#pragma mark -

void DRPreferencesWindow::on_nickLabel_editingFinished() {
    QString nick = ui->nickLabel->text();

    if(nick.length() > 0)
        this->settings->setValue("DRDefaultNick", nick);

    this->settings->sync();
}



