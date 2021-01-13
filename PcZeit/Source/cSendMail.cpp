/*< Jungheinrich > ********************************************************************************/
/*!
*	\brief      DriveFilter
*	\details    Filtert die ungültigen entfernbaren Laufwerke und Dateien ausd er Liste
*
*	\file		cSendMail.cpp
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
* cSendMail.cpp
*/

/**************************************************************************************************
* Includes
**************************************************************************************************/
#include "cSendMail.h"

/**************************************************************************************************
* Defines
**************************************************************************************************/

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
cSendMail::cSendMail(const QString &MailServer, const QString &User, const QString &Pwd, const QString &MailRcpt, bool bLog, bool bLstSnd) :
mMailServer(MailServer),
mUser(User),
mPwd(Pwd),
mMailRcpt(MailRcpt),
mbLstSnd(bLstSnd)
{
    mbFileOpen = false;
    mbProcessOk = true;
    if (bLog)
    {
        mLog.setFileName(QCoreApplication::applicationDirPath() + "/cUrlLog.txt");
        mbFileOpen = mLog.open(QIODevice::Append | QIODevice::Text);
    }

    msndWorker = new cSendMailWorker;
    msndWorker->regMember(MailServer, User, Pwd, MailRcpt);
    msndWorker->moveToThread(&cSendMailWorkerThread);

    connect(&cSendMailWorkerThread, &QThread::finished, msndWorker, &QObject::deleteLater);
    connect(this, &cSendMail::sigSendMail, msndWorker, &cSendMailWorker::slotSendMail);
    connect(this, &cSendMail::sigReSendMail, msndWorker, &cSendMailWorker::slotReSendMail);
    connect(msndWorker, &cSendMailWorker::sigRetStat, this, &cSendMail::slotRetStatus);
    connect(msndWorker, &cSendMailWorker::sigRetMsg, this, &cSendMail::slotRetMsg);

    cSendMailWorkerThread.start();
}
/************************ Ende Konstruktor *****************************************************/

//*************************************************************************************************
// FunktionsName:   Destruktor
/// \details        -
/// \param[in]      -
/// \return         -
//*************************************************************************************************
cSendMail::~cSendMail()
{
    mLog.close();
}
/************************ Ende Konstruktor *****************************************************/

//*************************************************************************************************
// FunktionsName:   SendStatus
/// \details        Status der letzten Mail, konnte diese versendet werden
/// \param          -
/// \return         -
//*************************************************************************************************
bool cSendMail::SendStatus()
{
    return mbProcessOk;
}
/************************ Ende Konstruktor *****************************************************/

//*************************************************************************************************
// FunktionsName:   slotRetStatus
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cSendMail::slotRetStatus(bool retStat)
{
    mbProcessOk = retStat;
    if (mbFileOpen)
    {
        QTextStream out(&mLog);
        out << "Gesendet: " << retStat << "\n";
    }
    qDebug() << "Gesendet: " << retStat << "\n";
}
/************************ Ende slotRetStatus *****************************************************/

//*************************************************************************************************
// FunktionsName:   slotRetMsg
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cSendMail::slotRetMsg(QString strMsg)
{
    if (mbFileOpen)
    {
        QTextStream out(&mLog);
        out << strMsg << "\n";
    }
    qDebug() << strMsg << "\n";
}
/************************ Ende slotRetMsg *****************************************************/

//*************************************************************************************************
// FunktionsName:   MailIt
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
bool cSendMail::MailIt(QString strSubject, QString strZeit, QString strSpZt, QString strMain)
{
    if (mUser.isEmpty() || mMailServer.isEmpty() || mPwd.isEmpty() || mMailRcpt.isEmpty())
        return false;

    if (!mbLstSnd)
    {
        mbLstSnd = true;
        emit(sigReSendMail());
    }

    emit(sigSendMail(strSubject, strZeit, strSpZt, strMain));
    return true;
}
/************************ Ende MailIt *****************************************************/



/**************************************************************************************************
**************************************************************************************************
**************************************************************************************************
**************************************************************************************************
* WorkerThread
**************************************************************************************************
**************************************************************************************************
**************************************************************************************************
**************************************************************************************************/

//*************************************************************************************************
// FunktionsName:   regMember
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cSendMailWorker::regMember(const QString &MailServer, const QString &User, const QString &Pwd, const QString &MailRcpt)
{
    mPath = QCoreApplication::applicationDirPath();
    mMailFile.setFileName(QString("%1/Mail.txt").arg(mPath));
    mProgram = QString("\"%1/curl.exe\"").arg(mPath);

    mMailServer = MailServer;
    mUser = User;
    mPwd = Pwd;
    mMailRcpt = MailRcpt;
}
/************************ Ende regMember *****************************************************/

//*************************************************************************************************
// FunktionsName:   slotReSendMail
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cSendMailWorker::slotReSendMail()
{
    if (!mMailFile.exists())
        return;

    createSendProcess();
    mMailFile.remove();

    emit(sigRetStat(mbProcessOk));
}
/************************ Ende slotReSendMail *****************************************************/

//*************************************************************************************************
// FunktionsName:   slotSendMail
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cSendMailWorker::slotSendMail(QString strSubject, QString strZeit, QString strSpZt, QString strMain)
{
    if (mMailFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&mMailFile);

        QString from = QString("From: \"%1\" <%1>\n").arg(mUser);
        QString to = QString("To: \"%1\" <%1>\n").arg(mMailRcpt);
        strSubject = QString("Subject: %1 %2 %3\n\n").arg(strSubject, strZeit, strSpZt);
        strMain.prepend("\n");
        out << from << to << strSubject  << strMain;
    }
    mMailFile.close();

    createSendProcess();

    if (mbProcessOk)
        mMailFile.remove();
}
/************************ Ende slotSendMail *****************************************************/

