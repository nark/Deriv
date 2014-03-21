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


#include <QApplication>
#include <QDebug>
#include <QResource>
#include <QFile>
#include <QMessageBox>

#include "main.h"
#include "dr.h"
#include "drerror.h"
#include "drconnectdialog.h"
#include "drmainwindow.h"
#include "drpreferenceswindow.h"




wi_p7_spec_t *wc_spec;





int main(int argc, char *argv[])
{
    wi_pool_t *pool;
    DRError *error = NULL;

    // init Qt app
    QApplication a(argc, argv);

    // set internal app settings
    QCoreApplication::setOrganizationName("Read-Write.fr");
    QCoreApplication::setOrganizationDomain("read-write.fr");
    QCoreApplication::setApplicationName("Deriv");

    // initialize libwired base
    wi_initialize();
    // set program arguments to libwired (unused here)
    wi_load(argc, (const char **)argv);

    // configure libwired logging
    wi_log_tool = true;
    wi_log_level = WI_LOG_INFO;

    // init libwired global memory pool
    pool = wi_pool_init(wi_pool_alloc());

    // load Wired 2.0 specification
    DR::loadSpecification(&wc_spec, &error);

    // if specification loaded
    if(error == NULL) {
        // display the main window
        DRMainWindow::instance()->show();

        // display the connect dialog if DRShowConnectAtStartup user setting is true
        if(DRPreferencesWindow::instance()->settings->value(DRShowConnectAtStartup).toBool())
            DRMainWindow::instance()->on_actionNewConnection_triggered();

    // if specification load error
    } else {
        // display the error
        // NOTE: maybe an event loop is needed here ?
        DR::showError(error);

        // release the global libwired pool
        wi_release(pool);

        // terminate the app
        return error->errorCode();
    }

    // start the app event loop
    int result = a.exec();

    // release the global libwired pool
    wi_release(pool);

    // terminate the app
    return result;
}


