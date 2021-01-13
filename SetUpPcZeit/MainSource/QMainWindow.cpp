/*< Jungheinrich > ********************************************************************************/
/*!
*	\brief      erzeugt und instanziert die GUI
*	\details    Beinhaltet File Save und Open. Erzeugt alle Menues
*
*	\file		QMainPanel.cpp
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
* QMainPanel.cpp
*/

/**************************************************************************************************
* Includes
**************************************************************************************************/
#include "QMainWindow.h"
#include "QMainPanel.h"

#include <dwmapi.h>
#include <windowsx.h>
#include <stdexcept>

#include <QPushButton>
#include <QFile>

/**************************************************************************************************
* Defines
**************************************************************************************************/

/**************************************************************************************************
* Variablen
**************************************************************************************************/
HWND winId = 0;
HDC hdc;
PAINTSTRUCT ps;
/**************************************************************************************************
* Funktionen
**************************************************************************************************/
QMainPanel *BorderlessWindow::mainPanel;
QApplication *BorderlessWindow::a;

//*************************************************************************************************
// FunktionsName:   cCfgWriter
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
BorderlessWindow::BorderlessWindow( QApplication *app, HBRUSH windowBackground, const int xPos, const int yPos, const int width, const int height ) : hWnd(0),
  hInstance( GetModuleHandle( NULL ) ),
  borderless( false ),
  borderlessResizeable( true ),
  aeroShadow( false ),
  closed( false ),
  visible( false )
{
  WNDCLASSEX wcx = { 0 };
  wcx.cbSize = sizeof( WNDCLASSEX );
  wcx.style = CS_HREDRAW | CS_VREDRAW;
  wcx.hInstance = hInstance;
  wcx.lpfnWndProc = WndProc;
  wcx.cbClsExtra	= 0;
  wcx.cbWndExtra	= 0;
  wcx.lpszClassName = L"WindowClass";
  wcx.hbrBackground = windowBackground;
  wcx.hCursor = LoadCursor( hInstance, IDC_ARROW );
  RegisterClassEx( &wcx );
  if ( FAILED( RegisterClassEx( &wcx ) ) ) throw std::runtime_error( "Couldn't register window class" );

  hWnd = CreateWindow( L"WindowClass", L"BorderlessWindow", static_cast<DWORD>( Style::windowed ), xPos, yPos, width, height, 0, 0, hInstance, nullptr );
  if ( !hWnd ) throw std::runtime_error( "couldn't create window because of reasons" );

  SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

  mainPanel = new QMainPanel(hWnd, this);
  winId = (HWND)mainPanel->winId();
        
  show();
  toggleBorderless();
  a = app;

  // die Fenstergröße erst hier einstellen, aus der mainPanel heraus kracht es, bevor das Fenster angezeigt wird.
  SetWindowPos(hWnd, NULL, this->windowPos.x, this->windowPos.y, this->windowPos.width, this->windowPos.height, SWP_SHOWWINDOW);
}
/************************ Ende ActivateEdit *******************************************************/

//*************************************************************************************************
// FunktionsName:   cCfgWriter
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
BorderlessWindow::~BorderlessWindow()
{
  hide();
  DestroyWindow( hWnd );
}
/************************ Ende ActivateEdit *******************************************************/

