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
#include <QApplication>
#include <QMutex>
#include "drconnection.h"
#include "drmainwindow.h"
#include "drchatcontroller.h"
#include "druserscontroller.h"
#include "drpreferenceswindow.h"
#include "main.h"
#include "dr.h"



static QMutex readMutex;



#pragma mark -

DRConnection::DRConnection(wi_url_t *url, QObject *parent) :
    QObject(parent)
{
    this->url = (wi_url_t *)wi_retain(url);

    this->server = NULL;
    this->serverName = (const char *)0;
    this->chatController = NULL;
    this->usersController = NULL;

    this->connected = false;
    this->connecting = false;
    this->reconnecting = false;

    this->autoConnect = false;
    this->autoReconnect = false;
}


DRConnection::~DRConnection() {
    wi_release(this->url), this->url = NULL;

    if(this->server != NULL)
        delete this->server, this->server = NULL;

    if(this->chatController != NULL)
        delete this->chatController, this->chatController = NULL;

    if(this->usersController != NULL)
        delete this->usersController, this->usersController = NULL;
}






#pragma mark -

QString DRConnection::URLIdentifier() {
    QString identifier = "";

    identifier += QString(wi_string_cstring(wi_url_user(this->url)));
    identifier += QString("@");
    identifier += QString(wi_string_cstring(wi_url_host(this->url)));
    identifier += QString(":");
    identifier += QString(wi_string_cstring(wi_string_with_format(WI_STR("%d"), wi_url_port(this->url))));

    return identifier;
}


QString DRConnection::URLPassword() {
    wi_string_t     *string = NULL;
    QString         password = "";

    string = wi_url_password(this->url);

    if(string != NULL)
        password += QString(wi_string_cstring(string));

    return password;
}









#pragma mark -

void DRConnection::connect(QObject *receiver) {
    wi_mutable_url_t *mutable_url;
    QThread* thread = new QThread();

    mutable_url = (wi_mutable_url_t *)wi_mutable_copy(this->url);

    this->connecting = true;

    // enforce default Wired 2.0 port (4871)
    if(!wi_url_port(this->url))
        wi_mutable_url_set_port(mutable_url, 4871);

    // enforce default user
    if(!wi_url_user(this->url))
        wi_mutable_url_set_user(mutable_url, WI_STR("guest"));

    // enforce password from keychain
    if(!wi_url_password(this->url)) {
        QString password = DR::loadPasswordForULRIdentifier(this->URLIdentifier());
        if(password.length() > 0) {
            wi_mutable_url_set_password(mutable_url, wi_string_with_cstring(password.toStdString().c_str()));
        }
    }

    this->url = mutable_url;

    // move the connection object to a background thread
    this->moveToThread(thread);

    // connect the started() thread signal to the connection thread slot
    QObject::connect(thread, SIGNAL(started()), this, SLOT(connectThread()));

    // connect receiver signals
    QObject::disconnect(this, SIGNAL(connectionSucceeded(DRConnection*)), receiver, SLOT(connectionSucceeded(DRConnection*)));
    QObject::disconnect(this, SIGNAL(connectionError(DRConnection*,DRError*)), receiver, SLOT(connectionError(DRConnection*,DRError*)));
    QObject::disconnect(this, SIGNAL(connectionClosed(DRConnection*,DRError*)), receiver, SLOT(connectionClosed(DRConnection*,DRError*)));

    QObject::connect(this, SIGNAL(connectionSucceeded(DRConnection*)), receiver, SLOT(connectionSucceeded(DRConnection*)));
    QObject::connect(this, SIGNAL(connectionError(DRConnection*,DRError*)), receiver, SLOT(connectionError(DRConnection*,DRError*)));
    QObject::connect(this, SIGNAL(connectionClosed(DRConnection*,DRError*)), receiver, SLOT(connectionClosed(DRConnection*,DRError*)));

    // start connection in background
    thread->start();

    // to test on the main thread
    //this->connectThread();
}




