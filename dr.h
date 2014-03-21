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


#ifndef DR_H
#define DR_H

#include <QObject>
#include <QMainWindow>
#include <QDateTime>
#include <wired/wired.h>
#include "drerror.h"

/** @brief A static shortcut for wiredStringToQString method */
#define WSTOQS(wstring) DR::wiredStringToQString(wstring)

/** @brief A static shortcut for qStringToWiredString method */
#define QSTOWS(qstring) DR::qStringToWiredString(qstring)


/**
 * @brief The DR class is mostly a set of static methods that makes the bridge between Qt types and
 * libwired types. It also host formatting and util methods that are supposed to move in another
 * more appropiated location in the future.
 */
class DR : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief loadSpecification loads the Wired 2.0 specification from a XML file embed in
     * application resources (wired.xml).
     * @param spec Is a pointer to a pointer to the Wired specification object.
     * @param error Is a pointer to a pointer to an error object.
     * @return true if the specification is properly loaded
     */
    static bool loadSpecification(wi_p7_spec_t **spec, DRError **error);



    /**
     * @brief showError displays a QMessageBox based on error object parameters. The message box
     * can be shown as a modal window (if the system supports it) if the modalParent parameter is
     * provided.
     * @param error a valid error object
     * @param modalParent a valid QMainWindow object
     */
    static void showError(DRError *error, QMainWindow *modalParent = 0);



    /**
     * @brief loadPasswordForULRIdentifier loads a password from your user keychain regarding to
     * the given URL identifier (login@address:port). It is a wrapper method around the
     * QtKeychain library (for more details check https://github.com/frankosterfeld/qtkeychain).
     * @param identifier a valid URL identifier (login@address:port)
     * @return the password in clear text
     */
    static QString loadPasswordForULRIdentifier(QString identifier);



    /**
     * @brief savePasswordForULRIdentifier save a password into your user keychain for a given URL
     * identifier (i.e. login@address:port). It is a wrapper method around the QtKeychain library
     * (for more details check https://github.com/frankosterfeld/qtkeychain).
     * @param password
     * @param identifier
     * @return true if password is properly saved
     */
    static bool savePasswordForULRIdentifier(QString password, QString identifier);



    /**
     * @brief iconFromWiredData creates a QIcon instance from a wi_data_t object.
     * @param data a Wired data object that represents a base64 image
     * @return an icon instance
     */
    static QIcon iconFromWiredData(wi_data_t *data);



    /**
     * @brief getDefaultBase64UserIcon returns the default Wired icon as base64 data.
     * @return a Wired data object
     */
    static wi_data_t* getDefaultBase64UserIcon();



    /**
     * @brief iconForBase64String returns a QIcon from a base64 string.
     * @param base64 a base64 string encoded following the RFC 2045.
     * (http://www.rfc-editor.org/rfc/rfc2045.txt)
     * @return an icon instance
     */
    static QIcon iconForBase64String(QString base64);



    /**
     * @brief wiredStringToQString converts a wi_string_t object to a QString instance.
     * @param wstring a Wired string object
     * @return a Qt string instance
     */
    static QString wiredStringToQString(wi_string_t *wstring);



    /**
     * @brief qStringToWiredString converts a QString instance to a wi_string_t object.
     * @param qstring a Qt string instance
     * @return a Wired string object
     */
    static wi_string_t* qStringToWiredString(QString qstring);



    /**
     * @brief wiredDateToQDateTime converts a wi_date_t object to a QDateTime object
     * @param wdate a Wired date object
     * @return a date time instance
     */
    static QDateTime wiredDateToQDateTime(wi_date_t *wdate);



    /**
     * @brief humanReadableElapsedTimeSinceDate formats the given date time in a human readable
     * string with the following format: x days x hours x min x sec
     * @param date since when the interval started
     * @return a formatted QString
     */
    static QString humanReadableElapsedTimeSinceDate(QDateTime date);



    /**
     * @brief humanReadableElapsedTime formats the given time interval in a human readable string
     * with the following format: x days x hours x min x sec
     * @param interval of time in seconds
     * @return a formatted QString
     */
    static QString humanReadableElapsedTime(quint32 interval);



    /**
     * @brief humanReadableStringForSizeInBytes formats the given bytes size in a human readable
     * string. (KB, MB, GB, etc)
     * @param size as unsigned long long type
     * @return a formatted QString
     */
    static QString humanReadableStringForSizeInBytes(unsigned long long size);
};

#endif // DR_H
