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



#ifndef DRERROR_H
#define DRERROR_H

#include <QObject>
#include <QHash>
#include <wired/wired.h>



#define DR_ERROR_DOMAIN_WIRED "fr.read-write.Wired"
#define DR_ERROR_DOMAIN_DERIV "fr.read-write.Deriv"



/**
 * @brief DRErrorKeys defines used keys to build the error params hash: a description and a reason.
 */
enum DRErrorKeys {
    DRErrorDescriptionKey,
    DRErrorReasonKey
};




/**
 * @brief The WIWiredAdditionsErrorDomain enum lists internal error codes.
 */
enum _WIWiredAdditionsErrorDomain {
    WIAddressLookupFailed,
    WIAddressNetServiceLookupFailed,

    WISocketConnectFailed,
    WISocketListenFailed,
    WISocketAcceptFailed,
    WISocketWriteFailed,
    WISocketReadFailed,

    WIP7SpecLoadFailed
};
typedef enum _WIWiredAdditionsErrorDomain				WIWiredAdditionsErrorDomain;




/**
 * @brief The WCWiredClientError enum lists Wired 2.0 client related error codes.
 */
enum _WCWiredClientError {
    WCWiredClientServerDisconnected,
    WCWiredClientBanned,
    WCWiredClientTransferDownloadDirectoryNotFound,
    WCWiredClientTransferExists,
    WCWiredClientTransferFailed,
    WCWiredClientUserNotFound,
};
typedef enum _WCWiredClientError				WCWiredClientError;




/**
 * @brief The WCWiredProtocolError enum lists Wired 2.0 protocol related error codes.
 */
enum _WCWiredProtocolError {
    WCWiredProtocolInternalError				= 0,
    WCWiredProtocolInvalidMessage				= 1,
    WCWiredProtocolUnrecognizedMessage			= 2,
    WCWiredProtocolMessageOutOfSequence			= 3,
    WCWiredProtocolLoginFailed					= 4,
    WCWiredProtocolPermissionDenied				= 5,
    WCWiredProtocolNotSubscribed				= 6,
    WCWiredProtocolAlreadySubscribed			= 7,
    WCWiredProtocolChatNotFound					= 8,
    WCWiredProtocolAlreadyOnChat				= 9,
    WCWiredProtocolNotOnChat					= 10,
    WCWiredProtocolNotInvitedToChat				= 11,
    WCWiredProtocolUserNotFound					= 12,
    WCWiredProtocolUserCannotBeDisconnected		= 13,
    WCWiredProtocolFileNotFound					= 14,
    WCWiredProtocolFileExists					= 15,
    WCWiredProtocolAccountNotFound				= 16,
    WCWiredProtocolAccountExists				= 17,
    WCWiredProtocolAccountInUse					= 18,
    WCWiredProtocolTrackerNotEnabled			= 19,
    WCWiredProtocolNotRegistered				= 20,
    WCWiredProtocolBanNotFound					= 21,
    WCWiredProtocolBanExists					= 22,
    WCWiredProtocolBoardNotFound				= 23,
    WCWiredProtocolBoardExists					= 24,
    WCWiredProtocolThreadNotFound				= 25,
    WCWiredProtocolPostNotFound					= 26,
    WCWiredProtocolRsrcNotSupported				= 27
};
typedef enum _WCWiredProtocolError				WCWiredProtocolError;



/**
 * @brief The DRError class represents errors as a Qt object. An error has an error code and beyond
 * to a particular error domain. The class also provides human error description and reason.
 */
class DRError : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief DRError builds an error object from code and parameters.
     * @param code a valid error code (see WIWiredAdditionsErrorDomain, WCWiredClientError and
     * WCWiredProtocolError)
     * @param params a hash of parameters using keys defined in DRErrorKeys enumeration
     * @param domain a valid domain (see DR_ERROR_DOMAIN_WIRED and DR_ERROR_DOMAIN_DERIV)
     * @param parent
     */
    explicit DRError(int code, QHash<DRErrorKeys,QString> params,
                     QString domain = DR_ERROR_DOMAIN_WIRED, QObject *parent = 0);


    /**
     * @brief DRError builds an error object from a Wired message. Domain is DR_ERROR_DOMAIN_WIRED.
     * @param a Wired error message object
     * @param parent
     */
    explicit DRError(wi_p7_message_t *message, QObject *parent = 0);



    /**
     * @brief errorCode returns the error code
     * @return error code as integer
     */
    int errorCode();



    /**
     * @brief errorDescription returns the error description as string
     * @return error description as string
     */
    QString errorDescription();


    /**
     * @brief errorReason returns the error reason as string
     * @return error reason as string
     */
    QString errorReason();

private:
    int     _code;
    QString _domain;
    QString _description;
    QString _reason;
    QHash<DRErrorKeys, QString> _params;
};

#endif // DRERROR_H
