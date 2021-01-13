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

#ifndef PCZEIT_H
#define PCZEIT_H

/**************************************************************************************************
* Includes
**************************************************************************************************/
#include <QtWidgets>
#include <QtGui>
#include <QApplication>
#include <QTimer> 
#include <QSystemTrayIcon>
#include <QMainWindow>
#include "Version.h"
#include "cNativeEvent.h"

/**************************************************************************************************
* Defines
**************************************************************************************************/
QT_BEGIN_NAMESPACE
class cWinApiStuff;
class cLogFile;
class cSendMail;
class cPwdHandler;
QT_END_NAMESPACE

//*************************************************************************************************
// KlassenName: 	cPcZeit
/// \details        -
//*************************************************************************************************
class cPcZeit : public QMainWindow
{
    Q_OBJECT

public:
    cPcZeit(QObject* parent = 0);
    virtual ~cPcZeit();

    enum WochenTag { Montag = 1, Dienstag = 2, Mittwoch = 3, Donnerstag = 4, Freitag = 5, Samstag = 6, Sonntag = 7 };
    Q_ENUMS(WochenTag);

    cNativeEvent mFilter;
    //bool event(QEvent* e); Damit wird dann das Pwd nicht mehr ausgelesen...
    
protected:
    bool eventFilter(QObject *obj, QEvent *ev);
    
signals:
    void finished(); 		/*!< Signal wird zum beenden gesendet */
    void sigTicket();
            
public slots:
    void slotRunMainLoop();
    void slotExit();
    void slotPlus();
    void slotNow();
    void slotSetup();
    void slotShowZeit();
    void slotRunReadLoop();
    void slotRunPlayLoop();
    void slotAboutToQuit();
    void slotFadeIn();
    void slotFadeOut();
    void slotShow();
    void slotMsgBoxbuttonClicked(QAbstractButton* button);
    void slotTicket();
    void slotReminder();
    void slotWriteSettings();
    void slotReadSettings();

private:
    // Funktionen 
    void loadTheme();
    void createTrayIcon();
    void prepSettings();
    void LogEntry();
    void readSettings();
    void readStyleSheet();
    void writeSettings();
    void setTrayToolTip();
    void PopUpBox(QString strTitle, QString strMsg, QMessageBox::Icon Enm, bool bSpEnd);
    bool PwdAbfrage();
    bool ScreenChange();
    void ProgrammKill();
    void showMsg(QString strTitle, QString strMsg, QMessageBox::Icon Enm, bool pSpEnd = false);
    QString adjTimeNow();

    cWinApiStuff* mWinApi;
    cLogFile* Logger;
    cSendMail* mSendIt;
    cPwdHandler* mPwd;
    QSystemTrayIcon* mTrayIcon; 

    QAction* mpProg;
    QTimer* mReadTimer;
    QTimer* mPlayTimer;
    QTimer* mFadeInTimer;
    QTimer* mShowTimer;
    QTimer* mFadeOutTimer;
    QTimer* mReminderTimer;
    QMessageBox* mMbox; 
    QAbstractButton* mButtYesRole;
    QAbstractButton* mButtNoRole;
    qreal mOpacity;
    bool mCntUp;

    QTime mTbegin;
    QTime mTend;
    QTime mTBakBegin;
    QTime mTBakEnd;
    QTime mTNowBegin;

    QString mSpZeit;
    QString mGespZeit;
    QStringList mPrograms;
    QString mProg; 
    QDate mBackSpieltag;
    QDate mHeute;
    QString mBackPrograms;
    QStringList mTickets;

    int mVerbSpielzeit;
    int miGespZeit;
    int miSpZeit;
    int miPlusZeit;

    QRgb mRGB;
    bool mFirstLog;
    //bool mFirstRun;
    //int mTicket;
    bool mb5Flag;
    bool mb1Flag;
    bool mbRegChanged;
    DWORD mPID;
    QString mStyleSheetFile;
    int mPwdCnt;
    int mWochenTag;
    int mShowMsg;
    QString mSrvLogPfad;
    QString mailServerPort;
    QString mailUser;
    QString mailPwd;
    QString mailEmpfanger;
    bool mailCurlLog;
    bool mailLastSend;
};

#endif // PCZEIT_H
