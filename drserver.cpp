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


#include <QDebug>
#include "drserver.h"
#include "dr.h"




DRServer::DRServer(wi_p7_message_t *message) : DRP7MessageObject(message)
{
    if(message != NULL)
        this->setObjectWithMessage(message);
}


void DRServer::setObjectWithMessage(wi_p7_message_t *message) {
    wi_p7_uint32_t      uint32var = 0;
    wi_p7_uint64_t      uint64var = 0;
    wi_string_t         *string;
    wi_data_t           *banner;
    wi_date_t           *date;

    if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.server_info"))) {

        string = wi_p7_message_string_for_name(message, WI_STR("wired.info.application.name"));
        this->applicationName = WSTOQS(string);

        string = wi_p7_message_string_for_name(message, WI_STR("wired.info.application.version"));
        this->applicationVersion = WSTOQS(string);

        string = wi_p7_message_string_for_name(message, WI_STR("wired.info.application.build"));
        this->applicationBuild = WSTOQS(string);

        string = wi_p7_message_string_for_name(message, WI_STR("wired.info.os.name"));
        this->osName = WSTOQS(string);

        string = wi_p7_message_string_for_name(message, WI_STR("wired.info.os.version"));
        this->osVersion = WSTOQS(string);

        string = wi_p7_message_string_for_name(message, WI_STR("wired.info.arch"));
        this->arch = WSTOQS(string);

        string = wi_p7_message_string_for_name(message, WI_STR("wired.info.name"));
        this->name = WSTOQS(string);

        string = wi_p7_message_string_for_name(message, WI_STR("wired.info.description"));
        this->description = WSTOQS(string);

        wi_p7_message_get_uint32_for_name(message, &uint32var, WI_STR("wired.info.downloads"));
        this->downloads = (int)uint32var;

        wi_p7_message_get_uint32_for_name(message, &uint32var, WI_STR("wired.info.uploads"));
        this->uploads = (int)uint32var;

        wi_p7_message_get_uint32_for_name(message, &uint32var, WI_STR("wired.info.download_speed"));
        this->downloadSpeed = (int)uint32var;

        wi_p7_message_get_uint32_for_name(message, &uint32var, WI_STR("wired.info.upload_speed"));
        this->uploadSpeed = (int)uint32var;

        date = wi_p7_message_date_for_name(message, WI_STR("wired.info.start_time"));
        this->startTime = DR::wiredDateToQDateTime(date);

        wi_p7_message_get_uint64_for_name(message, &uint64var, WI_STR("wired.info.files.count"));
        this->filesCount = uint64var;

        wi_p7_message_get_uint64_for_name(message, &uint64var, WI_STR("wired.info.files.size"));
        this->filesSize = uint64var;

        banner = wi_p7_message_data_for_name(message, WI_STR("wired.info.banner"));
        this->banner = DR::iconFromWiredData(banner);
    }

}




QString DRServer::versionString() {
    QString version;

    version.sprintf("%s %s (%s) on %s %s (%s)",
                    this->applicationName.toStdString().c_str(),
                    this->applicationVersion.toStdString().c_str(),
                    this->applicationBuild.toStdString().c_str(),
                    this->osName.toStdString().c_str(),
                    this->osVersion.toStdString().c_str(),
                    this->arch.toStdString().c_str());

    return version;
}