//*************************************************************************************************
// FunktionsName:   createSendProcess
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cSendMailWorker::createSendProcess()
{
    mbProcessOk = true;
    mLabelCmd = createProcess(mPath);

    // Prozess für CMD aufsetzen
    QString strMsg;
    QStringList arguments;
    arguments << "-v" << "--ssl-reqd" << "-k" << "--anyauth";
    arguments.append(QString("--url \"smtps://%1\"").arg(mMailServer));
    arguments.append(QString("--mail-from %1").arg(mUser));
    arguments.append(QString("--mail-rcpt %1").arg(mMailRcpt));
    arguments.append(QString("--user %1:%2").arg(mUser, mPwd));
    arguments.append(QString("--upload-file \"%1\"").arg(mMailFile.fileName()));

    // Starten
    runProcess(mLabelCmd, mProgram, arguments, strMsg);
    delete mLabelCmd;
}
/************************ Ende createSendProcess *****************************************************/

//*************************************************************************************************
// FunktionsName:   slotReadStdErr
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cSendMailWorker::slotReadStdErr()
{
    mbProcessOk = false;
    // CodePage für DOS Console
    QTextCodec *codec = QTextCodec::codecForName("IBM 850");
    QByteArray qbArr = mLabelCmd->readAllStandardError();
    QString stdErr = codec->toUnicode(qbArr);

    // Solange am Anfang oder Ende ein Wagenrücklauf(CR) oder Zeilenvorschub (LF) vorkommt diesen löschen
    int pos = 0;
    QRegExp rx("(^\\n|^\\r|\\n$|\\r$|^\\r\\n|\\r\\n$)", Qt::CaseInsensitive);
    while ((pos = rx.indexIn(stdErr, pos)) != -1)
    {
        stdErr = stdErr.remove(rx);
        pos = rx.indexIn(stdErr, pos);
    }
    pos = 0;
    // Solange doppelte CR, LF oder CRLF vorkommen diesen durch einen ersetzen
    rx.setPattern("(\\n|\\r|\\r\\n){2}");
    while ((pos = rx.indexIn(stdErr, pos)) != -1)
    {
        stdErr = stdErr.replace(rx, "\n");
        pos = rx.indexIn(stdErr, pos);
    }

    // Fehler beim versenden
    //rx.setPattern("(curl: \\(\\d{1,3}\\))");
    //if (rx.indexIn(stdErr) != -1)
    //    mbProcessOk = false;

    emit(sigRetMsg(stdErr));
}
/************************ Ende slotReadStdErr *****************************************************/

//*************************************************************************************************
// FunktionsName:   slotReadStdOut
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cSendMailWorker::slotReadStdOut()
{
    QTextCodec *codec = QTextCodec::codecForName("IBM 850");
    QByteArray qbArr = mLabelCmd->readAllStandardOutput();
    QString stdOut = codec->toUnicode(qbArr);

    // Solange am Anfang oder Ende ein Wagenrücklauf(CR) oder Zeilenvorschub (LF) vorkommt diesen löschen
    int pos = 0;
    QRegExp rx("(^\\n|^\\r|\\n$|\\r$|^\\r\\n|\\r\\n$)", Qt::CaseInsensitive);
    while ((pos = rx.indexIn(stdOut, pos)) != -1)
    {
        stdOut = stdOut.remove(rx);
        pos = rx.indexIn(stdOut, pos);
    }
    pos = 0;
    // Solange doppelte CR, LF oder CRLF vorkommen diesen durch einen ersetzen
    rx.setPattern("(\\n|\\r|\\r\\n){2}");
    while ((pos = rx.indexIn(stdOut, pos)) != -1)
    {
        stdOut = stdOut.replace(rx, "\n");
        pos = rx.indexIn(stdOut, pos);
    }

    // Fehler beim versenden bsp "curl: (35) Unknown SSL protocol error"
    rx.setPattern("(curl: \\(\\d{1,3}\\))");
    if (rx.indexIn(stdOut) != -1)
        mbProcessOk = false;

    emit(sigRetMsg(stdOut));
}
/************************ Ende slotReadStdOut *****************************************************/

//*************************************************************************************************
// FunktionsName:   createProcess
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
QProcess* cSendMailWorker::createProcess(QString path)
{
    QProcess* proc = new QProcess(this);
    proc->setWorkingDirectory(path);
    connect(proc, SIGNAL(readyReadStandardError()), this, SLOT(slotReadStdErr()));
    connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(slotReadStdOut()));

    return proc;
}
/************************ Ende createProcess *****************************************************/

//*************************************************************************************************
// FunktionsName:   runProcess
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cSendMailWorker::runProcess(QProcess* proc, QString program, QStringList arguments, QString strMsg)
{
    proc->setNativeArguments(arguments.join(" "));
    proc->start(program);

    QString strArg = arguments.join(" ");
    emit(sigRetMsg(program + " " + strArg));

    proc->waitForFinished();
    if ((proc->exitCode() != 0) || (proc->exitStatus() != 0))
        mbProcessOk = false;

    if (mbProcessOk)
        emit(sigRetMsg(strMsg));

    emit(sigRetStat(mbProcessOk));
}
/************************ Ende runProcess *****************************************************/

/************************ Ende cLogFile.cpp ****************************************************/