//*************************************************************************************************
// FunktionsName:   cCfgWriter
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
LRESULT CALLBACK BorderlessWindow::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
  BorderlessWindow *window = reinterpret_cast<BorderlessWindow*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
  if ( !window ) return DefWindowProc( hWnd, message, wParam, lParam );

  switch ( message ) {
    case WM_KEYDOWN: {
   /*   switch ( wParam ) {
        case VK_F5: {
          window->borderlessResizeable = !window->borderlessResizeable;
          break;
        }
        case VK_F6: {
          window->toggleShadow();
          window->toggleBorderless();
          SetFocus( winId );
          break;
        }
        case VK_F7: {
          window->toggleShadow();
          break;
        }
      }*/

      if ( wParam != VK_TAB ) return DefWindowProc( hWnd, message, wParam, lParam );

      SetFocus( winId );
      break;
    }

    // ALT + SPACE or F10 system menu
    case WM_SYSCOMMAND: {
      if ( wParam == SC_KEYMENU ) {
          RECT winrect;
          GetWindowRect( hWnd, &winrect );
          TrackPopupMenu( GetSystemMenu( hWnd, false ), TPM_TOPALIGN | TPM_LEFTALIGN, winrect.left + 5, winrect.top + 5, 0, hWnd, NULL);
          break;
      } else {
        return DefWindowProc( hWnd, message, wParam, lParam );
      }
    }

    case WM_SETFOCUS: {
      QString str( "Got focus" );
      QWidget *widget = QWidget::find( ( WId )HWND( wParam ) );
      if ( widget )
        str += QString( " from %1 (%2)" ).arg( widget->objectName() ).arg(widget->metaObject()->className() );
      str += "\n";
      OutputDebugStringA( str.toLocal8Bit().data() );
      break;
    }

    case WM_NCCALCSIZE: {
      //this kills the window frame and title bar we added with
      //WS_THICKFRAME and WS_CAPTION
      if (window->borderless) {
          return 0;
      }
      break;
    }

    case WM_KILLFOCUS: {
      QString str( "Lost focus" );
      QWidget *widget = QWidget::find( (WId)HWND( wParam ) );
      if ( widget )
        str += QString( " to %1 (%2)" ).arg( widget->objectName() ).arg(widget->metaObject()->className() );
      str += "\n";

      OutputDebugStringA( str.toLocal8Bit().data() );
      break;
    }

    case WM_DESTROY: {
      PostQuitMessage(0);
      break;
    }

    case WM_NCHITTEST: {
      if ( window->borderless )
      {
        if ( window->borderlessResizeable )
        {
          const LONG borderWidth = 8; //in pixels
          RECT winrect;
          GetWindowRect( hWnd, &winrect );
          long x = GET_X_LPARAM( lParam );
          long y = GET_Y_LPARAM( lParam );

          //bottom left corner
          if ( x >= winrect.left && x < winrect.left + borderWidth &&
            y < winrect.bottom && y >= winrect.bottom - borderWidth )
          {
            return HTBOTTOMLEFT;
          }
          //bottom right corner
          if ( x < winrect.right && x >= winrect.right - borderWidth &&
            y < winrect.bottom && y >= winrect.bottom - borderWidth )
          {
            return HTBOTTOMRIGHT;
          }
          //top left corner
          if ( x >= winrect.left && x < winrect.left + borderWidth &&
            y >= winrect.top && y < winrect.top + borderWidth )
          {
            return HTTOPLEFT;
          }
          //top right corner
          if ( x < winrect.right && x >= winrect.right - borderWidth &&
            y >= winrect.top && y < winrect.top + borderWidth )
          {
            return HTTOPRIGHT;
          }
          //left border
          if ( x >= winrect.left && x < winrect.left + borderWidth )
          {
            return HTLEFT;
          }
          //right border
          if ( x < winrect.right && x >= winrect.right - borderWidth )
          {
            return HTRIGHT;
          }
          //bottom border
          if ( y < winrect.bottom && y >= winrect.bottom - borderWidth )
          {
            return HTBOTTOM;
          }
          //top border
          if ( y >= winrect.top && y < winrect.top + borderWidth )
          {
            return HTTOP;
          }
        }

        return HTCAPTION;
      }
      break;
    }

    case WM_SIZE: {
      RECT winrect;
      GetClientRect( hWnd, &winrect );

      WINDOWPLACEMENT wp;
      wp.length = sizeof( WINDOWPLACEMENT );
      GetWindowPlacement( hWnd, &wp );
      if ( wp.showCmd == SW_MAXIMIZE ) {
          // Vollbild Größe ohne Rahmen
        QPushButton* pushButtonMaximize = mainPanel->findChild<QPushButton*>( "pushButtonMaximize" );
        pushButtonMaximize->setStyleSheet( "#pushButtonMaximize {image: url(:/images/Restore.png);} #pushButtonMaximize:hover { image: url(:/images/RestoreHover.png); }" );
        int defSice = 8;
        mainPanel->setGeometry(defSice, defSice, winrect.right - defSice * 2, winrect.bottom - defSice*2);
      } else {
          //kleineres Bild mit einem Rahmen, wie Vs2013. Umrandung sollte mit StatusBarFarbe erfolgen
        QPushButton* pushButtonMaximize = mainPanel->findChild<QPushButton*>( "pushButtonMaximize" );
        pushButtonMaximize->setStyleSheet("#pushButtonMaximize {image: url(:/images/Maximize.png);} #pushButtonMaximize:hover { image: url(:/images/MaximizeHover.png); }");
        int defSice = 1;
        mainPanel->setGeometry(defSice, defSice, winrect.right - defSice * 2, winrect.bottom - defSice * 2);
      }
      break;
    }

    case WM_GETMINMAXINFO: {
      MINMAXINFO* minMaxInfo = ( MINMAXINFO* )lParam;
      if ( window->minimumSize.required ) {
        minMaxInfo->ptMinTrackSize.x = window->getMinimumWidth();;
        minMaxInfo->ptMinTrackSize.y = window->getMinimumHeight();
      }

      if ( window->maximumSize.required ) {
        minMaxInfo->ptMaxTrackSize.x = window->getMaximumWidth();
        minMaxInfo->ptMaxTrackSize.y = window->getMaximumHeight();
      }
      return 0;
    }
  }
  return DefWindowProc(hWnd, message, wParam, lParam);
}
/************************ Ende ActivateEdit *******************************************************/

