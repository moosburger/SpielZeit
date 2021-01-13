/*<  > ********************************************************************************/
/*!
*	\brief      Main, die Mutter aller Dateien
*	\details
*
*	\file		main.cpp
*
*	\copyright  (C) 2014, All rights reserved.
*	\date       Erstellt am: 05.08.2014
*	\author     Gerhard Prexl
*
*	\version    1.0  -  05.08.2014
*/
/*< History > *************************************************************************************
*   Version     Datum       Kuerzel      Ticket#     Beschreibung
*   1.0         05.08.2014  GP           -------     Ersterstellung
* </ History ></ > ******************************************************************/

/**
* \addtogroup Modules-HandCoded
* \addtogroup Main
* \ingroup Modules-HandCoded
* main.cpp
*/

//#ifndef NOMINMAX
//#define NOMINMAX
//#endif
//bei  <qt_windows.h> nicht nötig, windows.h schon
/**************************************************************************************************
* Includes
**************************************************************************************************/
#include <qt_windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>

#include <QtWidgets>
#include "cPcZeit.h"
#include "cWinApiStuff.h"
#include "cSendMail.h"
#include "cLogFile.h"
#include "cPwd.h"

/**************************************************************************************************
* Defines
**************************************************************************************************/
#define cReadDelay 5000
#define cPlayDelay 30000
#define cMin 60
#define cFadeOut 25
#define cFadeIn 10
#define cFadeStep 0.01
#define cShow 10000
#define cRemind 5500
#define c15min "+15min"
#define c5min "+5min"

/**************************************************************************************************
* Variablen
**************************************************************************************************/

/**************************************************************************************************
* Funktionen
**************************************************************************************************/

//*************************************************************************************************
// FunktionsName:   Konstruktor
/// \details        Einsprung ins Programm
/// \param[in]      -
/// \return         -
//*************************************************************************************************
cPcZeit::cPcZeit(QObject* parent)
{
    mPID = NULL;
    mRGB = NULL;
    mb5Flag = false;
    mb1Flag = false;
    mFirstLog = true;
    //mFirstRun = true;
    mbRegChanged = false;
    //mTicket = 0;
    mPwdCnt = 0;
    miPlusZeit = 0;
    
    //Logger = new cLogFile(QDir::currentPath() + "/LogFile.txt");
    Logger = new cLogFile(QCoreApplication::applicationDirPath() + "/LogFile.txt");
    mWinApi = new cWinApiStuff(&mPrograms, Logger);
    mPwd = new cPwdHandler();

    //Log Programmstart
    Logger->LogMsg(QStringLiteral("\n%1\nProgrammstart am %2 um %3\n%4")
        .arg(cSeperator, QDate::currentDate().toString("dddd dd.MM.yyyy"), QTime::currentTime().toString("hh.mm"), cSeperator));

    createTrayIcon();

    mReadTimer = new QTimer(this);
    mPlayTimer = new QTimer(this);  
    mFadeInTimer = new QTimer(this);
    mFadeOutTimer = new QTimer(this);
    mShowTimer = new QTimer(this);
    mReminderTimer = new QTimer(this);

    mFilter.registerLogger(Logger);
    QAbstractEventDispatcher::instance()->installNativeEventFilter(&mFilter);

    QObject::connect(&mFilter, SIGNAL(sigGoingSuspend()), this, SLOT(slotWriteSettings()));
    QObject::connect(&mFilter, SIGNAL(sigResume()), this, SLOT(slotReadSettings()));

    QObject::connect(mReadTimer, SIGNAL(timeout()), this, SLOT(slotRunReadLoop()));
    QObject::connect(mPlayTimer, SIGNAL(timeout()), this, SLOT(slotRunPlayLoop()));
    QObject::connect(mFadeInTimer, SIGNAL(timeout()), this, SLOT(slotFadeIn()));
    QObject::connect(mFadeOutTimer, SIGNAL(timeout()), this, SLOT(slotFadeOut()));
    QObject::connect(mShowTimer, SIGNAL(timeout()), this, SLOT(slotShow()));
    QObject::connect(mReminderTimer, SIGNAL(timeout()), this, SLOT(slotReminder()));

    //Ticket signal
    QObject::connect(this, SIGNAL(sigTicket()), this, SLOT(slotTicket()));

    mMbox = new QMessageBox(NULL);
    mMbox->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    mMbox->setAttribute(Qt::WA_Hover);
    mButtYesRole = mMbox->addButton("Ok", QMessageBox::YesRole);
    mButtNoRole = mMbox->addButton("Nein", QMessageBox::NoRole);
    QObject::connect(mMbox, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(slotMsgBoxbuttonClicked(QAbstractButton*)));

    readStyleSheet();
    loadTheme();
    mSendIt = new cSendMail(mailServerPort, mailUser, mailPwd, mailEmpfanger, mailCurlLog, mailLastSend);

    mTrayIcon->show();
    //Logger->LogMsg("\t\t\t\t\t\tDebug: Show TrayIcon (Constructor)");

    mMbox->installEventFilter(this);

    //QString strMsg = QStringLiteral("Nicht gut! Deine Zeit in der Du spielen darfst ist überschritten.\nIn %1 min. wird das Spiel beendet, besser Du speicherst jetzt!");
    //showMsg(cTitel, strMsg.arg(QString::number(mVerbSpielzeit / cMin)), QMessageBox::Critical, true);

    //if(!mSendIt->MailIt("StartZeit", "\nBody\nBody"))
    //    Logger->LogMsg("Keine SMTP Angaben");
}
/************************ Ende quit *****************************************************/

//*************************************************************************************************
// FunktionsName:   Destruktor
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
cPcZeit::~cPcZeit()
{
    slotAboutToQuit();
}
/************************ Ende quit *****************************************************/