void DRConnection::disconnect() {
    this->connected = false;

    readMutex.lock();

    if(this->p7_socket)
        wi_p7_socket_close(this->p7_socket);

    if(this->socket)
        wi_socket_close(this->socket);

    if(this->p7_socket != NULL)
        wi_release(this->p7_socket), this->p7_socket = NULL;

    // TODO: check if this is a normal behavior
    if(this->socket != NULL)
        wi_release(this->socket), this->socket = NULL;

    if(this->server != NULL)
        delete this->server, this->server = NULL;

    if(this->chatController != NULL)
        delete this->chatController, this->chatController = NULL;

    if(this->usersController != NULL)
        delete this->usersController, this->usersController = NULL;

    readMutex.unlock();
}





void DRConnection::sendMessage(wi_p7_message_t *message) {
    wi_boolean_t result = wi_p7_socket_write_message(this->p7_socket, 10, message);

    if(!result) {
        DRError *error = this->errorWithCode(WISocketWriteFailed);

        emit connectionError(this, error);
    }
}







#pragma mark -

 void DRConnection::receiveMessagesLoop(DRError **error) {
     int state;

     while(this->connected == true) {
         wi_p7_message_t *message = NULL;

         do {
             state = wi_socket_wait(this->socket, 0.1);
         } while((state != WI_SOCKET_READY) && this->connected);

         if(this->connected == false)
             break;

         readMutex.lock();
         message = wi_p7_socket_read_message(this->p7_socket, 0.0);
         readMutex.unlock();

         if(message != NULL) {
            if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.send_ping"))) {
                 wi_p7_message_t *wd_ping_message = wi_p7_message_with_name(WI_STR("wired.ping"), wc_spec);
                 wi_p7_socket_write_message(this->p7_socket, 10, wd_ping_message);

            }
            else if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.ping"))) {

            }
            else if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.error"))) {
                 emit receivedError(message);

            }
            else {
                 emit receivedMessage(message);
            }
         } else {
             if(wi_error_domain() == WI_ERROR_DOMAIN_ERRNO && wi_error_code() == ETIMEDOUT) {
                int code = wi_socket_error(this->socket);
                qDebug() << "Socket error: ETIMEDOUT error : " << code;
             }
             *error = this->errorWithCode(WISocketReadFailed);

             this->connected == false;
             break;
        }
        QCoreApplication::processEvents();
     }

     qDebug() << "end message loop";
}







void DRConnection::connectThread() {
    wi_pool_t           *pool;
    DRError             *error = NULL;

    // make libwired enter the connection thread
    wi_thread_enter_thread();

    // init a libwired memeory pool for this thread
    pool = wi_pool_init(wi_pool_alloc());

    // try to connect
    this->p7_socket = (wi_p7_socket_t *)wi_retain(this->connectSocket(&error));

    // check connection OK
    if(!this->p7_socket) {
        this->connecting = false;

        QString errorString = QString("Cannot connect to ") + QString(wi_string_cstring(wi_url_host(this->url)));
        // if error, back to main thread
        this->moveToThread(QApplication::instance()->thread());
        // notify receiver
        emit connectionError(this, error);
        return;
    }

    // try to login
    if(!this->login(&error)) {
        this->connecting = false;
        // if error, back to main thread
        this->moveToThread(QApplication::instance()->thread());
        // notify receiver
        emit connectionError(this, error);
        return;
    }

    // init sub controllers
    this->chatController = new DRChatController(this);
    this->usersController = new DRUsersController(this);

    // send user infos
    this->sendUserInfo();
    // join public chat
    this->joinPublicChat();

    // we are now connected
    this->connecting = false;
    this->connected = true;

    // move back the connection to the main thread
    this->moveToThread(QApplication::instance()->thread());

    // notify connection succeeded
    emit connectionSucceeded(this);

    // start the receive message loop
    this->receiveMessagesLoop(&error);

    // clean the connection
    this->disconnect();

    // notify connection is closed
    emit connectionClosed(this, error);

    // release the curent libwired pool
    wi_release(pool);

    // make libwired exit this thread
    wi_thread_exit_thread();
}






