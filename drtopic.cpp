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



#include "drtopic.h"
#include "dr.h"


DRTopic::DRTopic(wi_p7_message_t *message) : DRP7MessageObject(message)
{
    if(message != NULL)
        this->setObjectWithMessage(message);
}



void DRTopic::setObjectWithMessage(wi_p7_message_t *message) {
    this->topic = QString(wi_string_cstring(wi_p7_message_string_for_name(message, WI_STR("wired.chat.topic.topic"))));
    this->nick = QString(wi_string_cstring(wi_p7_message_string_for_name(message, WI_STR("wired.user.nick"))));

    wi_date_t *date = wi_p7_message_date_for_name(message, WI_STR("wired.chat.topic.time"));
    this->time = DR::wiredDateToQDateTime(date);
}




QString DRTopic::formattedTopic() {
    if (this->topic.isNull() || this->topic.isEmpty())
        return QString("");

    return this->topic;
}

QString DRTopic::formattedTopicByUser() {
    if (this->topic.isNull() || this->topic.isEmpty())
        return QString("");

    return QString("%1 by %2").arg(this->topic, this->nick);
}

QString DRTopic::formattedTopicByUserWithDate() {
    if (this->topic.isNull() || this->topic.isEmpty())
        return QString("");

    return QString("%1 by %2 - %3").arg(this->topic, this->nick, this->time.toString());
}
