#include <QDebug>
#include "drerror.h"



DRError::DRError(int code, QHash<DRErrorKeys, QString> params, QString domain, QObject *parent) : QObject(parent)
{
    QString descriptionOption;
    QString reasonOption;

    this->_code = code;
    this->_params = params;
    this->_domain = domain;

    if(!params.isEmpty()) {
        descriptionOption   = params.value(DRErrorDescriptionKey);
        reasonOption        = params.value(DRErrorReasonKey);
    }

    switch (this->_code) {
    case WIAddressLookupFailed:
        this->_description = "Address Lookup Failed";
        this->_reason = QString("Could not resolve the address %1: %2").arg(descriptionOption, reasonOption);
        break;

    case WISocketConnectFailed:
        this->_description = "Connect Failed";
        this->_reason = QString("Could not connect to %1: %2").arg(descriptionOption, reasonOption);
        break;

    case WISocketWriteFailed:
        this->_description = "Socket Write Failed";
        this->_reason = QString("Could not write to %1: %2").arg(descriptionOption, reasonOption);
        break;

    case WISocketReadFailed:
        this->_description = "Socket Read Failed";
        this->_reason = QString("Could not read from %1: %2").arg(descriptionOption, reasonOption);
        break;

    case WIP7SpecLoadFailed:
        this->_description = "Protocol Specification Load Failed";
        this->_reason = QString("Could not load specification %1: %2").arg(descriptionOption, reasonOption);
        break;

    default:
        this->_description = "Unknow Error";
        this->_reason = "Unknow Error Reason";
        break;
    }
}





DRError::DRError(wi_p7_message_t *message, QObject *parent) : QObject() {
    wi_p7_enum_t error;

    wi_p7_message_get_enum_for_name(message, &error, WI_STR("wired.error"));

    this->_code = (int)error;
    this->_domain = "fr.read-write.Deriv";

    switch (this->_code) {
    case WCWiredProtocolLoginFailed:
        this->_description = "Login Failed";
        this->_reason = "Could not login, the user name and/or password you supplied was rejected.";
        break;

    default:
        this->_description = "Unknow Client Error";
        this->_reason = "Unknow Client Error Reason";
        break;
    }
}






int DRError::errorCode() {
    return this->_code;
}




QString DRError::errorDescription() {
    return this->_description;
}



QString DRError::errorReason() {
    return this->_reason;
}




