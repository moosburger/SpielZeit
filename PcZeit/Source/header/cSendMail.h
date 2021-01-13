/*< Jungheinrich > ********************************************************************************/
/*!
*	\brief      Headerdatei zu cSendMail.cpp
*	\details
*
*	\file		cSendMail.h
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
* cSendMail.h
*/

#ifndef SENDMAIL_H
#define SENDMAIL_H

/**************************************************************************************************
* Includes
**************************************************************************************************/
#include <QtWidgets>

/**************************************************************************************************
* Defines
**************************************************************************************************/

//*************************************************************************************************
// KlassenName:     cSendMail
/// \details        -
//*************************************************************************************************

class cSendMailWorker : public QObject
{
    Q_OBJECT

public:
    void regMember(const QString &MailServer, const QString &User, const QString &Pwd, const QString &MailRcpt);

public slots:
void slotSendMail(QString strSubject, QString strZeit, QString strSpZt, QString strMain);
    void slotReSendMail();

signals:
    void sigRetStat(bool retStat);
    void sigRetMsg(QString strMsg);

private slots:
    void slotReadStdOut();
    void slotReadStdErr();

private:
    QProcess* createProcess(QString path);
    void createSendProcess();
    void runProcess(QProcess* proc, QString program, QStringList arguments, QString strMsg);

    bool mbProcessOk;
    QProcess* mLabelCmd; 
    QString mPath;
    QString mMailServer;
    QString mUser;
    QString mPwd;
    QString mMailRcpt;
    QFile mMailFile;
    QString mProgram;
};

class cSendMail : public QObject
{
    Q_OBJECT
        QThread cSendMailWorkerThread;

public:
    cSendMail(const QString &MailServer, const QString &User, const QString &Pwd, const QString &MailRcpt, bool bLog, bool bLstSnd);
    virtual ~cSendMail();
    bool MailIt(QString strSubject, QString strZeit, QString strSpZt, QString strMain);
    cSendMailWorker* msndWorker;
    bool SendStatus();

public slots:
    void slotRetStatus(bool retStat);
    void slotRetMsg(QString strMsg);

signals:
    void sigSendMail(QString strSubject, QString strZeit, QString strSpZt, QString strMain);
    void sigReSendMail();

private:
    QFile mLog;
    bool mbFileOpen;
    bool mbLstSnd;
    bool mbProcessOk;
    QProcess* mLabelCmd;
    QString mMailServer;
    QString mUser;
    QString mPwd;
    QString mMailRcpt;

};
/************************ Ende Klasse cSendMail *************************************************/
#endif 
/************************ Ende cSendMail.h ******************************************************/