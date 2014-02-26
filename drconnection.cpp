#include "drconnection.h"
#include "drmainwindow.h"
#include "drchatcontroller.h"
#include "druserscontroller.h"
#include "main.h"
#include <QDebug>
#include <QApplication>

#pragma mark -

DRConnection::DRConnection(wi_url_t *url, QObject *parent) :
    QObject(parent)
{
    this->url = (wi_url_t *)wi_retain(url);

    this->server = NULL;
    this->chatController = NULL;
    this->usersController = NULL;

    this->connected = false;
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

void DRConnection::connect() {
    wi_pool_t           *pool;

    wi_thread_enter_thread();

    pool = wi_pool_init(wi_pool_alloc());

    this->p7_socket = (wi_p7_socket_t *)wi_retain(wc_connect(this->url));

    if(!this->p7_socket) {
        QString errorString = QString("Cannot connect to ") + QString(wi_string_cstring(wi_url_host(this->url)));
        emit connectError(errorString);
        return;
    }

    if(!wc_login(this->p7_socket, this->url)) {
        emit connectError(QString("Could not login"));
        return;
    }

    this->chatController = new DRChatController(this);
    this->usersController = new DRUsersController(this);

    this->sendUserInfo();
    this->joinPublicChat();
    this->connected = true;

    this->moveToThread(QApplication::instance()->thread());
    emit connectSucceeded();
    this->receiveMessagesLoop();

    wi_release(pool);

    wi_thread_exit_thread();
}




void DRConnection::disconnect() {
    this->connected = false;

    wi_p7_socket_close(this->p7_socket);
    wi_socket_close(this->socket);

    if(this->p7_socket != NULL)
        wi_release(this->p7_socket), this->p7_socket = NULL;

    if(this->socket != NULL)
        wi_release(this->socket), this->socket = NULL;

    if(this->server != NULL)
        delete this->server, this->server = NULL;

    if(this->chatController != NULL)
        delete this->chatController, this->chatController = NULL;

    if(this->usersController != NULL)
        delete this->usersController, this->usersController = NULL;
}




void DRConnection::sendMessage(wi_p7_message_t *message) {
    wi_p7_socket_write_message(this->p7_socket, 10, message);
}





#pragma mark -

 void DRConnection::receiveMessagesLoop() {
     while(this->connected == true) {
         wi_p7_message_t *message = wi_p7_socket_read_message(this->p7_socket, 0.0);

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
                qDebug() << "Socket error: " << code;
             }
         }
         QCoreApplication::processEvents();
     }

     if(this->connected == false) {
        emit connectionClosed(this);
     }
}






