/*< Jungheinrich > ********************************************************************************/
/*!
*	\brief      Headerdatei zu DriveFilter.cpp
*	\details
*
*	\file		cLogFile.h
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
* cLogFile.h
*/

#ifndef LOGFILE_H
#define LOGFILE_H

/**************************************************************************************************
* Includes
**************************************************************************************************/
#include <QtWidgets>

/**************************************************************************************************
* Defines
**************************************************************************************************/
#define cSeperator "####################################################################################################################################################"

//*************************************************************************************************
// KlassenName:     cLogFile
/// \details        -
//*************************************************************************************************

class cLogFile : public QObject
{
    Q_OBJECT

public:
    cLogFile(QString file);
    virtual ~cLogFile();

    void LogMsg(QString Msg);
    void SrvLog(QString SrvPfd, QString Msg);

private:
    QFile mLog;
    bool mbFileOpen;
};
/************************ Ende Klasse cLogFile *************************************************/
#endif 
/************************ Ende cLogFile.h ******************************************************/