//*************************************************************************************************
// FunktionsName:   eventFilter
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
bool cPcZeit::eventFilter(QObject* fncObject, QEvent* fncEvent)
{
    if (fncEvent->spontaneous())
        return false;

    if ((fncObject == mMbox) && (fncEvent->type() == QEvent::HoverEnter))
    {
        mFadeOutTimer->stop();
        mOpacity = 1.0;
        mMbox->setWindowOpacity(mOpacity);
        mMbox->raise();
    }

    if ((fncObject == mMbox) && (fncEvent->type() == QEvent::HoverLeave))
    {
        mOpacity = 1.0;
        mMbox->setWindowOpacity(mOpacity);
        mFadeOutTimer->start(cFadeOut);
        mMbox->raise();
    }
    return false;
}
/************************ Ende quit *****************************************************/

//*************************************************************************************************
// FunktionsName:   slotAboutToQuit
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cPcZeit::slotAboutToQuit()
{
    if (mReadTimer)
    {
        delete mReadTimer;
        mReadTimer = NULL;
    }

    if (mPlayTimer)
    {
        delete mPlayTimer;
        mPlayTimer = NULL;
    }

    if (mFadeInTimer)
    {
        delete mFadeInTimer;
        mFadeInTimer = NULL;
    }

    if (mFadeOutTimer)
    {
        delete mFadeOutTimer;
        mFadeOutTimer = NULL;
    }

    if (mShowTimer)
    {
        delete mShowTimer;
        mShowTimer = NULL;
    }

    if (mWinApi)
    {
        delete mWinApi;
        mWinApi = NULL;
    }

    if (Logger)
    {
        delete Logger;
        Logger = NULL;
    }

    if (mMbox)
    {
        delete mMbox;
        mMbox = NULL;
    }    

    if (mPwd)
    {
        delete mPwd;
        mPwd = NULL;
    }

    if (Logger)
    {
        delete Logger;
        Logger = NULL;
    }

    if (mpProg)
    {
        delete mpProg;
        mpProg = NULL;
    }

    if (mTrayIcon)
    {
        delete mTrayIcon;
        mTrayIcon = NULL;
    }

    if (mSendIt)
    {
        delete mSendIt;
        mSendIt = NULL;
    }
}
/************************ Ende slotAboutToQuit *****************************************************/

//*************************************************************************************************
// FunktionsName:   slotExit
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cPcZeit::slotExit()
{
    if (!PwdAbfrage())
        return;

    mReadTimer->stop();
    mPlayTimer->stop();
    mFadeInTimer->stop();
    mFadeOutTimer->stop();
    mShowTimer->stop();

    writeSettings();

    Logger->LogMsg(QStringLiteral("normales beenden\n%1").arg(cSeperator));

    emit finished();
}
/************************ Ende slotExit *****************************************************/

//*************************************************************************************************
// FunktionsName:   slotPlus
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cPcZeit::slotPlus()
{
    if (!PwdAbfrage())
        return;

    int min = 0;
    QAction *action = qobject_cast<QAction*>(sender());
    if (action->text() == c5min)
        min = 5;

    if (action->text() == c15min)
        min = 15;

    QString strTime = adjTimeNow();
    Logger->LogMsg(QStringLiteral("Spielzeit um %1 min verlängert.%2").arg(QString::number(min), strTime));

    miPlusZeit += (min * cMin);
    mFirstLog = true;

    // speichern
    writeSettings();

    //Damit die Einstellungen wieder ausgelesen werden
    mbRegChanged = true;

    // Timer zur LAN Zeit erinnerung
    mReminderTimer->start(cRemind);
}
/************************ Ende slotPlus *****************************************************/

//*************************************************************************************************
// FunktionsName:   slotNow
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cPcZeit::slotNow()
{
    if (!PwdAbfrage())
        return;

    mTNowBegin = QTime::currentTime();

    Logger->LogMsg(QStringLiteral("Startzeit verändert"));
    mFirstLog = true;
}
/************************ Ende slotPlus *****************************************************/

//*************************************************************************************************
// FunktionsName:   slotTicket
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cPcZeit::slotTicket()
{
    QString Title = cTitel;
    QString Text = QStringLiteral("TicketNummer eingeben");
    bool ok;
    QString strTicket = QInputDialog::getText(NULL, Title, Text, QLineEdit::Normal, "-", &ok, Qt::FramelessWindowHint);

    if (!ok)
        return;

    // Ticket in der Liste
    bool bFnd = false;
    foreach(QString tckt, mTickets)
    {
        QStringList lstTckt = tckt.split(";");
        if (strTicket.trimmed() != lstTckt[0].trimmed())
            continue;

        bFnd = true;

        QString TcktNr = "Ticket" + lstTckt[1];
        QSettings settings(qApp->organizationName(), qApp->applicationName());
        settings.remove(TcktNr);
    }

    if (!bFnd)
    {
        Text = QStringLiteral("Ungültiges Ticket (%1)").arg(strTicket);
        showMsg(cTitel, Text, QMessageBox::Information);
        Logger->LogMsg(Text);
        return;
    }

    QString strTime = adjTimeNow();
    bool Ok;
    int iSpZeit = 0;

    if (strTime.isEmpty())                  // Innerhalb der erlaubten Zeit, also Spielzeit verlängern
    {
        iSpZeit = mSpZeit.toInt(&Ok, 10) * cMin;
        Text = QStringLiteral("Ticket# %1 eingelöst. Spielzeit einmalig um %2 min verlängert.").arg(strTicket, QString::number(iSpZeit / cMin));
    }
    else if (mVerbSpielzeit < miSpZeit)     // ausserhalb der erlaubten Zeit und kaum Spielzeit mehr, erlauben und verlängern
    {
        iSpZeit = mSpZeit.toInt(&Ok, 10) * cMin;
        Text = QStringLiteral("Ticket# %1 eingelöst. Spielzeit einmalig um %2 min verlängert. %3").arg(strTicket, QString::number(iSpZeit / cMin), strTime);
    }
    else                                    // ausserhalb der erlaubten Zeit und volle Spielzeit vorhanden, Spielen erlauben keine Verlängerung
        Text = QStringLiteral("Ticket# %1 eingelöst. %3").arg(strTicket, strTime);
    
    showMsg(cTitel, Text, QMessageBox::Information);
    Logger->LogMsg(Text);
    miPlusZeit += iSpZeit;
    mFirstLog = true;
    //mTicket++;

    // speichern
    writeSettings();

    //Damit die Einstellungen wieder ausgelesen werden
    mbRegChanged = true;

    // Timer zur LAN Zeit erinnerung
    mReminderTimer->start(cRemind);
}
/************************ Ende slotTicket *****************************************************/

