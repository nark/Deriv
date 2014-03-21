#include "drthreaditem.h"
#include "drboarditem.h"
#include "dr.h"

DRThreadItem::DRThreadItem(wi_p7_message_t *message, DRBoardItem *board, DRServerConnection *connection):
    DRBoardNodeItem(connection, board)
{
    this->connection = connection;
    this->board = board;

    if(message != NULL) {
        wi_string_t *string;

        string = wi_p7_message_string_for_name(message, WI_STR("wired.board.thread"));
        this->threadID = WSTOQS(string);

        string = wi_p7_message_string_for_name(message, WI_STR("wired.board.subject"));
        this->subject = WSTOQS(string);

        string = wi_p7_message_string_for_name(message, WI_STR("wired.user.nick"));
        this->nick = WSTOQS(string);
    }
}
