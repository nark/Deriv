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
    job.setAutoDelete( false );
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
