/*
   Copyright 2014, Rafaël Warnault <rw@read-write.fr>

   Clementine is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Clementine is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Clementine.  If not, see <http://www.gnu.org/licenses/>.
*/



#ifndef DRSERVER_H
#define DRSERVER_H

#include <QObject>
#include <wired/wired.h>



class DRServer : public QObject
{
    Q_OBJECT
public:
    wi_string_t *applicationName;
    wi_string_t *applicationVersion;
    wi_string_t *applicationBuild;
    wi_string_t *osName;
    wi_string_t *osVersion;
    wi_string_t *arch;
    wi_boolean_t support_rsrc;

    wi_string_t *name;
    wi_string_t *description;
    wi_data_t   *banner;

    explicit DRServer(QObject *parent = 0);

signals:

public slots:

};

#endif // DRSERVER_H
