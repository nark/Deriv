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


#ifndef DRSERVER_H
#define DRSERVER_H

#include <QObject>
#include <QIcon>
#include <QDateTime>
#include <wired/wired.h>
#include "drp7messageobject.h"



class DRServer : public DRP7MessageObject
{
    Q_OBJECT
public:
    QString applicationName;
    QString applicationVersion;
    QString applicationBuild;
    QString osName;
    QString osVersion;
    QString arch;
    QString name;
    QString description;
    QIcon banner;
    bool support_rsrc;
    int downloads;
    int uploads;
    int downloadSpeed;
    int uploadSpeed;
    QDateTime startTime;
    int64_t filesCount;
    int64_t filesSize;

    explicit DRServer(wi_p7_message_t *message);
    void setObjectWithMessage(wi_p7_message_t *message);

    QString versionString();
};

#endif // DRSERVER_H