//*************************************************************************************************
// FunktionsName:   slotWriteSettings
/// \details        -
/// \param          -
/// \return         -
//********************************************* ****************************************************
void cPcZeit::slotWriteSettings()
{
    mTrayIcon->show();
    //Logger->LogMsg("\t\t\t\t\t\tDebug: Show TrayIcon (slotWriteSettings)");
    writeSettings();
}
/************************ Ende slotWriteSettings *****************************************************/

//*************************************************************************************************
// FunktionsName:   slotWriteSettings
/// \details        -
/// \param          -
/// \return         -
//********************************************* ****************************************************
void cPcZeit::slotReadSettings()
{
    mTrayIcon->show();
    //Logger->LogMsg("\t\t\t\t\t\tDebug: Show TrayIcon (slotReadSettings)");
    mFirstLog = true;
    prepSettings();
}
/************************ Ende slotWriteSettings *****************************************************/

//*************************************************************************************************
// FunktionsName:   slotShowZeit
/// \details        -
/// \param          -
/// \return         -
//********************************************* ****************************************************
void cPcZeit::slotShowZeit()
{
    QString strMin = QString::number(mVerbSpielzeit / cMin);
    QString strSek = QString::number(mVerbSpielzeit % cMin);
    QString strMsg;

    if (mTBakBegin.isValid())
        strMsg = QStringLiteral("verbleibende Spielzeit: %1min %2sek\nvon %3 bis %4 Uhr\n%5")
        .arg(strMin).arg(strSek).arg(mTBakBegin.toString("hh.mm"), mTBakEnd.toString("hh.mm"), mpProg->text());

    mTrayIcon->showMessage(cTitel, strMsg, QSystemTrayIcon::Information, 1000);
}
/************************ Ende slotShowZeit *****************************************************/

//*************************************************************************************************
// FunktionsName:   slotSetup
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cPcZeit::slotSetup()
{
#ifdef NDEBUG
    //QString program = QDir::currentPath() + "/SetUpPcZeit.exe";
    QString program = QCoreApplication::applicationDirPath() + "/SetUpPcZeit.exe";
#else
    QString program = "../Win32/Release/SetUpPcZeit.exe";
#endif
    QFileInfo Info(program);
    if (!Info.exists())
    {
        QMessageBox::critical(NULL, QStringLiteral(cTitel), QStringLiteral("Kann Programm\n%1\nnicht finden!").arg(program), QMessageBox::Ok);
        return;
    }
    QStringList arguments;
    QProcess *myProcess = new QProcess();
    //myProcess->startDetached(program, arguments);
    myProcess->start(program, arguments);
    myProcess->waitForFinished(300000);
    //Damit die Einstellungen wieder ausgelesen werden
    mbRegChanged = true;

    delete myProcess;
}
/************************ Ende slotPlus *****************************************************/

//*************************************************************************************************
// FunktionsName:   slotRunMainLoop
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cPcZeit::slotRunMainLoop()
{
    //TryIcon ToolTip setzten
    mVerbSpielzeit = miSpZeit - miGespZeit;
    setTrayToolTip();

    mReadTimer->start(cReadDelay); // 5sek
}
/************************ Ende slotRunMainLoop *****************************************************/

//*************************************************************************************************
// FunktionsName:   slotRunMainLoop
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cPcZeit::slotRunReadLoop()
{
    // laufen wir noch, oder sind wir eigentlich schon gestopped?
    if (!mReadTimer->isActive())
        return;

    mTrayIcon->show();
    //Logger->LogMsg("\t\t\t\t\t\tDebug: Show TrayIcon (slotRunReadLoop)");
    prepSettings();

    if (mFirstLog)
    {
        LogEntry();
        mFirstLog = false;
    }   

    //Läuft ein Programm
    mProg = mWinApi->CheckRunnigProgram(&mPID);
    if (mProg.isEmpty())
        return;

    QString msg;
    msg = QStringLiteral("Programm '%1' läuft\nverbleibende Spielzeit %2 min %3 sek\n").arg(mProg, QString::number(mVerbSpielzeit / cMin), QString::number(mVerbSpielzeit % cMin));
    Logger->LogMsg(msg);

    msg = QStringLiteral("%1 %2m%3s (%4-%5).txt").arg(mProg, QString::number(mVerbSpielzeit / cMin), QString::number(mVerbSpielzeit % cMin), mTBakBegin.toString("hh.mm"), mTBakEnd.toString("hh.mm"));
    Logger->SrvLog(mSrvLogPfad, msg);

    QDate ToDay = QDate::currentDate();
    QTime Now = QTime::currentTime();

    mpProg->setText(mProg);
    
    // Spielzeit im Rahmen
    bool bRahmen = true;
    if ((Now < mTBakBegin) || (Now > mTBakEnd))
    {
        msg = QStringLiteral("Du spielst ausserhalb der erlaubten Zeit (%1 bis %2)!").arg(mTBakBegin.toString("hh:mm"), mTBakEnd.toString("hh:mm"));
        bRahmen = false;
    }

    if (mVerbSpielzeit <= 0)
    {
        msg = QStringLiteral("Deine Spielzeit (%1 min.) ist abgelaufen!").arg(mSpZeit);
        bRahmen = false;
    }

    if (bRahmen)
    {   // innerhalb der erlaubten Zeit
        msg = QStringLiteral("Spiel %1 am %2 um %3 gestartet,\nverbleibende Spielzeit %4 min %5 sek").arg(mProg, ToDay.toString("dd.MM.yy"), Now.toString("hh.mm"), QString::number(mVerbSpielzeit / cMin), QString::number(mVerbSpielzeit % cMin));
        QString strHlp = "";
        if (!mSendIt->MailIt("Spielstart", Now.toString("hh.mm"), strHlp, msg))
            Logger->LogMsg("Keine SMTP Angaben");
        else
            Logger->LogMsg("Sende Mail");
    }
    else
    {
        Logger->LogMsg(msg);

        ProgrammKill();

        // Ticket einlösen
        msg += QStringLiteral("\nWillst Du ein Ticket einlösen?");
        PopUpBox(cTitel, msg, QMessageBox::Question, false);
        mButtYesRole->setText("Ja");
        mButtNoRole->setVisible(true);

        msg = QStringLiteral("Programm '%1' beendet\n").arg(mProg);
        Logger->LogMsg(msg);
        msg = QStringLiteral("%1 beendet.txt").arg(mProg);
        Logger->SrvLog(mSrvLogPfad, msg);

        msg = QStringLiteral("Spiel %1  am %2 um %3 beendet,\nSpielzeit ausserhalb der Rahmenzeit von %4 bis %5\nverbleibende Spielzeit %6 min %7 sek")
            .arg(mProg, ToDay.toString("dd.MM.yy"), Now.toString("hh.mm"), mTBakBegin.toString("hh:mm"), mTBakEnd.toString("hh:mm"), QString::number(mVerbSpielzeit / cMin), QString::number(mVerbSpielzeit % cMin));
        QString strHlp = "";
        if (!mSendIt->MailIt("Spiel Start und Ende", Now.toString("hh.mm"), strHlp, msg))
            Logger->LogMsg("Keine SMTP Angaben");
        else
            Logger->LogMsg("Sende Mail");
        return;
    }
    // jetzt gehts in die Spieleschleife, ReadTimer Stoppen
    mReadTimer->stop();    
    mPlayTimer->start(cPlayDelay); 
    mTrayIcon->showMessage("", "", QSystemTrayIcon::Information, 0);
}
/************************ Ende slotRunReadLoop *****************************************************/

