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