#pragma mark -

 void DRConnection::sendUserInfo() {
     wi_data_t *icon;
     wi_p7_message_t *message;
     QString string;

     string = DRPreferencesWindow::instance()->settings->value(DRDefaultNick).toString();
     message = wi_p7_message_with_name(WI_STR("wired.user.set_nick"), wc_spec);
     wi_p7_message_set_string_for_name(message, WI_STR(string.toStdString().c_str()), WI_STR("wired.user.nick"));
     this->sendMessage(message);

     string = DRPreferencesWindow::instance()->settings->value(DRDefaultStatus).toString();
     message = wi_p7_message_with_name(WI_STR("wired.user.set_status"), wc_spec);
     wi_p7_message_set_string_for_name(message, WI_STR(string.toStdString().c_str()), WI_STR("wired.user.status"));
     this->sendMessage(message);

     icon = DR::getDefaultBase64UserIcon();
     message = wi_p7_message_with_name(WI_STR("wired.user.set_icon"), wc_spec);
     wi_p7_message_set_data_for_name(message, icon, WI_STR("wired.user.icon"));
     this->sendMessage(message);
 }



 void DRConnection::joinPublicChat() {
     wi_p7_message_t *message;

     message = wi_p7_message_with_name(WI_STR("wired.chat.join_chat"), wc_spec);
     wi_p7_message_set_uint32_for_name(message, 1, WI_STR("wired.chat.id"));

     this->sendMessage(message);
 }






#pragma mark -

wi_p7_socket_t * DRConnection::connectSocket(DRError **error) {
    wi_enumerator_t		*enumerator;
    wi_array_t			*addresses;
    wi_address_t		*address;
    wi_string_t         *password;

    addresses = wi_host_addresses(wi_host_with_string(wi_url_host(this->url)));

    if(addresses) {
        enumerator = wi_array_data_enumerator(addresses);

        while((address = (wi_address_t *)wi_enumerator_next_data(enumerator))) {
            wi_address_set_port(address, wi_url_port(url));

            this->socket = (wi_socket_t *)wi_retain(wi_socket_with_address(address, WI_SOCKET_TCP));

            if(!this->socket) {
                *error = this->errorWithCode(WISocketConnectFailed);

                continue;
            }

            wi_socket_set_interactive(this->socket, true);

            wi_log_debug(WI_STR("Connecting to %@:%u..."), wi_address_string(address), wi_address_port(address));

            if(!wi_socket_connect(this->socket, 10.0)) {
                wi_socket_close(this->socket);

                *error = this->errorWithCode(WISocketConnectFailed);

                continue;
            }

            wi_log_debug(WI_STR("Connected, performing handshake"));

            this->p7_socket = wi_p7_socket_init_with_socket(wi_p7_socket_alloc(), this->socket, wc_spec);

            password = wi_url_password(this->url);

            if(!password)
                password = WI_STR("");

            if(!wi_p7_socket_connect(this->p7_socket,
                                     10.0,
                                     WI_P7_ENCRYPTION_RSA_AES256_SHA1 | WI_P7_CHECKSUM_SHA1,
                                     WI_P7_BINARY,
                                     wi_url_user(this->url),
                                     wi_string_sha1(password))) {
                wi_log_error(WI_STR("Could not connect to %@: %m"), wi_address_string(address));

                wi_socket_close(this->socket);

                *error = this->errorWithCode(WISocketConnectFailed, QString(wi_string_cstring(wi_error_string())));

                continue;
            }

            wi_log_debug(WI_STR("Connected to P7 server with protocol %@ %@"),
                wi_p7_socket_remote_protocol_name(this->p7_socket), wi_p7_socket_remote_protocol_version(this->p7_socket));

            return this->p7_socket;
        }
    }
    else {
        *error = this->errorWithCode(WIAddressLookupFailed);
    }

    return NULL;
}





