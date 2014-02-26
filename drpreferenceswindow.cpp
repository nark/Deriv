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
}



DRPreferencesWindow::~DRPreferencesWindow()
{
    delete ui;
}




#pragma mark -

void DRPreferencesWindow::on_lineEdit_editingFinished()
{
   //QSettings
}
