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



#include "druser.h"

DRUser::DRUser(QObject *parent) :
    QObject(parent)
{
}


DRUser::DRUser(wi_p7_message_t *message, QObject *parent) :
    QObject(parent)
{
    this->setUserWithMessage(message);
}





#pragma mark -

void DRUser::setUserWithMessage(wi_p7_message_t *message) {
    if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.chat.user_list")) ||
       wi_is_equal(wi_p7_message_name(message), WI_STR("wired.chat.user_join")))
    {
        wi_p7_message_get_int32_for_name(message, &userID, WI_STR("wired.user.id"));
        wi_p7_message_get_bool_for_name(message, &this->isIdle, WI_STR("wired.user.idle"));
        wi_p7_message_get_uint32_for_name(message, &this->color, WI_STR("wired.account.color"));

        this->nick = QString(wi_string_cstring(wi_p7_message_string_for_name(message, WI_STR("wired.user.nick"))));
        this->status = QString(wi_string_cstring(wi_p7_message_string_for_name(message, WI_STR("wired.user.status"))));

        QString base64icon = QString(wi_string_cstring(wi_data_base64(wi_p7_message_data_for_name(message, WI_STR("wired.user.icon")))));
        this->icon = this->iconForBase64String(base64icon);
    }
    else if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.chat.user_status")))
    {
        wi_p7_message_get_bool_for_name(message, &this->isIdle, WI_STR("wired.user.idle"));
        wi_p7_message_get_uint32_for_name(message, &this->color, WI_STR("wired.account.color"));

        this->nick = QString(wi_string_cstring(wi_p7_message_string_for_name(message, WI_STR("wired.user.nick"))));
        this->status = QString(wi_string_cstring(wi_p7_message_string_for_name(message, WI_STR("wired.user.status"))));
    }
}



#pragma mark -

QIcon DRUser::iconForBase64String(QString base64) {
    QByteArray base64Data = base64.toUtf8();

    QImage image;
    image.loadFromData(QByteArray::fromBase64(base64Data));

    return QIcon(QPixmap::fromImage(image));;
}