//*************************************************************************************************
// FunktionsName:   cCfgWriter
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
void BorderlessWindow::toggleBorderless() {
  if ( visible )
  {
    Style newStyle = ( borderless ) ? Style::windowed : Style::aero_borderless;
    SetWindowLongPtr( hWnd, GWL_STYLE, static_cast<LONG>( newStyle ) );

    borderless = !borderless;
    if ( newStyle == Style::aero_borderless ) {
      toggleShadow();
    }
    //redraw frame
    SetWindowPos( hWnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE );
    show();
  }
}
/************************ Ende ActivateEdit *******************************************************/

//*************************************************************************************************
// FunktionsName:   cCfgWriter
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
void BorderlessWindow::toggleShadow() {
  if ( borderless ) {
    aeroShadow = !aeroShadow;
    const MARGINS shadow_on = { 1, 1, 1, 1 };
    const MARGINS shadow_off = { 0, 0, 0, 0 };
    const MARGINS* pMarInset = (aeroShadow) ? (&shadow_on) : (&shadow_off);
    DwmExtendFrameIntoClientArea(hWnd, pMarInset);
  }
}
/************************ Ende ActivateEdit *******************************************************/

//*************************************************************************************************
// FunktionsName:   cCfgWriter
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
void BorderlessWindow::toggleResizeable() {
  borderlessResizeable = borderlessResizeable ? false : true;
}
/************************ Ende ActivateEdit *******************************************************/

//*************************************************************************************************
// FunktionsName:   cCfgWriter
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
bool BorderlessWindow::isResizeable() {
  return borderlessResizeable ? true : false;
}
/************************ Ende ActivateEdit *******************************************************/
//*************************************************************************************************
// FunktionsName:   cCfgWriter
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
void BorderlessWindow::show() {
  ShowWindow( hWnd, SW_SHOW );
  visible = true;
}
/************************ Ende ActivateEdit *******************************************************/

//*************************************************************************************************
// FunktionsName:   cCfgWriter
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
void BorderlessWindow::hide() {
  ShowWindow( hWnd, SW_HIDE );
  visible = false;
}
/************************ Ende ActivateEdit *******************************************************/

//*************************************************************************************************
// FunktionsName:   cCfgWriter
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
bool BorderlessWindow::isVisible() {
  return visible ? true : false;
}
/************************ Ende ActivateEdit *******************************************************/

