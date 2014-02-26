#include "main.h"
#include "drconnectdialog.h"
#include "drmainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QResource>
#include <QFile>
#include <QMessageBox>


wi_p7_spec_t				*wc_spec;



static wi_boolean_t         load_spec();



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Read-Write.fr");
    QCoreApplication::setOrganizationDomain("read-write.fr");
    QCoreApplication::setApplicationName("Deriv");

    wi_pool_t				*pool;

    wi_initialize();
    wi_load(argc, (const char **)argv);

    wi_log_tool 	= true;
    wi_log_level 	= WI_LOG_INFO;

    pool = wi_pool_init(wi_pool_alloc());

    if(load_spec() == false)
        return -1;

    DRMainWindow::instance()->show();
    DRMainWindow::instance()->on_actionNewConnection_triggered();

    int result = a.exec();

    wi_release(pool);
    return result;
}





static wi_boolean_t load_spec() {
    QResource common("/wired.xml");

    QFile commonFile(common.absoluteFilePath());

    qDebug() << common.absoluteFilePath();

    if (!commonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Unable to open file: " << commonFile.fileName() << " besause of error " << commonFile.errorString() << endl;
        return false;
    }

    QTextStream in(&commonFile);
    QString content = in.readAll();

    wc_spec = wi_p7_spec_init_with_string(wi_p7_spec_alloc(), WI_STR(content.toStdString().c_str()), WI_P7_CLIENT);

    if(!wc_spec) {
        wi_log_fatal(WI_STR("Could not open wired.xml: %m"));
        return false;
    }

    wi_p7_spec_message_t *message = wi_p7_spec_message_with_name(wc_spec, WI_STR("wired.user.info"));
    if(!message) {
        wi_log_fatal(WI_STR("Invalid spec, could not create message"));
        return false;
    }

    return true;
}


