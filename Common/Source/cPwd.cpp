/*< Jungheinrich > ********************************************************************************/
/*!
*	\brief      DriveFilter
*	\details    Filtert die ungültigen entfernbaren Laufwerke und Dateien ausd er Liste
*
*	\file		cPwd.cpp
*
*	\copyright  (C) 2014 Jungheinrich GmbH, All rights reserved.
*	\date       Erstellt am: 12.03.2015
*	\author     Gerhard Prexl
*
*	\version    1.0  -  12.03.2015
*/
/*< History > *************************************************************************************
*	Version     Datum       Kuerzel      Ticket#     Beschreibung
*   1.0         12.03.2015  GP           -------     Ersterstellung
* </ History ></ Jungheinrich > ******************************************************************/

/**
* \addtogroup Modules-HandCoded
* \addtogroup DriveFilter
* \ingroup Modules-HandCoded
* cLogFile.cpp
*/

/**************************************************************************************************
* Includes
**************************************************************************************************/
#include <QtWidgets>
#include <QTinyAES>

#include "cPwd.h"
#include "Version.h"

/**************************************************************************************************
* Defines
**************************************************************************************************/
#define cCRYPTO "587b14e9ef30e95b64dc5ec71230197a"
#define cPass "ù`ƒYJµl¼p3&÷vM®ÞþDBî‘uºË–ÖÙ" //Apfel2Schorle3
/**************************************************************************************************
* Variablen
**************************************************************************************************/

/**************************************************************************************************
* Funktionen
**************************************************************************************************/

//*************************************************************************************************
// FunktionsName:   Konstruktor
/// \details        -
/// \param[in]      -
/// \return         -
//*************************************************************************************************
cPwdHandler::cPwdHandler()
{
    QSettings settings(qApp->organizationName(), qApp->applicationName());
    QByteArray data = settings.value("Pwd").toByteArray();
    if (data.isEmpty())
    {
        settings.setValue("Pwd", QByteArray(cPass));
        data = QByteArray(cPass);
    }

   /* TinyAES crypto;
    QByteArray key = crypto.HexStringToByte(cCRYPTO);
    QByteArray decrypted = crypto.Decrypt(data, key);
    mPwd = QString(decrypted);*/

    mPwd = DecryptPwd(data);
}
/************************ Ende Konstruktor *****************************************************/

//*************************************************************************************************
// FunktionsName:   Destruktor
/// \details        -
/// \param[in]      -
/// \return         -
//*************************************************************************************************
cPwdHandler::~cPwdHandler()
{

}
/************************ Ende Konstruktor *****************************************************/

//*************************************************************************************************
// FunktionsName:   PwdAbfrage
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
int cPwdHandler::PwdAbfrage(QWidget* pthis)
{
    return mPwdAbf("", pthis);
}
/************************ Ende PwdAbfrage ****************************************************/

//*************************************************************************************************
// FunktionsName:   PwdAbfrage
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
int cPwdHandler::PwdAbfrage(const QString Title)
{
    return mPwdAbf(Title);
}
/************************ Ende PwdAbfrage ****************************************************/

//*************************************************************************************************
// FunktionsName:   ChangePwd
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
bool cPwdHandler::ChangePwd(const QString Title)
{
    if (mPwdAbf(Title) != ERR_OK)
        return false;

    return mChangePwd(Title);
}
/************************ Ende ChangePwd ****************************************************/

//*************************************************************************************************
// FunktionsName:   ChangePwd
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
bool cPwdHandler::ChangePwd(QWidget* pThis)
{
    if (mPwdAbf("", pThis) != ERR_OK)
        return false;

    return mChangePwd("", pThis);
}
/************************ Ende ChangePwd ****************************************************/

