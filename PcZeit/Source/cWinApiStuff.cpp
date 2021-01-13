/*<  > ********************************************************************************/
/*!
*	\brief      Main, die Mutter aller Dateien
*	\details
*
*	\file		cWinApiStuff.cpp
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
* cWinApiStuff.cpp
*/

/**************************************************************************************************
* Includes
**************************************************************************************************/
#include <qt_windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include "cWinApiStuff.h"
#include "cLogFile.h"

#pragma comment(lib, "advapi32.lib")
/**************************************************************************************************
* Defines
**************************************************************************************************/
#define cTitel "Spielzeit Kontrolle"
#define cDelay 5000
#define cMin 60

/**************************************************************************************************
* Variablen
**************************************************************************************************/

/**************************************************************************************************
* Funktionen
**************************************************************************************************/
static cCallBackHandler* pThis;
static LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam);

//*************************************************************************************************
// FunktionsName:   CALLBACK MouseHookProc
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        if (wParam == WM_MOUSEMOVE)
            pThis->mbMo = true;

        if (wParam == WM_KEYUP)
            pThis->mbKb = true;
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}
/************************ Ende CALLBACK MouseHookProc *******************************************************/

//*************************************************************************************************
// FunktionsName:   Konstruktor
/// \details        Einsprung ins Programm
/// \param[in]      -
/// \return         -
//*************************************************************************************************
cCallBackHandler::cCallBackHandler()
{
    //LogFile
    qDebug() << "Konstruktor cCallBackHandler";
    mbMo = false;
    mbKb = false;

    pThis = this;

    mRunHookTimer = new QTimer(this);
    QObject::connect(mRunHookTimer, SIGNAL(timeout()), this, SLOT(slotRunHooks()));
    mRunHookTimer->start(2000);

#ifdef NDEBUG
    kbhook = SetWindowsHookEx(WH_KEYBOARD_LL, HookProc, NULL, 0);
    mousehook = SetWindowsHookEx(WH_MOUSE_LL, HookProc, NULL, 0);
#endif
}
/************************ Ende quit *****************************************************/

//*************************************************************************************************
// FunktionsName:   Destruktor
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
cCallBackHandler::~cCallBackHandler()
{
    mRunHookTimer->stop();
    delete mRunHookTimer;
    pThis = NULL;

    UnhookWindowsHookEx(kbhook);
    UnhookWindowsHookEx(mousehook);
}
/************************ Ende quit *****************************************************/

//*************************************************************************************************
// FunktionsName:   slotRunMainLoop
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cCallBackHandler::slotRunHooks()
{
    emit sigMouseActive(mbMo);
    emit sigKeyBoardActive(mbKb);
    mbMo = false;
    mbKb = false;
}
/************************ Ende quit *****************************************************/

//*************************************************************************************************
// FunktionsName:   Konstruktor
/// \details        Einsprung ins Programm
/// \param[in]      -
/// \return         -
//*************************************************************************************************
cWinApiStuff::cWinApiStuff(QStringList* pPrograms, cLogFile* pLogger) :
    mPrograms(pPrograms),
    mLogger(pLogger)
{
    mbMouseMove = false;
    mbKeyBoard = false;
    
    cCall = new cCallBackHandler();
    cCall->moveToThread(&cCallBackHandlerThread);

    connect(&cCallBackHandlerThread, &QThread::finished, cCall, &QObject::deleteLater);

    QObject::connect(cCall, SIGNAL(sigMouseActive(bool)), this, SLOT(slotMouseActive(bool)));
    QObject::connect(cCall, SIGNAL(sigKeyBoardActive(bool)), this, SLOT(slotKeyBoardActive(bool)));

    cCallBackHandlerThread.start();
}
/************************ Ende quit *****************************************************/

//*************************************************************************************************
// FunktionsName:   Destruktor
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
cWinApiStuff::~cWinApiStuff()
{
    cCallBackHandlerThread.quit();
    cCallBackHandlerThread.wait();
    cCall = NULL;
}
/************************ Ende quit *****************************************************/

//*************************************************************************************************
// FunktionsName:   slotMouseActive
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cWinApiStuff::slotMouseActive(bool bActive)
{
    if (bActive)
        mbMouseMove = true;
}
/************************ Ende slotMouseActive *****************************************************/

//*************************************************************************************************
// FunktionsName:   slotKeyBoardActive
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cWinApiStuff::slotKeyBoardActive(bool bActive)
{
    if (bActive)
        mbKeyBoard = true;
}
/************************ Ende slotKeyBoardActive *****************************************************/

//*************************************************************************************************
// FunktionsName:   CheckRunnigProgram
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
bool cWinApiStuff::AktivesFenster(QString strProg)
{
    TCHAR buff[255];
    QString strActive;

    GetWindowText(GetForegroundWindow(), (LPWSTR)buff, 254);
    strActive = QString::fromWCharArray(buff);

    if (strActive == strProg)
        return true;

    return false;
}
/************************ Ende PwdAbfrage ****************************************************/

