/*<  > ********************************************************************************/
/*!
*	\brief      Headerdatei zu MainWindow.cpp
*	\details
*
*	\file		mainwindow.h
*
*	\copyright  (C) 2014 , All rights reserved.
*	\date       Erstellt am: 05.08.2014
*	\author     Gerhard Prexl
*
*	\version    1.0  -  05.08.2014
*/
/*< History > *************************************************************************************
*	Version     Datum       Kuerzel      Ticket#     Beschreibung
*   1.0         05.08.2014  GP           -------     Ersterstellung
* </ History ></  > ******************************************************************/

/**
* \addtogroup Modules-HandCoded
* \addtogroup Main
* \ingroup Modules-HandCoded
* mainwindow.h
*/

#ifndef QMAINPANEL_H
#define QMAINPANEL_H

/**************************************************************************************************
* Includes
**************************************************************************************************/
#include <QtWidgets>
#include <QtGui>
#include <QtCore>

#include "QWinWidget.h"
#include "Version.h"
/**************************************************************************************************
* Defines
**************************************************************************************************/
QT_BEGIN_NAMESPACE
class BorderlessWindow;
class cPwdHandler;
QT_END_NAMESPACE

#define cWidth 457 //515
#define cHeightAdd 30
#define cProgramCnt 7

#ifdef MAILSERVER
    #define cHeight 530
#else
    #define cHeight 470
#endif
//*************************************************************************************************
// KlassenName:     cMainWindow
/// \details        QMainWindow wird fuer das Gui Handling verwendet. Alles rund um die Oberflaeche und deren Events wird hier abgehandelt.
//*************************************************************************************************
class QMainPanel : public QWinWidget 
{
  Q_OBJECT

public:
    QMainPanel(HWND hWnd, BorderlessWindow* pParent);
#if QT_VERSION >= 0x050000
  bool nativeEvent(const QByteArray &, void *msg, long *result);
#else
  bool winEvent(MSG *message, long *result);
#endif
  void mousePressEvent( QMouseEvent *event );

  enum WochenTag {Mo = 1, Di = 2, Mi = 3, Do = 4, Fr = 5, Sa = 6, So = 7};
  Q_ENUMS(WochenTag);

public slots:
    void pushButtonMinimizeClicked();
    void pushButtonMaximizeClicked();
    void pushButtonCloseClicked();
    //*********************************************************************************
    // Eigene Slots (nicht BorderLess benötigte)
    //*********************************************************************************
    void slotSafeIt(bool);
    void slotExit(bool);
    void slotDelIt(bool);
    void slotEditFinished(); 
    void slotSrvPthFinished();
    void slotComboBox(QString);
    void slotBallonChanged(int);
    void slotPopUpchanged(int);
    void slotPwdChange(bool);
    void slotGenTicket(bool);
    void slotDruckeTicket(bool);
	
signals:
    //*********************************************************************************
    // Eigene Signale (nicht BorderLess benötigte)
    //*********************************************************************************

private slots:
    //*********************************************************************************
    // Eigene Slots (nicht BorderLess benötigte)
    //*********************************************************************************

private:
    //*********************************************************************************
    // BorderLess Funktionen
    //*********************************************************************************
    HWND windowHandle;
    void loadTheme();
    void readSettings();
    void writeSettings();

    QWidget* createCentralWidget(QWidget* centralWidget);
    void qDestructor();

    //*********************************************************************************
    // BorderLess Variablen
    //*********************************************************************************
    QWidget* createStatusBar();
    QWidget* createTitleBar(const QString &Title);
    QScrollArea* createScrollBar();
    BorderlessWindow* mParent;
    QString mStyleSheetFile;

    //*********************************************************************************
    // Eigene Funktionen (nicht BorderLess benötigte)
    //*********************************************************************************
    void Init();
    QGroupBox* SpielZeit();
    QGroupBox* WochenZeit();
    QGroupBox* Programme();
    QGroupBox* showTicket();
    QGroupBox* Einstellungen();
    QGroupBox* MailServer();
    QBoxLayout* GetHBox(QLabel* pRahmen, QLabel* pBis);
    QBoxLayout* GetHBox(QLabel* pTag, QTimeEdit* pVon, QTimeEdit* pBis);
    void setParameter();
    bool SrvPthValid();

    //*********************************************************************************
    // Eigene Variablen (nicht BorderLess benötigte)
    //*********************************************************************************
    // die Zeiger auf die verwendeten Klassen
    cPwdHandler* mPwd;
    QLineEdit* mSpielZeit;
    QLineEdit* mGespielteZeit;
    QBoxLayout* mPrgmVbox;
    QComboBox* mStyle; 
    QLineEdit* mServerPath;
    QLineEdit* mSmtpServer;
    QLineEdit* mSmtpPort;
    //QComboBox* mSmtpSecurity;
    QLineEdit* mMailUser;
    QLineEdit* mMailPwd;
    QLineEdit* mRecipient;

    QList<QTimeEdit*> mBegin;
    QList<QTimeEdit*> mEnd;    
    QList<QTime> mTbegin;
    QList<QTime> mTend;
    QList<QLabel*> mTckt;
    QStringList mWBeg;
    QStringList mWEnd;
    QPushButton* mDruckButton;
    QPushButton* mGenButton;

    QString mSpZeit;
    QString mGespZeit;
    QString mSrvLogPfad;
    QStringList mPrograms;
    QStringList mTickets;
    QString mailServer;
    QString mailPort;
    QString mailUser;
    QString mailPwd;
    QString mailEmpfanger;

    bool mailCurlLog;
    int mShowMsg;
};
/************************ Ende Klasse cMainWindow *************************************************/
#endif // QMAINPANEL_H
/************************ Ende mainwindow.h ******************************************************/
