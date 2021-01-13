/*< Jungheinrich > ********************************************************************************/
/*!
*	\brief      Main, die Mutter aller Dateien
*	\details
*
*	\file		main.cpp
*
*	\copyright  (C) 2014 Jungheinrich GmbH, All rights reserved.
*	\date       Erstellt am: 05.08.2014
*	\author     Gerhard Prexl
*
*	\version    1.0  -  05.08.2014
*/
/*< History > *************************************************************************************
*   Version     Datum       Kuerzel      Ticket#     Beschreibung
*   1.0         05.08.2014  GP           -------     Ersterstellung
* </ History ></ Jungheinrich > ******************************************************************/

/**
* \addtogroup Modules-HandCoded
* \addtogroup Main
* \ingroup Modules-HandCoded
* main.cpp
*/

/**************************************************************************************************
* Includes
**************************************************************************************************/
#include <QFile>
#include <QIcon>
#include "QMainWindow.h"
#include "Version.h"

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
// FunktionsName:   main
/// \details        Einsprung ins Programm
/// \param[in]      argc
/// \param[in]      argv
/// \return         int
//*************************************************************************************************
int main( int argc, char *argv[] ) 
{
  QApplication *application = new QApplication( argc, argv );

  // Die ResourcenDatei mit den Icon Infos
  Q_INIT_RESOURCE(Resources);

  QString WinTitle = QString(APPNAME);// +" " + QString(STRPRODUCTVER);
  qApp->setOrganizationName(FIRMA);
  qApp->setApplicationName(WinTitle);
  qApp->setWindowIcon(QIcon(":/images/SetupApp.ico"));
  qApp->setQuitOnLastWindowClosed(false);

  // Font
  QFont mainFont = qApp->font();
  mainFont.setStyleStrategy( QFont::PreferAntialias );
  qApp->setFont(mainFont);

  // Background color
  // This is only for WinApi window, Qt widgets use BorderlessWindow.css stylesheet
  HBRUSH windowBackground = CreateSolidBrush( RGB( 0, 0, 0 ) );

  // Create window
  BorderlessWindow window( application, windowBackground, defX, defY, defWidth, defHeight );

  // Launch
  return qApp->exec();
}
/************************ Ende main **************************************************************/

/************************ Ende main.cpp **********************************************************/

