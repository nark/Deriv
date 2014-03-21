#include <QDebug>
#include <QFileInfo>
#include <QApplication>
#include <QMutex>
#include "drboardscontroller.h"
#include "drboardstreemodel.h"
#include "drthreaditem.h"
#include "main.h"
#include "dr.h"



static QMutex mutex;


DRBoardsController::DRBoardsController(DRServerConnection* connection) :
    DRConnectionController(connection)
{
    this->boardsModel = new DRBoardsTreeModel(connection);
    this->threadsModel = new QStandardItemModel();

    this->connection->addDelegateForMessage(this, "wired.board.board_list");
    this->connection->addDelegateForMessage(this, "wired.board.thread_list");

    this->getBoards();
}




void DRBoardsController::connectReceiver(QObject *object) {

}


void DRBoardsController::disconnectReceiver(QObject *object) {

}






void DRBoardsController::getBoards() {
    this->boardsModel->clearBoards();

    // reload boards
    wi_p7_message_t *message = wi_p7_message_with_name(WI_STR("wired.board.get_boards"), wc_spec);
    this->connection->sendMessage(message);
}




void DRBoardsController::getThreads(DRBoardItem *board) {
    board->clearThreads();

    // reload thread
    wi_p7_message_t *message = wi_p7_message_with_name(WI_STR("wired.board.get_threads"), wc_spec);
    wi_p7_message_set_string_for_name(message, QSTOWS(board->path()), WI_STR("wired.board.board"));
    this->connection->sendMessage(message);
}




void DRBoardsController::connectionSucceeded(DRServerConnection *connection) {

}

void DRBoardsController::connectionError(DRServerConnection *connection, DRError *error) {

}

void DRBoardsController::connectionClosed(DRServerConnection *connection, DRError *error) {

}



void DRBoardsController::receivedMessage(wi_p7_message_t *message, DRServerConnection *connection) {

    if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.board.board_list"))) {
        this->receivedBoard(message);
    }
    else if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.board.board_list.done"))) {
        // signal to retore expanded state
    }
    else if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.board.thread_list"))) {
        this->receivedThread(message);
    }
    else if(wi_is_equal(wi_p7_message_name(message), WI_STR("wired.board.thread_list.done"))) {

    }
}



void DRBoardsController::receivedError(DRError *error, DRServerConnection *connection) {

}







void DRBoardsController::receivedBoard(wi_p7_message_t *message) {
    QString path = WSTOQS(wi_p7_message_string_for_name(message, WI_STR("wired.board.board")));

    DRBoardItem *board = this->boardsModel->boardForPath(path);

    DRBoardItem *newBoard = this->boardsModel->addBoard(message, board);

    if(newBoard != NULL)
        this->getThreads(newBoard);
}







void DRBoardsController::receivedThread(wi_p7_message_t *message) {
    QString boardPath = WSTOQS(wi_p7_message_string_for_name(message, WI_STR("wired.board.board")));

    DRBoardItem *board = (DRBoardItem *)this->boardsModel->boardForPath(boardPath);

    if(board == NULL)
        return;

    DRThreadItem *thread = new DRThreadItem(message, board, this->connection);
    board->addThread(thread);

    this->threadsModel->appendRow(new QStandardItem(thread->subject));
}









