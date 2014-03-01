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



#ifndef DRUSER_H
#define DRUSER_H

#include <QObject>
#include <QIcon>
#include <wired/wired.h>

class DRUser : public QObject
{
    Q_OBJECT
public:    
    QString nick;
    QString status;
    QIcon icon;

    wi_p7_int32_t userID;
    wi_boolean_t isIdle;
    wi_p7_uint32_t color;

    explicit DRUser(QObject *parent = 0);
    explicit DRUser(wi_p7_message_t *message, QObject *parent = 0);

signals:

public slots:
    void setUserWithMessage(wi_p7_message_t *message);


private:
    QIcon iconForBase64String(QString base64);

};

#endif // DRUSER_H
