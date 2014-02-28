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
