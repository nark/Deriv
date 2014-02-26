#include "drchatcontroller.h"
#include "druserscontroller.h"



#pragma mark -

DRChatController::DRChatController(DRConnection *connection) :
    DRConnectionController(connection)
{
    this->topic = NULL;

    QObject::connect(this->connection,  SIGNAL(receivedMessage(wi_p7_message_t*)),
                     this,              SLOT(receivedMessage(wi_p7_message_t*)));
}


DRChatController::~DRChatController() {
    if(this->topic != NULL)
        delete this->topic, this->topic = NULL;
}






#pragma mark -

void DRChatController::receivedMessage(wi_p7_message_t *message) {
    if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.chat.say"))) {
        this->receivedWiredChatSay(message);
    }
    else if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.chat.me"))) {
        this->receivedWiredChatMe(message);
    }
    else if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.chat.topic"))) {
        this->receivedWiredChatTopic(message);
    }
    else if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.chat.user_status"))) {
        this->receivedWiredUserStatus(message);
    }
}






#pragma mark -

void DRChatController::receivedWiredChatSay(wi_p7_message_t *message) {
    DRUser                  *user;
    wi_p7_int32_t           userID;

    wi_p7_message_get_int32_for_name(message, &userID, WI_STR("wired.user.id"));

    if(userID <= 0)
        return;

    user = this->connection->usersController->userWithID(userID);

    if(user == NULL)
        return;

    wi_string_t *string = wi_p7_message_string_for_name(message, WI_STR("wired.chat.say"));

    if(!string || wi_string_length(string) <= 0)
        return;

    string = wi_string_with_format(WI_STR("%@ : %@"), user->nick, string);

    emit chatControllerReceivedChatSay(this->connection, QString(wi_string_cstring(string)), user);
}




void DRChatController::receivedWiredChatMe(wi_p7_message_t *message) {
    DRUser                  *user;
    wi_p7_int32_t           userID;

    wi_p7_message_get_int32_for_name(message, &userID, WI_STR("wired.user.id"));

    if(userID <= 0)
        return;

    user = this->connection->usersController->userWithID(userID);

    if(user == NULL)
        return;

    wi_string_t *string = wi_p7_message_string_for_name(message, WI_STR("wired.chat.say"));

    if(!string || wi_string_length(string) <= 0)
        return;

    string = wi_string_with_format(WI_STR("*** %@ %@"), user->nick, string);

    emit chatControllerReceivedChatMe(this->connection, QString(wi_string_cstring(string)), user);
}




void DRChatController::receivedWiredChatTopic(wi_p7_message_t *message) {
    if(this->topic == NULL)
        this->topic = new DRTopic(message);
    else
        this->topic->setTopicWithMessage(message);

    emit chatControllerTopicChanged(this->connection, this->topic);
}




void DRChatController::receivedWiredUserStatus(wi_p7_message_t *message) {

}