//*************************************************************************************************
// FunktionsName:   slotRunReadLoop
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cPcZeit::slotRunPlayLoop()
{
    // laufen wir noch, oder sind wir eigentlich schon gestopped?
    if (!mPlayTimer->isActive())
        return;

    QTime Now = QTime::currentTime();
    QDate ToDay = QDate::currentDate();
     
    mTrayIcon->show();
    //Logger->LogMsg("\t\t\t\t\t\tDebug: Show TrayIcon (slotRunPlayLoop)"); 
    if (mbRegChanged)
    {
        mbRegChanged = false; 
        prepSettings();
        LogEntry();
    }

    if (mPwdCnt > 4)
    {
        QString msg = QStringLiteral("Du hast 5 mal versucht das Passwort zu knacken!\nFür jetzt ist spielen beendet");
        Logger->LogMsg(msg);
        showMsg(cTitel, msg, QMessageBox::Critical, true);

        msg = QStringLiteral("Programm '%1' beendet\n").arg(mProg);
        Logger->LogMsg(msg);

        msg = QStringLiteral("Spiel %1 am %2 um %3 wg. hacken des Passwortes beendet,\nverbleibende Spielzeit %4 min %5 sek").arg(mProg, ToDay.toString("dd.MM.yy"), Now.toString("hh.mm"), QString::number(mVerbSpielzeit / cMin), QString::number(mVerbSpielzeit % cMin));
        QString strHlp = QString("%1 min").arg(QString::number(miGespZeit / cMin));
        if (!mSendIt->MailIt("SpielEnde", Now.toString("hh.mm"), strHlp, msg))
            Logger->LogMsg("Keine SMTP Angaben");
        else
            Logger->LogMsg("Sende Mail");

        ProgrammKill();
        return;
    }

    //Läuft das Programm noch
    mProg = mWinApi->CheckRunnigProgram(&mPID);
    if (mProg.isEmpty())
    {
        mPlayTimer->stop();
        mReadTimer->start(cReadDelay);

        Logger->LogMsg(QStringLiteral("Programm '%1' beendet.\nverbleibende Spielzeit %2 min %3 sek").arg(mpProg->text(), QString::number(mVerbSpielzeit / cMin), QString::number(mVerbSpielzeit % cMin)));

        QString msg = QStringLiteral("%1 beendet Restzeit %2m%3s.txt").arg(mpProg->text(), QString::number(mVerbSpielzeit / cMin), QString::number(mVerbSpielzeit % cMin));
        Logger->SrvLog(mSrvLogPfad, msg);

        msg = QStringLiteral("Spiel %1 am %2 um %3 beendet,\nverbleibende Spielzeit %4 min %5 sek").arg(mpProg->text(), ToDay.toString("dd.MM.yy"), Now.toString("hh.mm"), QString::number(mVerbSpielzeit / cMin), QString::number(mVerbSpielzeit % cMin));
        QString strHlp = QString("%1 min").arg(QString::number(miGespZeit / cMin));
        if (!mSendIt->MailIt("SpielEnde", Now.toString("hh.mm"), strHlp, msg))
            Logger->LogMsg("Keine SMTP Angaben");
        else
            Logger->LogMsg("Sende Mail");

        mpProg->setText("");
        writeSettings();
        return;
    }
    //Programm im Vordergrund? 
#ifdef NDEBUG
    if (!mWinApi->AktivesFenster(mProg))
    {
        Logger->LogMsg(QStringLiteral("Programm '%1' läuft im Hintergrund.\nverbleibende Spielzeit %2 min %3 sek").arg(mProg, QString::number(mVerbSpielzeit / cMin), QString::number(mVerbSpielzeit % cMin)));  
        return;
    }
#else
    bool Rennt = mWinApi->AktivesFenster(mProg);
#endif
    Logger->LogMsg(QStringLiteral("Programm '%1' läuft im Vordergrund.\nverbleibende Spielzeit %2 min %3 sek\n").arg(mProg, QString::number(mVerbSpielzeit / cMin), QString::number(mVerbSpielzeit % cMin)));

#ifndef NDEBUG
    mWinApi->mbMouseMove = true;
    mWinApi->mbKeyBoard = false;
#endif

    bool bScrChng = ScreenChange();
    // Maus mal bewegt, KeyBoard betätigt oder Bildschirm hat sich verändert
    if ((!mWinApi->mbMouseMove) && (!mWinApi->mbKeyBoard) && (!bScrChng))
    {
        Logger->LogMsg(QStringLiteral("Keine Aktivität.\nverbleibende Spielzeit %2 min %3 sek").arg(QString::number(mVerbSpielzeit / cMin), QString::number(mVerbSpielzeit % cMin)));
        return;
    }
    Logger->LogMsg(QStringLiteral("Aktivitäten:\nMaus %1\nKeyBoard %2\nBildschirm %3\n%4\n")
                    .arg(QString::number(mWinApi->mbMouseMove), QString::number(mWinApi->mbKeyBoard), QString::number(bScrChng), QTime::currentTime().toString()));
    //Logger->LogMsg(QStringLiteral("Aktivität erkannt"));
    mWinApi->mbMouseMove = false;
    mWinApi->mbKeyBoard = false;

    // Da der Zeitzähler hier ist, gibts beim Start 30 sek. Zeitguthaben. In ReadLoop wird das Spiel als gestartet erkannt, dann 30 Pause bis zur PlayLoop
    miGespZeit += cPlayDelay / 1000; // in Sek

    //Wieviel Zeit ist denn noch?
    mVerbSpielzeit = miSpZeit - miGespZeit;
    setTrayToolTip();

    // Spielzeit im Rahmen
    //QTime Now = QTime::currentTime();
    //bool bRahmen = true;
    //if ((Now < mTBakBegin) || (Now > mTBakEnd))
    //{ // ausserhalb
    //    bRahmen = false;
    //}

    QString strMsg;
    if ((mVerbSpielzeit <= 5 * cMin) && (mVerbSpielzeit > 5 * cMin - 5))
    {
        strMsg = QStringLiteral("Nicht gut! Deine Spielzeit ist beinahe abgelaufen.\nIn %1 min. wird das Spiel beendet!");
        mb5Flag = true;
    }

    if ((mVerbSpielzeit <= cMin) && (mVerbSpielzeit > cMin - 5))
    {
        strMsg = QStringLiteral("Nicht gut! Deine Spielzeit ist überschritten.\nIn %1 min. wird das Spiel beendet, besser Du speicherst jetzt!");
        mb1Flag = true;
    }

    if ((mb5Flag) || (mb1Flag))
    {
        mb5Flag = false;
        mb1Flag = false;

        showMsg(cTitel, strMsg.arg(QString::number(mVerbSpielzeit / cMin)), QMessageBox::Critical, true);
        Logger->LogMsg(strMsg.arg(QString::number(mVerbSpielzeit / cMin)));
    }
    
    if (mVerbSpielzeit <= 0)
    {
        strMsg = QStringLiteral("Spielzeit verbraucht, Programm '%1' beendet").arg(mProg);
        showMsg(cTitel, strMsg, QMessageBox::Critical, true);
        Logger->LogMsg(strMsg);

        strMsg = QStringLiteral("Spiel %1 am %2 um %3 beendet,\nverbleibende Spielzeit %4 min %5 sek").arg(mProg, ToDay.toString("dd.MM.yy"), Now.toString("hh.mm"), QString::number(mVerbSpielzeit / cMin), QString::number(mVerbSpielzeit % cMin));
        QString strHlp = QString("%1 min").arg(QString::number(miGespZeit / cMin));
        if (!mSendIt->MailIt("SpielEnde", Now.toString("hh.mm"), strHlp, strMsg))
            Logger->LogMsg("Keine SMTP Angaben");
        else
            Logger->LogMsg("Sende Mail");

        ProgrammKill();
    }
    writeSettings();

    // Backup, wenn die Spieleschleife nicht mehr aktiv ist, muß die ReadLoop aktiv sein!
    if (!mPlayTimer->isActive())
        mReadTimer->start(cReadDelay);
}
/************************ Ende slotRunReadLoop *****************************************************/