//*************************************************************************************************
// FunktionsName:   cCfgWriter
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
void BorderlessWindow::setWindowSize(const int width, const int height)
{
    SetWindowPos(hWnd, NULL, 0, 0, width, height, SWP_NOMOVE);
}
/************************ Ende ActivateEdit *******************************************************/

//*************************************************************************************************
// FunktionsName:   cCfgWriter
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
void BorderlessWindow::setWindowPos(const int xPos, const int yPos, const int width, const int height)
{
    this->windowPos.x = xPos;
    this->windowPos.y = yPos;
    this->windowPos.height = height;
    this->windowPos.width = width;
}
/************************ Ende ActivateEdit *******************************************************/

//*************************************************************************************************
// FunktionsName:   cCfgWriter
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
void BorderlessWindow::getWindowPos(int* xPos, int* yPos, int* width, int* height)
{
    *xPos = this->windowPos.x;
    *yPos = this->windowPos.y;
    *height = this->windowPos.height;
    *width = this->windowPos.width;
}
/************************ Ende ActivateEdit *******************************************************/


//*************************************************************************************************
// FunktionsName:   cCfgWriter
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
RECT BorderlessWindow::getWindowSize()
{
    RECT rect;
    GetWindowRect(hWnd, &rect);

    return rect;
}
/************************ Ende ActivateEdit *******************************************************/

//*************************************************************************************************
// FunktionsName:   cCfgWriter
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
// Minimum size
void BorderlessWindow::setMinimumSize( const int width, const int height ) {
  this->minimumSize.required = true;
  this->minimumSize.width = width;
  this->minimumSize.height = height;
}
/************************ Ende ActivateEdit *******************************************************/

//*************************************************************************************************
// FunktionsName:   cCfgWriter
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
bool BorderlessWindow::isSetMinimumSize() {
    return this->minimumSize.required;
}
/************************ Ende ActivateEdit *******************************************************/

//*************************************************************************************************
// FunktionsName:   cCfgWriter
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
void BorderlessWindow::removeMinimumSize() {
  this->minimumSize.required = false;
  this->minimumSize.width = 0;
  this->minimumSize.height = 0;
}
/************************ Ende ActivateEdit *******************************************************/

//*************************************************************************************************
// FunktionsName:   cCfgWriter
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
int BorderlessWindow::getMinimumWidth() {
  return minimumSize.width;
}
/************************ Ende ActivateEdit *******************************************************/

//*************************************************************************************************
// FunktionsName:   cCfgWriter
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
int BorderlessWindow::getMinimumHeight() {
  return minimumSize.height;
}
/************************ Ende ActivateEdit *******************************************************/

//*************************************************************************************************
// FunktionsName:   cCfgWriter
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
// Maximum size
void BorderlessWindow::setMaximumSize( const int width, const int height ) {
  this->maximumSize.required = true;
  this->maximumSize.width = width;
  this->maximumSize.height = height;
}
/************************ Ende ActivateEdit *******************************************************/

//*************************************************************************************************
// FunktionsName:   cCfgWriter
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
bool BorderlessWindow::isSetMaximumSize() {
    return this->maximumSize.required;
}
/************************ Ende ActivateEdit *******************************************************/

//*************************************************************************************************
// FunktionsName:   cCfgWriter
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
void BorderlessWindow::removeMaximumSize() {
  this->maximumSize.required = false;
  this->maximumSize.width = 0;
  this->maximumSize.height = 0;
}
/************************ Ende ActivateEdit *******************************************************/

//*************************************************************************************************
// FunktionsName:   cCfgWriter
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
int BorderlessWindow::getMaximumWidth() {
  return maximumSize.width;
}
/************************ Ende ActivateEdit *******************************************************/

//*************************************************************************************************
// FunktionsName:   cCfgWriter
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
int BorderlessWindow::getMaximumHeight() {
  return maximumSize.height;
}
/************************ Ende ActivateEdit *******************************************************/

/************************ Ende QMainPanel.cpp *******************************************************/