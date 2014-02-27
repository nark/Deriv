#ifndef DRP7MESSAGEOBJECT_H
#define DRP7MESSAGEOBJECT_H

#include <QObject>
#include <wired/wired.h>


class DRP7MessageObject : public QObject
{
    Q_OBJECT
public:
    explicit DRP7MessageObject(wi_p7_message_t *message, QObject *parent = 0);
    ~DRP7MessageObject();
};

#endif // DRP7MESSAGEOBJECT_H