//*************************************************************************************************
// FunktionsName:   prepSettings
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
QString cPcZeit::adjTimeNow()
{
    QString strTime;
    mTNowBegin = QTime::currentTime();
    if ((mTNowBegin >= mTbegin) && (mTNowBegin < mTend))
    {
        // aktuelle Zeit innerhalb der erlaubten Zeit -> Zeit ungültig setzen
        mTNowBegin = QTime(25, 0, 0, 0);
    }
    else
    {
        strTime = "\nSpielt ab " + mTNowBegin.toString("hh.mm");
    }
    return strTime;
}
/************************ Ende slotRunReadLoop *****************************************************/

//*************************************************************************************************
// FunktionsName:   prepSettings
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cPcZeit::prepSettings()
{
    bool Ok;
    readSettings(); //werden nur hier gelesen, wenn ein Spiel läuft, gibts keine Updates! Ausser das Flag wurde gesetzt
    miGespZeit = mGespZeit.toInt(&Ok, 10) * cMin;
    miSpZeit = mSpZeit.toInt(&Ok, 10) * cMin;
    mTBakBegin = mTbegin;
    mTBakEnd = mTend;

    // Spielzeit mit den einmaligen Erweiterungen
    miSpZeit += miPlusZeit;
    //Wieviel Zeit ist denn noch
    mVerbSpielzeit = miSpZeit - miGespZeit;

    if (!mTNowBegin.isValid())
        return;

    mTBakBegin = mTNowBegin;
    if (mTNowBegin.addSecs((miSpZeit + cMin)) > mTBakEnd)
    {
        mTBakEnd = mTBakBegin;
        mTBakEnd = mTBakEnd.addSecs(miSpZeit + cMin);
    }
}
/************************ Ende prepSettings *****************************************************/

//*************************************************************************************************
// FunktionsName:   LogEntry
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cPcZeit::LogEntry()
{
    QMetaObject metaObject = cPcZeit::staticMetaObject;
    QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("WochenTag"));
    QString tag = metaEnum.valueToKey(mWochenTag);

    Logger->LogMsg(QStringLiteral("Auslesen der Einstellungen\nRahmenZeit am %1 von %2 bis %3, Spielzeit %4 min %5 sek\n")
        .arg(tag, mTBakBegin.toString("hh:mm"), mTBakEnd.toString("hh:mm"), QString::number(mVerbSpielzeit / cMin), QString::number(mVerbSpielzeit % cMin)));

    setTrayToolTip();
    slotShowZeit();
}
/************************ Ende LogEntry *****************************************************/

