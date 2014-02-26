#include <QMutex>
#include "draboutwindow.h"
#include "ui_draboutwindow.h"


static DRAboutWindow          *m_Instance = 0;



#pragma mark -

DRAboutWindow*  DRAboutWindow::instance(QWidget* parent) {
    static QMutex mutex;
    if (!m_Instance) {
        mutex.lock();

        if (!m_Instance)
            m_Instance = new DRAboutWindow(parent);

        mutex.unlock();
    }
    return m_Instance;
}




void DRAboutWindow::drop() {
    static QMutex mutex;
    mutex.lock();
    delete m_Instance;
    m_Instance = 0;
    mutex.unlock();
}






#pragma mark -

DRAboutWindow::DRAboutWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DRAboutWindow)
{
    ui->setupUi(this);
}

DRAboutWindow::~DRAboutWindow()
{
    delete ui;
}
