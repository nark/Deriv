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
#include <QSettings>
#include <QDebug>

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

    this->setDefault();
}




DRPreferencesWindow::~DRPreferencesWindow()
{
    delete ui;
    delete settings, settings = NULL;
}






#pragma mark -

void DRPreferencesWindow::show() {
    this->loadSettings();

    QMainWindow::show();
}







#pragma mark -

void DRPreferencesWindow::setDefault() {
    if(!this->settings->contains(DRDefaultNick))
        this->settings->setValue(DRDefaultNick, QString("Deriv"));

    if(!this->settings->contains(DRDefaultStatus))
        this->settings->setValue(DRDefaultStatus, QString("Qt Client"));

    if(!this->settings->contains(DRShowConnectAtStartup))
        this->settings->setValue(DRShowConnectAtStartup, true);
}



void DRPreferencesWindow::loadSettings() {
    ui->nickField->setText(this->settings->value(DRDefaultNick).toString());
    ui->statusField->setText(this->settings->value(DRDefaultStatus).toString());
    ui->showConnectAtStartupCheckBox->setChecked(this->settings->value(DRShowConnectAtStartup).toBool());
}





#pragma mark -

void DRPreferencesWindow::on_nickField_editingFinished() {
    QString nick = ui->nickField->text();

    // check if changed
    if(nick.length() <= 0 || nick == this->settings->value(DRDefaultNick))
        return;

    // set and save settings
    this->settings->setValue(DRDefaultNick, nick);
    this->settings->sync();

    // broadcast (to connections controller)
    emit userNickDidChange(nick);
}





void DRPreferencesWindow::on_statusField_editingFinished() {
    QString status = ui->statusField->text();

    // check if changed
    if(status.length() <= 0 || status == this->settings->value(DRDefaultStatus))
        return;

    // set and save settings
    this->settings->setValue(DRDefaultStatus, status);
    this->settings->sync();

    // broadcast (to connections controller)
    emit userStatusDidChange(status);
}




void DRPreferencesWindow::on_showConnectAtStartupCheckBox_clicked() {
    // set and save settings
    this->settings->setValue(DRShowConnectAtStartup, ui->showConnectAtStartupCheckBox->isChecked());
    this->settings->sync();
}