//*************************************************************************************************
// FunktionsName:   ProgrammKill
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cPcZeit::ProgrammKill()
{
    mWinApi->BeendeProgram(mPID);
    mpProg->setText("");

    mPlayTimer->stop();
    mReadTimer->start(cReadDelay);
}
/************************ Ende ProgrammKill *****************************************************/

//*************************************************************************************************
// FunktionsName:   creatTrayIcon
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cPcZeit::createTrayIcon()
{
    QMenu* trayIconMenu = new QMenu();
    QAction* pExit = new QAction(QIcon(":/images/Darth-Mauls-light-sabers-icon.png"), QStringLiteral("Beenden"), this);
    QObject::connect(pExit, SIGNAL(triggered()), this, SLOT(slotExit()));

    QAction* pSetup = new QAction(QIcon(":/images/R2-D2-icon.png"), QStringLiteral("Einstellungen"), this);
    QObject::connect(pSetup, SIGNAL(triggered()), this, SLOT(slotSetup()));

    QAction* pPlus5 = new QAction(QIcon(":/images/Millenium-Falcon-02-icon.png"), QStringLiteral(c5min), this);
    QObject::connect(pPlus5, SIGNAL(triggered()), this, SLOT(slotPlus()));

    QAction* pPlus15 = new QAction(QIcon(":/images/Imperial-Star-Destroyer-icon.png"), QStringLiteral(c15min), this);
    QObject::connect(pPlus15, SIGNAL(triggered()), this, SLOT(slotPlus()));

    QAction* pNow = new QAction(QIcon(":/images/C3PO-icon.png"), QStringLiteral("Spiele jetzt"), this);
    QObject::connect(pNow, SIGNAL(triggered()), this, SLOT(slotNow()));

    QAction* pTckt = new QAction(QIcon(":/images/Tie-Fighter-01-icon.png"), QStringLiteral("Ticket einlösen"), this);
    QObject::connect(pTckt, SIGNAL(triggered()), this, SLOT(slotTicket()));

    mpProg = new QAction(QIcon(":/images/Boba-Fett-icon.png"), QStringLiteral(""), this);
    QObject::connect(mpProg, SIGNAL(triggered()), this, SLOT(slotShowZeit()));

    QMenu* trayIconPapaMenu = new QMenu("Darth Vader Mode");
    trayIconPapaMenu->setIcon(QIcon(":/images/Darth-Vader-icon.png"));
    trayIconPapaMenu->addAction(pSetup);
    trayIconPapaMenu->addSeparator();
    trayIconPapaMenu->addAction(pPlus5);
    trayIconPapaMenu->addAction(pPlus15);
    trayIconPapaMenu->addAction(pNow);
    trayIconPapaMenu->addSeparator();
    trayIconPapaMenu->addAction(pExit);

    trayIconMenu->addMenu(trayIconPapaMenu);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(pTckt);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(mpProg);

    mTrayIcon = new QSystemTrayIcon(QIcon(":/images/PcZeitApp.png"));
    mTrayIcon->setContextMenu(trayIconMenu);
    mTrayIcon->setVisible(true);
    mTrayIcon->show();
    //Logger->LogMsg("\t\t\t\t\t\tDebug: Show TrayIcon (createTrayIcon)");
    
    setTrayToolTip();
}
/************************ Ende creatTrayIcon *****************************************************/

//*************************************************************************************************
// FunktionsName:   setTrayToolTip
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cPcZeit::setTrayToolTip()
{
    QString strMin = QString::number(mVerbSpielzeit / cMin);
    QString strSek = QString::number(mVerbSpielzeit % cMin);
    //QString strHlp = QDir::currentPath();
    QString strHlp = QCoreApplication::applicationDirPath();

    if (mTBakBegin.isValid())
        strHlp = QStringLiteral("verbleibende Spielzeit: %1min %2sek\nvon %3 bis %4 Uhr.\nLogPfad %5")
        .arg(strMin, strSek, mTBakBegin.toString("hh.mm"), mTBakEnd.toString("hh.mm"), strHlp);

    mTrayIcon->setToolTip(strHlp);
    mTrayIcon->show();
    //Logger->LogMsg("\t\t\t\t\t\tDebug: Show TrayIcon (setTrayToolTip)");
}
/************************ Ende setTrayToolTip *****************************************************/

//*************************************************************************************************
// FunktionsName:   readStyleSheet
/// \details        Liest die letzten Einstellungen der Fensterposition und Groeße. Stellt diese dann ein
/// \param          -
/// \return         -
//*************************************************************************************************
void cPcZeit::readStyleSheet()
{
    QSettings settings(qApp->organizationName(), qApp->applicationName());
    mStyleSheetFile = settings.value("style").toString();

#ifdef MAILSERVER
    QString mailServer = settings.value("mailServer").toString();
    QString mailPort = settings.value("mailPort").toString();
    mailServerPort = QString("%1:%2").arg(mailServer, mailPort);
    mailUser = settings.value("mailUser").toString();
    QByteArray data = settings.value("mailPwd").toByteArray();
    mailPwd = mPwd->DecryptPwd(data);
    mailEmpfanger = settings.value("mailEmpfanger").toString();
    mailCurlLog = settings.value("mailCurlLog", false).toBool();
    mailLastSend = settings.value("mailSend", true).toBool();
#endif
}
/************************ Ende readStyleSheet *****************************************************/