//*************************************************************************************************
// FunktionsName:   mPwdAbf
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
int cPwdHandler::mPwdAbf(const QString Title, QWidget* pThis)
{
    bool ok;
    QString text = QInputDialog::getText(pThis, Title, QStringLiteral("Passwortabfrage"), QLineEdit::Password, "", &ok, Qt::FramelessWindowHint);

    if (!ok)
        return ERR_IGNORE;

    if (ok && text != mPwd)
        return ERR_NOK;

    return ERR_OK;
}
/************************ Ende mPwdAbf ****************************************************/

//*************************************************************************************************
// FunktionsName:   mDecryptPwd
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
QString cPwdHandler::DecryptPwd(QByteArray data)
{
    TinyAES crypto;
    QByteArray key = crypto.HexStringToByte(cCRYPTO);
    QByteArray decrypted = crypto.Decrypt(data, key);
    return QString(decrypted);
}
/************************ Ende mDecryptPwd ****************************************************/

//*************************************************************************************************
// FunktionsName:   mEncryptPwd
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
QByteArray cPwdHandler::EncryptPwd(QString text)
{
    TinyAES crypto;
    QByteArray key = crypto.HexStringToByte(cCRYPTO);
    QByteArray data = text.toUtf8();
    return crypto.Encrypt(data, key);
}
/************************ Ende mEncryptPwd ****************************************************/

//*************************************************************************************************
// FunktionsName:   mChangePwd
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
bool cPwdHandler::mChangePwd(const QString Title, QWidget* pThis)
{
    bool ok;
    bool tryAgain;
    QStringList text = { "", "" };

    do
    {
        tryAgain = false;
        for (int i = 0; i < 2; i++)
        {
            text[i] = QInputDialog::getText(pThis, Title, QStringLiteral("Neues Passwort eingeben"), QLineEdit::Password, "", &ok, Qt::FramelessWindowHint);
            //if (!ok || text[i].isEmpty()) return false;
        }
        if (text[0] != text[1])
        {
            int ret = QMessageBox::information(NULL, Title, QStringLiteral("Passwörter stimmen nicht überein, erneuter Versuch?"), QMessageBox::Ok | QMessageBox::Cancel);
            if (ret == QMessageBox::Ok)
                tryAgain = true;
            else
                return false;
        }
    }
    while (tryAgain);
    
    /*TinyAES crypto;
    QByteArray key = crypto.HexStringToByte(cCRYPTO);
    QByteArray data = text[0].toUtf8();    
    QByteArray encrypted = crypto.Encrypt(data, key);    */ 
    QByteArray encrypted = EncryptPwd(text[0]);
    QSettings settings(qApp->organizationName(), qApp->applicationName());
    settings.setValue("Pwd", encrypted);
    
    return true;
}
/************************ Ende mChangePwd ****************************************************/

//*************************************************************************************************
// FunktionsName:   TicketCrypto
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
bool cPwdHandler::TicketCrypto(QStringList* pTicket, bool bDir)
{
    TinyAES crypto;
    QByteArray key = crypto.HexStringToByte(cCRYPTO);
    int cnt = 0;

    if (!bDir)
    { // Neu erzeugte Tickets verschlüsseln und in die Registry speichern
        QSettings settings(qApp->organizationName(), qApp->applicationName());
        foreach(QString strTicket, *pTicket)
        {
            QByteArray data = strTicket.toUtf8();
            QByteArray encrypted = crypto.Encrypt(data, key);
            QString strLabel = "Ticket" + QString::number(cnt++);
            settings.setValue(strLabel, encrypted);
        }
    }
    else
    {// Ausgelesene Tickets entschlüsseln
        QSettings settings(qApp->organizationName(), qApp->applicationName());
        pTicket->clear();
        for (int c = 0; c < cTICKETCNT; c++)
        {
            QString strLabel = "Ticket" + QString::number(cnt++);
            QByteArray data = settings.value(strLabel).toByteArray();
            if (data.isEmpty())
                continue;

            QByteArray decrypted = crypto.Decrypt(data, key);
            pTicket->append(QString(decrypted) + ";" + QString::number(c));
        }
    }
    return true;
}
/************************ Ende TicketCrypto ****************************************************/

/************************ Ende cPwd.cpp ****************************************************/