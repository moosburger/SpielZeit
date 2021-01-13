/*<  > ********************************************************************************/
/*!
*	\brief      Main, die Mutter aller Dateien
*	\details
*
*	\file		cWinApiStuff.h
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
* cWinApiStuff.h
*/

#ifndef WINAPISTUFF_H
#define WINAPISTUFF_H

/**************************************************************************************************
* Includes
**************************************************************************************************/
#include <QtWidgets>

/**************************************************************************************************
* Defines
**************************************************************************************************/
QT_BEGIN_NAMESPACE
//class cPcZeit;
class cLogFile;
QT_END_NAMESPACE

//*************************************************************************************************
// KlassenName: 	cCallBackHandler als Worker
/// \details        -
//*************************************************************************************************
class cCallBackHandler : public QObject
{
    Q_OBJECT

public:
    cCallBackHandler();
    virtual ~cCallBackHandler();

    bool mbMo;
    bool mbKb;

signals:
    void sigMouseActive(bool);
    void sigKeyBoardActive(bool);

public slots:
    void slotRunHooks();

private:
    QTimer* mRunHookTimer;
    HHOOK kbhook;
    HHOOK mousehook;
};

//*************************************************************************************************
// KlassenName: 	cWinApiStuff
/// \details        -
//*************************************************************************************************
class cWinApiStuff : public QObject
{
    Q_OBJECT
        QThread cCallBackHandlerThread;

public:
    cWinApiStuff(QStringList* pPrograms, cLogFile* pLogger);
    virtual ~cWinApiStuff();

    friend BOOL CALLBACK EnumWindowsProc(HWND hWnd, long lParam);

    bool AktivesFenster(QString strProg);
    QString matchProcessName(DWORD processID, QStringList processName, QString strReg = NULL);
    QString CheckRunnigProgram(DWORD* processID);
    bool BeendeProgram(DWORD dwProcessId);

    bool mbMouseMove;
    bool mbKeyBoard;

public slots :
    void slotMouseActive(bool);
    void slotKeyBoardActive(bool);

private:
    bool SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);

    cCallBackHandler* cCall;
    cLogFile* mLogger;
    QStringList* mPrograms;
    QVector<QStringList> mRunProg;
    QString mProg;
};

#endif 