//*************************************************************************************************
// FunktionsName:   readSettings
/// \details        Liest die letzten Einstellungen der Fensterposition und Groeße. Stellt diese dann ein
/// \param          -
/// \return         -
//*************************************************************************************************
void cPcZeit::readSettings()
{
    QTime begin, end;
    mHeute = QDate::currentDate();
    QSettings settings(qApp->organizationName(), qApp->applicationName());

    mWochenTag = mHeute.dayOfWeek(); // 1 = Monday to 7 = Sunday
    switch (mWochenTag)
    {
        case Qt::Monday:
            begin = settings.value(cMoBeg, cDefBegin).toTime();
            end = settings.value(cMoEnd, cDefEnd).toTime();
            break;
        case Qt::Tuesday:
            begin = settings.value(cDiBeg, cDefBegin).toTime();
            end = settings.value(cDiEnd, cDefEnd).toTime();
            break;
        case Qt::Wednesday:
            begin = settings.value(cMiBeg, cDefBegin).toTime();
            end = settings.value(cMiEnd, cDefEnd).toTime();
            break;
        case Qt::Thursday:
            begin = settings.value(cDoBeg, cDefBegin).toTime();
            end = settings.value(cDoEnd, cDefEnd).toTime();
            break;
        case Qt::Friday:
            begin = settings.value(cFrBeg, cDefBegin).toTime();
            end = settings.value(cFrEnd, cDefEnd).toTime();
            break;
        case Qt::Saturday:
            begin = settings.value(cSaBeg, cDefBegin).toTime();
            end = settings.value(cSaEnd, cDefEnd).toTime();
            break;

        default: //Qt::Sunday
            begin = settings.value(cSoBeg, cDefBegin).toTime();
            end = settings.value(cSoEnd, cDefEnd).toTime();
            //end = QTime::fromString(settings.value(cSoEnd, cDefEnd).toString(), "hh:mm:ss");
            break;
    }

    QString GespZeit = settings.value("gespielteZeit", cDefGesp).toString();
    QString SpZeit = settings.value("Spielzeit", cDefZeit).toString();
    //QDate Spieltag = settings.value("Datum").toDate(); funktioniert nicht!
    QDate Spieltag = QDate::fromString(settings.value("Datum").toString(), "dd.MM.yyyy");
    QString Programs = settings.value("Programme").toString();

    // noch Ticket oder sonstige Pluszeit vorhanden
    if (miPlusZeit == 0)
        miPlusZeit = settings.value("PlusZeit", "0").toInt();
    
    mShowMsg = settings.value("BallonMsg").toInt();

    QString logMsg;
    // Parameter geändert, dann neu ins LogFile
    if (mTbegin != begin)
    {
        mTbegin = begin;
        mFirstLog = true;
        logMsg.append(QStringLiteral("Startzeit verändert\n"));
    }
    if (mTend != end)
    {
        mTend = end;
        mFirstLog = true;
        logMsg.append(QStringLiteral("Endezeit verändert\n"));
    }
    if (mGespZeit != GespZeit)
    {
        mGespZeit = GespZeit;
        mFirstLog = true;
        logMsg.append(QStringLiteral("gespielte Zeit verändert\n"));
    }
    if (mSpZeit != SpZeit)
    {
        mSpZeit = SpZeit;
        mFirstLog = true;
        logMsg.append(QStringLiteral("Spielzeit verändert\n"));
    }
    if (mBackSpieltag != Spieltag)
    {
        mBackSpieltag = Spieltag;
        mFirstLog = true;
        logMsg.append(QStringLiteral("Spieltag verändert\n"));
    }
    if (mBackPrograms != Programs)
    {
        mBackPrograms = Programs;
        mFirstLog = true;
        logMsg.append(QStringLiteral("Programme verändert\n"));
    }
    mPrograms = Programs.split(";");

#ifdef MAILSERVER
    QString bSend = " NICHT ";
    if (mailLastSend)
        bSend = " ";

    if (mFirstLog)
        logMsg.append(QStringLiteral("Letzte Mail konnte%1versendet werden!\n").arg(bSend));
#endif

    // Die Meldungen beim ersten start nicht mitloggen
    if (/*(!mFirstRun) &&*/ (!logMsg.isEmpty()))
        Logger->LogMsg(logMsg);
    //else
    //mFirstRun = false;

    if (Spieltag < mHeute)
    {
        Logger->LogMsg(QStringLiteral("neuer Spieltag"));

        mGespZeit = cDefGesp;
        // die neuen Daten schreiben sonst poppt der Ballon zyklisch auf
        settings.setValue("Datum", QDate::currentDate().toString("dd.MM.yyyy"));
        settings.setValue("gespielteZeit", mGespZeit);

        // Spielzeit wieder auf den eingestelleten Default Wert stellen
        mSpZeit = SpZeit;
        settings.setValue("Spielzeit", mSpZeit);

        // PlusZeit war gestern
        miPlusZeit = 0;
        settings.setValue("PlusZeit", miPlusZeit);
    }

    // ServerPfad
    mSrvLogPfad = settings.value("ServerPfad", "").toString();

    // Tickets
    mPwd->TicketCrypto(&mTickets, true);
 }
/************************ Ende readSettings ******************************************************/

//*************************************************************************************************
// FunktionsName:   writeSettings
/// \details        Schreibt die letzten Einstellungen 
/// \param          -
/// \return         -
//*************************************************************************************************
void cPcZeit::writeSettings()
{
    QSettings settings(qApp->organizationName(), qApp->applicationName());    
    //settings.setValue("Datum", QDate::currentDate().toString("dd.MM.yyyy"));
    settings.setValue("Datum", mHeute.toString("dd.MM.yyyy"));

    QString spZeit;
    if (miPlusZeit > 0)
    {
        settings.setValue("PlusZeit", miPlusZeit);
        spZeit = QString::number((miGespZeit / cMin));
        // Ticket eingelöst, erlaubte Spielzeit nochmal auslesen, verbleibende Spielzeit abziehen und zurückschreiben
        //spZeit = settings.value("Spielzeit", cDefZeit).toString();
    }
    else if(miGespZeit <= cMin)
        // weniger als 1 Minute gespielt
        spZeit = "0";

  /*  else if (miGespZeit > miSpZeit)
        // gespielte Zeit mit plus minuten versehen
        spZeit = QString::number((miSpZeit / cMin));*/

    else        
        // gespielte Zeit
        spZeit = QString::number((miGespZeit / cMin));

    settings.setValue("gespielteZeit", spZeit);
    settings.setValue("mailSend", mSendIt->SendStatus());
}
/************************ Ende writeSettings *****************************************************/