//*************************************************************************************************
// FunktionsName:   CheckRunnigProgram
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
QString cWinApiStuff::CheckRunnigProgram(DWORD* processID)
{
    QString rProg = NULL;
    QString strRegEx = NULL;
    QStringList lstProc = { "javaw", "java" };
    int lstCnt = 0;
    bool finished = false;

    // Get the list of process identifiers.
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        return rProg;
    
    // Alle FensterTitel holen
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(this));
    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);
    do
    {   // Print the name and process identifier for each process.
        for (i = 0; i < cProcesses; i++)
        {
            if (aProcesses[i] == 0)
                continue;

            rProg = matchProcessName(aProcesses[i], *mPrograms, strRegEx);
            if (rProg.isEmpty())
                continue;

            *processID = aProcesses[i];
            break;
        }
        if ((rProg.isEmpty()) && (lstCnt < lstProc.size()))
            strRegEx = lstProc[lstCnt++];
        else
            finished = true;
    } 
    while (!finished);

    return rProg;
}
/************************ Ende CheckRunnigProgram ****************************************************/

//*************************************************************************************************
// FunktionsName:   matchProcessName
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
QString cWinApiStuff::matchProcessName(DWORD processID, QStringList processName, QString strReg)
{
    bool bJava = false;
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
    TCHAR lpszPrivilege[MAX_PATH] = TEXT("SeDebugPrivilege");
        
    // Get a handle to the process.
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (NULL == hProcess)
        return "";

    HANDLE hToken;
    // Rechte auf 64Bit    
    if (!OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        SetPrivilege(hToken, (LPCTSTR)&lpszPrivilege, true);
    
    // FilePfad holen
    int nChars = GetProcessImageFileName(hProcess, szProcessName, sizeof(szProcessName));

    // Compare process name with your string
    QString matchFound;
    QString strProcessName = QString::fromWCharArray(szProcessName);
    if (strProcessName == "<unknown>")
        return "";

    // Den ganzen FilePfad aufdröseln
    QFileInfo Info(strProcessName);
    strProcessName = Info.fileName();
    //mLogger->LogMsg(QStringLiteral("ProzessName %1").arg(strProcessName));

    foreach(QString prcName, processName)
    {
        if (!strReg.isEmpty())
        {
            QRegExp rx("(" + strReg + ")", Qt::CaseInsensitive);
            if (rx.indexIn(strProcessName) != -1)
                bJava = true;
        }
        if ((strProcessName.toLower() != prcName.toLower()) && (!bJava))
            continue;

        foreach(QStringList ProgHandle, mRunProg)
        {
            QString strPID = ProgHandle[0];
            QString WinTitle = ProgHandle[1];

            if (strPID == QString::number(processID))
            {
                matchFound = WinTitle;
                break;
            }
        }
        break;
    }
    // Release the handle to the process.    
    CloseHandle(hProcess);

    return matchFound;
}
/************************ Ende matchProcessName ****************************************************/

//*************************************************************************************************
// FunktionsName:   CALLBACK EnumWindowsProc
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
BOOL CALLBACK EnumWindowsProc(HWND hWnd, long lParam)
{
    TCHAR title[255];
    DWORD lpdwProcessId;
    cWinApiStuff *lpThis = reinterpret_cast<cWinApiStuff*>(lParam);

    if (IsWindowVisible(hWnd)) 
    {
        GetWindowThreadProcessId(hWnd, &lpdwProcessId);
        GetWindowText(hWnd, title, sizeof(title));
        QString Title = QString::fromWCharArray(title);
        if (!Title.isEmpty())
        {
            int Hwnd = reinterpret_cast<int>(hWnd);
            QStringList Window(QString::number(lpdwProcessId));
            Window.append(Title);
            lpThis->mRunProg.append(Window);
        }
    }
    return TRUE;
}
/************************ Ende CALLBACK EnumWindowsProc ****************************************************/

//*************************************************************************************************
// FunktionsName:   BeendeProgram
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
bool cWinApiStuff::BeendeProgram(DWORD dwProcessId)
{
    DWORD dwDesiredAccess = PROCESS_TERMINATE;
    BOOL  bInheritHandle = FALSE;
    UINT uExitCode = 0;
    HANDLE hProcess = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
    if (hProcess == NULL)
        return FALSE;

    BOOL result = TerminateProcess(hProcess, uExitCode);
    CloseHandle(hProcess);

    return result;
}
/************************ Ende BeendeProgram ****************************************************/

//*************************************************************************************************
// FunktionsName:   SetPrivilege
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
bool cWinApiStuff::SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
// access token handle, name of privilege to enable/disable, to enable or disable privilege  
{
    TOKEN_PRIVILEGES tp;
    LUID luid;

    if (!LookupPrivilegeValue(NULL, lpszPrivilege, &luid))
    //lookup privilege on local system, privilege to lookup, receives LUID of privilege
    {
        mLogger->LogMsg(QStringLiteral("LookupPrivilegeValue error: %1").arg(GetLastError()));
        return FALSE;
    }
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    if (bEnablePrivilege)
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    else
        tp.Privileges[0].Attributes = 0;

    // Enable the privilege or disable all privileges.
    if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
    {
        mLogger->LogMsg(QStringLiteral("AdjustTokenPrivileges error: %1").arg(GetLastError()));
        return FALSE;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
    {
        mLogger->LogMsg(QStringLiteral("The token does not have the specified privilege."));
        return FALSE;
    }

    return TRUE;
}
/************************ Ende SetPrivilege ****************************************************/

/************************ Ende cWinApiStuff ****************************************************/


