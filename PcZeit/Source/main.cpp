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

/**************************************************************************************************
* Includes
**************************************************************************************************/
#include "cPcZeit.h"
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

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName(FIRMA);
    app.setApplicationName(APPNAME);
    app.setApplicationVersion(STRPRODUCTVER);
    app.setWindowIcon(QIcon(":/images/PcZeitApp.png"));
    app.setQuitOnLastWindowClosed(false);

    // Klassen erzeugen
    cPcZeit* cMain = new cPcZeit();
    
    //Signal zum beenden der Anwendung
    QObject::connect(cMain, SIGNAL(finished()), &app, SLOT(quit()));
    //CleanUp
    QObject::connect(&app, SIGNAL(aboutToQuit()), cMain, SLOT(slotAboutToQuit()));

    QTimer::singleShot(0, cMain, SLOT(slotRunMainLoop()));        

    return app.exec();
}
