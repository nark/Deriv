#ifndef DRUSER_H
#define DRUSER_H

#include <QObject>
#include <wired/wired.h>

class DRUser : public QObject
{
    Q_OBJECT
public:
    wi_string_t             *nick;
    wi_string_t             *status;
    wi_data_t               *icon;

    wi_p7_uint32_t          userID;
    wi_boolean_t            isIdle;
    wi_p7_uint32_t          color;

    explicit DRUser(QObject *parent = 0);

signals:

public slots:

};

#endif // DRUSER_H
