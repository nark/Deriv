#ifndef DRTOPIC_H
#define DRTOPIC_H

#include <QObject>
#include <QDateTime>
#include <wired/wired.h>

class DRTopic : public QObject
{
    Q_OBJECT
public:
    QString             *topic;
    QString             *nick;
    QDateTime           *time;

    explicit            DRTopic(wi_p7_message_t *message, QObject *parent = 0);

    void                setTopicWithMessage(wi_p7_message_t *message);
    QString             qString();
};

#endif // DRTOPIC_H