//*************************************************************************************************
// FunktionsName:   PwdAbfrage
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
bool cPcZeit::PwdAbfrage()
{
#ifndef NDEBUG
    return true;
#endif

    int retVal = mPwd->PwdAbfrage(cTitel);
    if (retVal == ERR_IGNORE)
        return false;
    else if (retVal == ERR_NOK)
    {
        mPwdCnt++;
        showMsg(cTitel, "Falsches Passwort", QMessageBox::Information);
        Logger->LogMsg(QStringLiteral("Falsches Passwort"));
        return false;
    }
    mPwdCnt = 0;
    Logger->LogMsg(QStringLiteral("Passwort korrekt"));
    return true;
}
/************************ Ende PwdAbfrage ********************************************************/

//*************************************************************************************************
// FunktionsName:   loadTheme
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cPcZeit::loadTheme()
{
    // Aussehen der Applikation
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QFile file(mStyleSheetFile);
    if (file.open(QFile::ReadOnly))
    {
        QString styleSheet = (file.readAll());
        qApp->setStyleSheet(styleSheet);
    }
}
/************************ Ende loadTheme ********************************************************/

//*************************************************************************************************
// FunktionsName:   showMsg
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cPcZeit::showMsg(QString strTitle, QString strMsg, QMessageBox::Icon Enm, bool pSpEnd)
{
    if ((mShowMsg & 1) == 1)
    {
        PopUpBox(strTitle, strMsg, Enm, pSpEnd);
        mButtNoRole->setVisible(false);
        mButtYesRole->setText("Ok");
    }

    if ((mShowMsg & 2) == 2)
        mTrayIcon->showMessage(strTitle, strMsg, static_cast<QSystemTrayIcon::MessageIcon>(Enm), 1000);
}
/************************ Ende showMsg ****************************************************/

//*************************************************************************************************
// FunktionsName:   PopUpBox
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cPcZeit::PopUpBox(QString strTitle, QString strMsg, QMessageBox::Icon Enm, bool pSpEnd)
{
    mMbox->blockSignals(false);
    mMbox->setWindowTitle(strTitle);
    mMbox->setText(strMsg);
    mMbox->setIcon(Enm);
    mOpacity = 0.0;
    mMbox->setWindowOpacity(mOpacity);
    mCntUp = true;    
    mMbox->setFocus();

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect scrRect = screen->availableGeometry(); 

    // Damit wird die Geometrie aktualisiert
    mMbox->show();

    if (pSpEnd)
    {
        mMbox->setStyleSheet("QMessageBox {background-color: rgb(204, 255, 0); border: 4px solid rgb(255, 0, 204); border-radius: 4px;}"
            "QMessageBox QAbstractButton {background-color: rgb(255, 0, 204);}"
            "QMessageBox QLabel {background-color: rgb(0, 204, 255);color: black;}");
        
        QSize QmSize = mMbox->size();
        // BildSchirm Mitte in X
        int xPos = (scrRect.x() + scrRect.width()) / 2 - QmSize.width() / 2;
        // BildSchirm Mitte in Y
        //int yPos = (scrRect.y() + scrRect.height()) / 2 - QmSize.height() / 2;
        int yPos = scrRect.y() + 0;
        mMbox->move(xPos, yPos);
    }
    else
    {
        mMbox->setStyleSheet("");
        mMbox->move(scrRect.x(), scrRect.y() + 10);
    }
    mMbox->raise();
    mFadeInTimer->start(cFadeIn);
    QSize QmSize = mMbox->size();
}
/************************ Ende PopUpBox ****************************************************/

//*************************************************************************************************
// FunktionsName:   slotReminder
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cPcZeit::slotReminder()
{
    mReminderTimer->stop();
    QString msg = QStringLiteral("Internet Zeit mit verlängert?");
    PopUpBox(cTitel, msg, QMessageBox::Question, false);
}
/************************ Ende slotFadeIn ****************************************************/

//*************************************************************************************************
// FunktionsName:   slotFadeIn
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cPcZeit::slotFadeIn()
{
    mOpacity += cFadeStep;
    mMbox->setWindowOpacity(mOpacity);

    if (mOpacity > 1)
    {
        mFadeInTimer->stop();
        mShowTimer->start(cShow);
    }
    mMbox->raise();
}
/************************ Ende slotFadeIn ****************************************************/

//*************************************************************************************************
// FunktionsName:   slotShow
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cPcZeit::slotShow()
{
    mShowTimer->stop();
    mFadeOutTimer->start(cFadeOut);
    mMbox->raise();
}
/************************ Ende slotShow ****************************************************/

//*************************************************************************************************
// FunktionsName:   slotRunReadLoop
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cPcZeit::slotFadeOut()
{    
    mOpacity -= cFadeStep;
    mMbox->setWindowOpacity(mOpacity);
    
    if (mOpacity < 0)
    {
        mFadeOutTimer->stop();
        slotMsgBoxbuttonClicked(NULL);
    }
    mMbox->raise();
}
/************************ Ende slotFadeOut ****************************************************/

//*************************************************************************************************
// FunktionsName:   slotRunReadLoop
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cPcZeit::slotMsgBoxbuttonClicked(QAbstractButton* button)
{
    mMbox->blockSignals(true);
    mFadeInTimer->stop();
    mShowTimer->stop();
    mFadeOutTimer->stop();
        
    if ((mButtYesRole == button) && (button->text() == "Ja"))
        emit(sigTicket());

    mMbox->close();
}
/************************ Ende slotRunReadLoop ****************************************************/

//*************************************************************************************************
// FunktionsName:   ScreenChange
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
bool cPcZeit::ScreenChange()
{
    bool retVal = true;
    QPixmap originalPixmap = QPixmap();
    QScreen *screen = QGuiApplication::primaryScreen();

    if (!screen)
        return retVal;
    
    originalPixmap = screen->grabWindow(0);
    QRgb rgba = (originalPixmap.toImage().pixel(200, 200) + 
                    originalPixmap.toImage().pixel(210, 200) + 
                    originalPixmap.toImage().pixel(220, 200) + 
                    originalPixmap.toImage().pixel(230, 200)
                 ) / 4;

    if (rgba == mRGB)
        retVal = false;
    else
        mRGB = rgba;
 
    return retVal;
}
/************************ Ende ScreenChange ****************************************************/

/************************ Ende cPcZeit ****************************************************/


