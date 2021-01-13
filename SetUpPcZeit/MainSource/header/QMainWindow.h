/*< Jungheinrich > ********************************************************************************/
/*!
*	\brief      Headerdatei zu MainWindow.cpp
*	\details
*
*	\file		mainwindow.h
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
* mainwindow.h
*/

#ifndef QMAINWINDOW_H
#define QMAINWINDOW_H

#define NOMINMAX 	
/**************************************************************************************************
* Includes
**************************************************************************************************/
#include <windows.h>
#include "QWinWidget.h"
#include <QApplication>

/**************************************************************************************************
* Defines
**************************************************************************************************/

QT_BEGIN_NAMESPACE
class QMainPanel;
QT_END_NAMESPACE

#define defX 100
#define defY 50
#define defWidth 620
#define defHeight 210

//*************************************************************************************************
// KlassenName:     BorderlessWindow
/// \details        -
//*************************************************************************************************
class BorderlessWindow
{
  enum class Style : DWORD
  {
    windowed = ( WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CLIPCHILDREN ),
    aero_borderless = (WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CLIPCHILDREN)
  };

public:
  HWND hWnd;
  HINSTANCE hInstance;

  BorderlessWindow( QApplication *app, HBRUSH windowBackground, const int x, const int y, const int width, const int height );
  ~BorderlessWindow();
  static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
  void show();
  void hide();
  bool isVisible();

  void toggleBorderless();
  void toggleShadow();
  void toggleResizeable();
  bool isResizeable();

  void setMinimumSize( const int width, const int height );
  bool isSetMinimumSize();
  void removeMinimumSize();
  int getMinimumHeight();
  int getMinimumWidth();
  RECT getWindowSize();

  void setWindowSize(const int width, const int height);
  void setWindowPos(const int xPos, const int yPos, const int width, const int height);
  void getWindowPos(int* xPos, int* yPos, int* width, int* height);
  void setMaximumSize( const int width, const int height );
  bool isSetMaximumSize();
  int getMaximumHeight();
  int getMaximumWidth();
  void removeMaximumSize();

private:
  static QApplication *a;
  static QMainPanel *mainPanel;

  bool closed;
  bool visible;

  bool borderless;
  bool aeroShadow;
  bool borderlessResizeable;

  struct sizeType {
    sizeType() : required( false ), width( 0 ), height( 0 ) {}
    bool required;
    int width;
    int height;
  };

  struct posType {
      posType() : x(0), y(0), width(0), height(0)  {}
      int x;
      int y;
      int width;
      int height;
  };

  posType windowPos;
  sizeType minimumSize;
  sizeType maximumSize;
};
/************************ Ende Klasse cMainWindow *************************************************/
#endif
/************************ Ende mainwindow.h ******************************************************/
