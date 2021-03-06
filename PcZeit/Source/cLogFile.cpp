/*< Jungheinrich > ********************************************************************************/
/*!
*	\brief      DriveFilter
*	\details    Filtert die ungültigen entfernbaren Laufwerke und Dateien ausd er Liste
*
*	\file		cLogFile.cpp
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
cLogFile::cLogFile(QString LogFile)
{
    mLog.setFileName(LogFile);
    mbFileOpen = mLog.open(QIODevice::Append | QIODevice::Text);
}
/************************ Ende Konstruktor *****************************************************/

//*************************************************************************************************
// FunktionsName:   Destruktor
/// \details        -
/// \param[in]      -
/// \return         -
//*************************************************************************************************
cLogFile::~cLogFile()
{
    mLog.close();
}
/************************ Ende Konstruktor *****************************************************/

//*************************************************************************************************
// FunktionsName:   LogMsg
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cLogFile::LogMsg(QString Msg)
{
    QTextStream out(&mLog);
    out << Msg << "\n";
    qDebug() << Msg;
}
/************************ Ende LogMsg ****************************************************/

//*************************************************************************************************
// FunktionsName:   SrvLog
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void cLogFile::SrvLog(QString SrvPfd, QString Msg)
{
    if (SrvPfd.isEmpty())
        return;

    QFile SrvLog(QString("%1/%2").arg(SrvPfd, Msg));
    if (SrvLog.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << QString("%1/%2").arg(SrvPfd, Msg);
        SrvLog.close();
    }
}
/************************ Ende SrvLog ****************************************************/

/************************ Ende cLogFile.cpp ****************************************************/