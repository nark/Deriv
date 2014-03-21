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

#include <QEventLoop>
#include <QDebug>
#include <QResource>
#include <QFile>
#include <QMessageBox>
#include "dr.h"
#include "qtkeychain/keychain.h"



/**
 * Statically load the specification from XML file
 */
bool DR::loadSpecification(wi_p7_spec_t **spec, DRError **error) {
    QHash<DRErrorKeys,QString> errorParams;
    QResource common("/wired.xml");

    QFile commonFile(common.absoluteFilePath());

    if (!commonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        errorParams[DRErrorDescriptionKey] = commonFile.fileName();
        errorParams[DRErrorReasonKey] = commonFile.errorString();
        *error = new DRError(WIP7SpecLoadFailed, errorParams, DR_ERROR_DOMAIN_WIRED);

        qDebug() << "Unable to open file: " << commonFile.fileName() << " besause of error " << commonFile.errorString() << endl;

        return false;
    }

    QTextStream in(&commonFile);
    QString content = in.readAll();

    *spec = wi_p7_spec_init_with_string(wi_p7_spec_alloc(), WI_STR(content.toStdString().c_str()), WI_P7_CLIENT);

    if(!*spec) {
        errorParams[DRErrorDescriptionKey] = commonFile.fileName();
        errorParams[DRErrorReasonKey] = QString(wi_string_cstring(wi_error_string()));
        *error = new DRError(WIP7SpecLoadFailed, errorParams, DR_ERROR_DOMAIN_WIRED);

        wi_log_debug(WI_STR("Could not open wired.xml: %m"));

        return false;
    }

    // ultimate test to validate message building against the specification
    wi_p7_spec_message_t *message = wi_p7_spec_message_with_name(*spec, WI_STR("wired.user.info"));
    if(!message) {
        wi_log_debug(WI_STR("Invalid spec, could not create message"));

        return false;
    }

    return true;
}






void DR::showError(DRError *error, QMainWindow *modalParent) {
    QMessageBox msgBox;

    msgBox.setText(error->errorDescription());
    msgBox.setInformativeText(error->errorReason());
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    msgBox.exec();
}






QString DR::loadPasswordForULRIdentifier(QString identifier) {
    QKeychain::ReadPasswordJob job( QLatin1String("fr.read-write.Deriv") );
    job.setAutoDelete( false );
    job.setKey( identifier );

    QEventLoop loop;
    job.connect( &job, SIGNAL(finished(QKeychain::Job*)), &loop, SLOT(quit()) );
    job.start();
    loop.exec();

    const QString pw = job.textData();

    if ( job.error() ) {
        qDebug() << "Restoring password failed: " << qPrintable(job.errorString());
        return (const char *)0;
    }
    return pw;
}




bool DR::savePasswordForULRIdentifier(QString password, QString identifier) {
    QKeychain::WritePasswordJob job(QLatin1String("fr.read-write.Deriv"));
    job.setAutoDelete( true );
    job.setKey(identifier);
    job.setTextData(password);

    QEventLoop loop;
    job.connect( &job, SIGNAL(finished(QKeychain::Job*)), &loop, SLOT(quit()) );
    job.start();
    loop.exec();

    if ( job.error() ) {
        qDebug() << "Storing password failed: " << qPrintable(job.errorString());
        return false;
    }
    return true;
}




wi_data_t* DR::getDefaultBase64UserIcon() {
    QResource resource("/images/user-icon.png");

    QFile* file = new QFile((resource.absoluteFilePath()));

    file->open(QIODevice::ReadOnly);

    const QByteArray image = file->readAll();

    wi_data_t *data = wi_data_with_bytes(image.constData(), image.size()+1);

    file->close();

    if(data == NULL || wi_data_length(data) <= 0)
        return NULL;

    return data;
}




QIcon DR::iconFromWiredData(wi_data_t *data) {
    QString base64icon = QString(wi_string_cstring(wi_data_base64(data)));

    return DR::iconForBase64String(base64icon);
}





QIcon DR::iconForBase64String(QString base64) {
    QByteArray base64Data = base64.toUtf8();

    QImage image;
    image.loadFromData(QByteArray::fromBase64(base64Data));

    return QIcon(QPixmap::fromImage(image));;
}






QString DR::wiredStringToQString(wi_string_t *wstring) {
    return QString(wi_string_cstring(wstring));
}


wi_string_t* DR::qStringToWiredString(QString qstring) {
    return wi_string_with_cstring(qstring.toStdString().c_str());
}






QDateTime DR::wiredDateToQDateTime(wi_date_t *wdate) {
    wi_time_interval_t interval = wi_date_time_interval(wdate);

    return QDateTime::fromTime_t(interval);
}


QString DR::humanReadableElapsedTimeSinceDate(QDateTime date) {
    QDateTime now = QDateTime::currentDateTime();
    //long interval = date.daysTo(now) * 1000*60*60*24 + date.time().msecsTo(now.time());

    return DR::humanReadableElapsedTime(date.secsTo(now));
}


QString DR::humanReadableElapsedTime(quint32 interval) {
    QString         string;
    unsigned long   days, hours, minutes, seconds = 0;
    bool			past = false;

    interval = rint(interval);

    if(interval < 0) {
        past = true;
        interval = -interval;
    }

    days = interval / 86400;
    interval -= days * 86400;

    hours = interval / 3600;
    interval -= hours * 3600;

    minutes = interval / 60;
    interval -= minutes * 60;

    seconds = interval;

    if(days > 0) {
        string.sprintf("%lu days %0.2lu hours %0.2lu min %0.2lu sec", days, hours, minutes, seconds);
    }
    else if(hours > 0) {
        string.sprintf("%0.2lu hours %0.2lu min %0.2lu sec", hours, minutes, seconds);
    }
    else if(minutes > 0) {
        string.sprintf("%0.2lu min %0.2lu sec", minutes, seconds);
    }
    else {
        string.sprintf("00:%0.2lu seconds", minutes, seconds);
    }

    if(past)
        string.sprintf("%s ago", string.toStdString().c_str());

    return string;
}




QString DR::humanReadableStringForSizeInBytes(unsigned long long size) {
    QString string;
    double kb, mb, gb, tb, pb;

    if(size < 1000) {
        string.sprintf("%llu bytes", size);
        return string;
    }

    kb = (double) size / 1024.0;

    if(kb < 1000) {
        string.sprintf("%0.1f KB", kb);
        return string;
    }

    mb = (double) kb / 1024.0;

    if(mb < 1000) {
        string.sprintf("%0.1f MB", mb);
        return string;
    }

    gb = (double) mb / 1024.0;

    if(gb < 1000) {
        string.sprintf("%0.1f GB", gb);
        return string;
    }

    tb = (double) gb / 1024.0;

    if(tb < 1000) {
        string.sprintf("%0.1f TB", tb);
        return string;
    }

    pb = (double) tb / 1024.0;

    if(pb < 1000) {
        string.sprintf("%0.1f PB", pb);
        return string;
    }

    return "0 byte";
}
