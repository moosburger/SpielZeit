/*< Jungheinrich > ********************************************************************************/
/*!
*	\brief      cNativeEvent
*	\details    
*
*	\file		cNativeEvent.cpp
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
* \addtogroup Event
* \ingroup Modules-HandCoded
* cNativeEvent.cpp
*/

/**************************************************************************************************
* Includes
**************************************************************************************************/
#include <QtWidgets>
#include <qt_windows.h>

#include "cNativeEvent.h"
#include "cLogFile.h"

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
/*cNativeEvent::cNativeEvent(QObject * parent) : QObject(parent)
{
}*/
/************************ Ende Konstruktor *****************************************************/

//*************************************************************************************************
// FunktionsName:   Destruktor
/// \details        -
/// \param[in]      -
/// \return         -
//*************************************************************************************************
/*cNativeEvent::~cNativeEvent()
{
}*/
/************************ Ende Konstruktor *****************************************************/

//*************************************************************************************************
// FunktionsName:   Destruktor
/// \details        -
/// \param[in]      -
/// \return         -
//*************************************************************************************************
void cNativeEvent::registerLogger(cLogFile* pLogger)
{
    mIsSuspend = false;
    mIsResume = true;
    mLogger = pLogger;
}
/************************ Ende Konstruktor *****************************************************/

//*************************************************************************************************
// FunktionsName:   Events
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
bool cNativeEvent::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    PMSG msg = static_cast<PMSG>(message);

    if (eventType == "windows_generic_MSG" && msg->message == WM_POWERBROADCAST)
    {
        SetThreadExecutionState(ES_CONTINUOUS);
        //mLogger->LogMsg(QStringLiteral("PowerBroadCast\nmIsSuspend = %1\nmIsResume = %2\n").arg(QString::number(mIsSuspend), QString::number(mIsResume)));
        if ((msg->wParam == PBT_APMSUSPEND) && (!mIsSuspend))
        {
            SetThreadExecutionState(ES_CONTINUOUS);
            mLogger->LogMsg(QStringLiteral("%1\nGehe schlafen am %2 um %3\n%4\n")
                .arg(cSeperator, QDate::currentDate().toString("dddd dd.MM.yyyy"), QTime::currentTime().toString("hh.mm"), cSeperator));

            SetThreadExecutionState(ES_CONTINUOUS);
            emit(sigGoingSuspend());

            SetThreadExecutionState(ES_CONTINUOUS);
            mIsSuspend = true;
            mIsResume = false;
        }
        if ((msg->wParam == PBT_APMRESUMESUSPEND) && (!mIsResume))
        {
            mLogger->LogMsg(QStringLiteral("%1\nAufwachen am %2 um %3\n%4")
                .arg(cSeperator, QDate::currentDate().toString("dddd dd.MM.yyyy"), QTime::currentTime().toString("hh.mm"), cSeperator));

            emit(sigResume());

            mIsSuspend = false;
            mIsResume = true;
        }
    }
    return false;
}
/************************ Ende  ****************************************************/

/************************ Ende cLogFile.cpp ****************************************************/