wi_boolean_t DRConnection::login(DRError **error) {
    wi_p7_message_t		*message;
    wi_string_t         *password, *string;
    wi_data_t           *banner;

    wi_log_debug(WI_STR("Performing Wired handshake..."));

    message = wi_p7_message_with_name(WI_STR("wired.client_info"), wc_spec);
    wi_p7_message_set_string_for_name(message, WI_STR("Deriv"), WI_STR("wired.info.application.name"));
    wi_p7_message_set_string_for_name(message, WI_STR("0.1"), WI_STR("wired.info.application.version"));
    wi_p7_message_set_string_for_name(message, WI_STR("a1"), WI_STR("wired.info.application.build"));
    wi_p7_message_set_string_for_name(message, wi_process_os_name(wi_process()), WI_STR("wired.info.os.name"));
    wi_p7_message_set_string_for_name(message, wi_process_os_release(wi_process()), WI_STR("wired.info.os.version"));
    wi_p7_message_set_string_for_name(message, wi_process_os_arch(wi_process()), WI_STR("wired.info.arch"));
    wi_p7_message_set_bool_for_name(message, 0, WI_STR("wired.info.supports_rsrc"));

    message = this->writeMessageAndReadReply(message, NULL);

    if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.server_info"))) {
        if(this->server == NULL)
             this->server = new DRServer();

        string = wi_p7_message_string_for_name(message, WI_STR("wired.info.application.name"));
        this->server->applicationName = string;

        string = wi_p7_message_string_for_name(message, WI_STR("wired.info.application.version"));
        this->server->applicationVersion = string;

        string = wi_p7_message_string_for_name(message, WI_STR("wired.info.application.build"));
        this->server->applicationBuild = string;

        string = wi_p7_message_string_for_name(message, WI_STR("wired.info.os.name"));
        this->server->osName = string;

        string = wi_p7_message_string_for_name(message, WI_STR("wired.info.os.version"));
        this->server->osVersion = string;

        string = wi_p7_message_string_for_name(message, WI_STR("wired.info.arch"));
        this->server->arch = string;

        string = wi_p7_message_string_for_name(message, WI_STR("wired.info.name"));
        this->server->name = string;

        string = wi_p7_message_string_for_name(message, WI_STR("wired.info.description"));
        this->server->description = string;

        banner = wi_p7_message_data_for_name(message, WI_STR("wired.info.banner"));
        this->server->banner = banner;

        wi_log_debug(WI_STR("Connected to \"%@\""), wi_p7_message_string_for_name(message, WI_STR("wired.info.name")));

        this->serverName = QString(wi_string_cstring(this->server->name));
    }

    wi_log_debug(WI_STR("Logging in as \"%@\"..."), wi_url_user(this->url));

    password = wi_url_password(this->url);

    if(!password)
        password = WI_STR("");

    message = wi_p7_message_with_name(WI_STR("wired.send_login"), wc_spec);
    wi_p7_message_set_string_for_name(message, wi_url_user(url), WI_STR("wired.user.login"));
    wi_p7_message_set_string_for_name(message, wi_string_sha1(password), WI_STR("wired.user.password"));

    message = this->writeMessageAndReadReply(message, NULL);

    if(!wi_is_equal(wi_p7_message_name(message), WI_STR("wired.login"))) {
        wi_log_error(WI_STR("Login failed: %m"));

        if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.error"))) {
            *error = new DRError(message);
        }

        return false;
    } else {
        wi_p7_message_get_uint32_for_name(message, &this->userID, WI_STR("wired.user.id"));
    }

    message = wi_p7_socket_read_message(this->p7_socket, 0.0);

    if(!message)
        return false;

    if(!wi_is_equal(wi_p7_message_name(message), WI_STR("wired.account.privileges"))) {
        wi_log_error(WI_STR("Login failed: %m"));

        if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.error"))) {
            *error = new DRError(message);
        }

        return false;
    }

    return true;
}





wi_p7_message_t * DRConnection::writeMessageAndReadReply(wi_p7_message_t *message, wi_string_t *expected_error) {
    wi_string_t		*name, *error;

    if(!wi_p7_socket_write_message(this->p7_socket, 0.0, message))
        wi_log_fatal(WI_STR("Could not write message: %m"));

    message = wi_p7_socket_read_message(this->p7_socket, 0.0);

    if(!message)
        wi_log_fatal(WI_STR("Could not read message: %m"));

    name = wi_p7_message_name(message);

    if(wi_is_equal(name, WI_STR("wired.error"))) {
        error = wi_p7_message_enum_name_for_name(message, WI_STR("wired.error"));

        if(expected_error) {
            if(!wi_is_equal(error, expected_error))
               wi_log_fatal(WI_STR("Unexpected error %@"), error);
        } else {
            wi_log_fatal(WI_STR("Unexpected error %@"), error);
        }
    }

    return message;
}




DRError* DRConnection::errorWithCode(int code, QString info) {
    QHash<DRErrorKeys, QString> params;

    params[DRErrorDescriptionKey] = this->URLIdentifier();

    if(!info.isNull())
        params[DRErrorReasonKey] = info;

    return new DRError(code, params, DR_ERROR_DOMAIN_WIRED);
}