#pragma mark -

 void DRConnection::sendUserInfo() {
     wi_data_t *icon;
     wi_p7_message_t *message;

     message = wi_p7_message_with_name(WI_STR("wired.user.set_nick"), wc_spec);
     wi_p7_message_set_string_for_name(message, WI_STR("Deriv"), WI_STR("wired.user.nick"));
     this->sendMessage(message);

     message = wi_p7_message_with_name(WI_STR("wired.user.set_status"), wc_spec);
     wi_p7_message_set_string_for_name(message, WI_STR("Qt Client"), WI_STR("wired.user.status"));
     this->sendMessage(message);

     icon    = DRMainWindow::getBase64DefaultUserIcon();
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

wi_p7_socket_t * DRConnection::wc_connect(wi_url_t *url) {
    wi_enumerator_t		*enumerator;
    wi_array_t			*addresses;
    wi_address_t		*address;
    wi_string_t         *password;

    addresses = wi_host_addresses(wi_host_with_string(wi_url_host(url)));

    if(!addresses) {
        return NULL;
    }

    enumerator = wi_array_data_enumerator(addresses);

    while((address = (wi_address_t *)wi_enumerator_next_data(enumerator))) {
        wi_address_set_port(address, wi_url_port(url));

        this->socket = wi_socket_with_address(address, WI_SOCKET_TCP);

        if(!this->socket)
            continue;

        wi_socket_set_interactive(this->socket, true);

        wi_log_info(WI_STR("Connecting to %@:%u..."), wi_address_string(address), wi_address_port(address));

        if(!wi_socket_connect(this->socket, 10.0)) {
            wi_socket_close(this->socket);

            continue;
        }

        wi_log_info(WI_STR("Connected, performing handshake"));

        this->p7_socket = wi_p7_socket_init_with_socket(wi_p7_socket_alloc(), this->socket, wc_spec);

        password = wi_url_password(url);

        if(!password)
            password = WI_STR("");

        if(!wi_p7_socket_connect(this->p7_socket,
                                 10.0,
                                 WI_P7_ENCRYPTION_RSA_AES256_SHA1 | WI_P7_CHECKSUM_SHA1,
                                 WI_P7_BINARY,
                                 wi_url_user(url),
                                 wi_string_sha1(password))) {
            wi_log_error(WI_STR("Could not connect to %@: %m"), wi_address_string(address));

            wi_socket_close(this->socket);

            continue;
        }

        wi_log_info(WI_STR("Connected to P7 server with protocol %@ %@"),
            wi_p7_socket_remote_protocol_name(this->p7_socket), wi_p7_socket_remote_protocol_version(this->p7_socket));

        return this->p7_socket;
    }

    return NULL;
}





wi_boolean_t DRConnection::wc_login(wi_p7_socket_t *socket, wi_url_t *url) {
    wi_p7_message_t		*message;
    wi_string_t         *password, *string;
    wi_data_t           *banner;

    wi_log_info(WI_STR("Performing Wired handshake..."));

    message = wi_p7_message_with_name(WI_STR("wired.client_info"), wc_spec);
    wi_p7_message_set_string_for_name(message, WI_STR("Deriv"), WI_STR("wired.info.application.name"));
    wi_p7_message_set_string_for_name(message, WI_STR("0.1"), WI_STR("wired.info.application.version"));
    wi_p7_message_set_string_for_name(message, WI_STR("a1"), WI_STR("wired.info.application.build"));
    wi_p7_message_set_string_for_name(message, wi_process_os_name(wi_process()), WI_STR("wired.info.os.name"));
    wi_p7_message_set_string_for_name(message, wi_process_os_release(wi_process()), WI_STR("wired.info.os.version"));
    wi_p7_message_set_string_for_name(message, wi_process_os_arch(wi_process()), WI_STR("wired.info.arch"));
    wi_p7_message_set_bool_for_name(message, 0, WI_STR("wired.info.supports_rsrc"));

    message = wc_write_message_and_read_reply(socket, message, NULL);

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

        wi_log_info(WI_STR("Connected to \"%@\""), wi_p7_message_string_for_name(message, WI_STR("wired.info.name")));
    }

    wi_log_info(WI_STR("Logging in as \"%@\"..."), wi_url_user(url));

    password = wi_url_password(url);

    if(!password)
        password = WI_STR("");

    message = wi_p7_message_with_name(WI_STR("wired.send_login"), wc_spec);
    wi_p7_message_set_string_for_name(message, wi_url_user(url), WI_STR("wired.user.login"));
    wi_p7_message_set_string_for_name(message, wi_string_sha1(password), WI_STR("wired.user.password"));

    message = wc_write_message_and_read_reply(socket, message, NULL);

    if(!wi_is_equal(wi_p7_message_name(message), WI_STR("wired.login"))) {
        wi_log_info(WI_STR("Login failed"));

        return false;
    } else {
        wi_p7_message_get_uint32_for_name(message, &this->userID, WI_STR("wired.user.id"));
    }

    message = wi_p7_socket_read_message(socket, 0.0);

    if(!message)
        return false;

    if(!wi_is_equal(wi_p7_message_name(message), WI_STR("wired.account.privileges"))) {
        wi_log_info(WI_STR("Login failed"));

        return false;
    }

    return true;
}





wi_p7_message_t * DRConnection::wc_write_message_and_read_reply(wi_p7_socket_t *socket, wi_p7_message_t *message, wi_string_t *expected_error) {
    wi_string_t		*name, *error;

    if(!wi_p7_socket_write_message(socket, 0.0, message))
        wi_log_fatal(WI_STR("Could not write message: %m"));

    message = wi_p7_socket_read_message(socket, 0.0);

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
