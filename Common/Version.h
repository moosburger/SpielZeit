/*< Jungheinrich > ********************************************************************************/
/*!
*	\brief      VersionsHeader beinhaltet die aktuelle BuildVersion
*	\details
*
*	\file		Version.h
*
*	\copyright  (C) 2014 Jungheinrich GmbH, All rights reserved.
*	\date       Erstellt am: 05.08.2014
*	\author     Gerhard Prexl
*
*	\version    1.0  -  05.08.2014
*/
/*< History > *************************************************************************************
*	Version     Datum       Kuerzel      Ticket#     Beschreibung
*   1.0         05.08.2014  GP           -------     Ersterstellung
* </ History ></ Jungheinrich > ******************************************************************/

/**
* \addtogroup Modules-HandCoded
* \addtogroup Main
* \ingroup Modules-HandCoded
* Version.h
*/

#ifndef VERSION_H
#define VERSION_H

/**************************************************************************************************
* Includes
**************************************************************************************************/

/**************************************************************************************************
* Defines
**************************************************************************************************/
#define MAILSERVER

#define cTitel "Spielzeit Kontrolle"
#define cDefBegin       "08:00:00"
#define cDefEnd         "20:00:00"
#define cDefZeit        "30"
#define cDefGesp        "0"
#define cMoBeg          "01MoBeg"
#define cDiBeg          "02DiBeg"
#define cMiBeg          "03MiBeg"
#define cDoBeg          "04DoBeg"
#define cFrBeg          "05FrBeg"
#define cSaBeg          "06SaBeg"
#define cSoBeg          "07SoBeg"
#define cMoEnd          "08MoEnd"
#define cDiEnd          "09DiEnd"
#define cMiEnd          "10MiEnd"
#define cDoEnd          "11DoEnd"
#define cFrEnd          "12FrEnd"
#define cSaEnd          "13SaEnd"
#define cSoEnd          "14SoEnd"

#define APPNAME		    "Parameter"                         /*!< ApplikationsName fuer den Build */
#define FIRMA		    "Spielzeit"                         /*!< FirmenName fuer den Build */

#define FILEVER         1,1,6,0                             /*!< DateiVersion fuer den Build */
#define PRODUCTVER      1,1,6,0                             /*!< ProduktVersion fuer den Build */
#define STRFILEVER      "1.1.6.0\0"                         /*!< DateiVersion als String fuer den Build */
#define STRPRODUCTVER   "1.1.6.0\0"                         /*!< ProduktVersion als String fuer den Build */

#endif
/************************ Ende mainwindow.h ******************************************************/