#include "drtopic.h"

DRTopic::DRTopic(wi_p7_message_t *message, QObject *parent) :
    QObject(parent)
{
    this->topic = NULL;
    this->nick = NULL;
    this->time = NULL;

    if(message != NULL)
        this->setTopicWithMessage(message);
}



void DRTopic::setTopicWithMessage(wi_p7_message_t *message) {
    this->topic = new QString(wi_string_cstring(wi_p7_message_string_for_name(message, WI_STR("wired.chat.topic.topic"))));
    this->nick = new QString(wi_string_cstring(wi_p7_message_string_for_name(message, WI_STR("wired.user.nick"))));

    wi_date_t *date = wi_p7_message_date_for_name(message, WI_STR("wired.chat.topic.time"));
    this->time = new QDateTime(QDateTime::fromString(QString(wi_string_cstring(wi_date_rfc3339_string(date)))));
}


QString DRTopic::qString() {
    if (this->topic == NULL || this->topic->length() <= 0)
        return QString("");

//    wi_string_t *string = wi_string_with_format(WI_STR("%@ by %@ on %@"), this->topic, this->nick, wi_date_sqlite3_string(this->time));

//    return QString(wi_string_cstring(string));
}
