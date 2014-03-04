#ifndef DRERROR_H
#define DRERROR_H

#include <QObject>
#include <QHash>
#include <wired/wired.h>



#define DR_ERROR_DOMAIN_WIRED "fr.read-write.Wired"
#define DR_ERROR_DOMAIN_DERIV "fr.read-write.Deriv"



enum DRErrorKeys {
    DRErrorDescriptionKey,
    DRErrorReasonKey
};




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





enum _WCWiredClientError {
    WCWiredClientServerDisconnected,
    WCWiredClientBanned,
    WCWiredClientTransferDownloadDirectoryNotFound,
    WCWiredClientTransferExists,
    WCWiredClientTransferFailed,
    WCWiredClientUserNotFound,
};
typedef enum _WCWiredClientError				WCWiredClientError;





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




class DRError : public QObject
{
    Q_OBJECT
public:
    explicit DRError(int code, QHash<DRErrorKeys,QString> params, QString domain = DR_ERROR_DOMAIN_WIRED, QObject *parent = 0);
    explicit DRError(wi_p7_message_t *message, QObject *parent = 0);

    int errorCode();
    QString errorDescription();
    QString errorReason();

private:
    int _code;
    QString _domain;
    QString _description;
    QString _reason;
    QHash<DRErrorKeys, QString> _params;
};

#endif // DRERROR_H
