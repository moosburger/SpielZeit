/*<  > ********************************************************************************/
/*!
*	\brief      erzeugt und instanziert die GUI
*	\details    Beinhaltet File Save und Open. Erzeugt alle Menues
*
*	\file		QMainPanel.cpp
*
*	\copyright  (C) 2014 , All rights reserved.
*	\date       Erstellt am: 05.08.2014
*	\author     Gerhard Prexl
*
*	\version    1.0  -  05.08.2014
*/
/*< History > *************************************************************************************
*	Version     Datum       Kuerzel      Ticket#     Beschreibung
*   1.0         05.08.2014  GP           -------     Ersterstellung
* </ History ></  > ******************************************************************/

/**
* \addtogroup Modules-HandCoded
* \addtogroup Main
* \ingroup Modules-HandCoded
* QMainPanel.cpp
*/

/**************************************************************************************************
* Includes
**************************************************************************************************/
#include <windowsx.h>

#include "QMainWindow.h"
#include "QMainPanel.h"
#include "cPwd.h"

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
// FunktionsName:   QMainPanel
/// \details        Constructor
/// \param          -
/// \return         -
//*************************************************************************************************
QMainPanel::QMainPanel(HWND hWnd, BorderlessWindow* pParent) : 
    QWinWidget(hWnd), 
    mParent(pParent)
{
  windowHandle = hWnd;
  setObjectName( "mainPanel" );
  
  Init();
  readSettings();
  loadTheme();
  
  //Passwortabfrage
  mPwd = new cPwdHandler();
#ifdef NDEBUG
  int retVal = mPwd->PwdAbfrage(this);
  if (retVal != ERR_OK)
  {
      if (retVal == ERR_NOK)
        QMessageBox::information(this, "", QStringLiteral("Falsches Passwort"), QMessageBox::Ok);

      PostQuitMessage(0);
  }
#endif

  // TitelLeiste
  QWidget* TitleBar = createTitleBar(qApp->applicationName());

  // Main panel layout
  QGridLayout* mainGridLayout = new QGridLayout();
  mainGridLayout->setSpacing( 0 );
  mainGridLayout->setMargin( 0 );
  setLayout( mainGridLayout );

  // Vertical layout example
  QVBoxLayout* verticalLayout = new QVBoxLayout();
  verticalLayout->setSpacing(0);
  verticalLayout->setMargin(0);
  verticalLayout->setAlignment(Qt::AlignHCenter);
  verticalLayout->addWidget(TitleBar);
  
  // Central widget
  QWidget* centralWidget = new QWidget(this);
  centralWidget->setObjectName("centralWidget");
  centralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  
  // Das eingeh‰ngete HauptWidget (MyWidget)
  QWidget* MainWidget = createCentralWidget(centralWidget);  
  verticalLayout->addWidget(MainWidget);
  
    // Main panel scroll area
  QScrollArea* scrollArea = createScrollBar();

  // Show
  centralWidget->setLayout( verticalLayout );
  scrollArea->setWidget( centralWidget );
  mainGridLayout->addWidget( scrollArea );
    
  setParameter();
  show();
}
/************************ Ende QMainPanel *******************************************************/

//*************************************************************************************************
// FunktionsName:   QMainPanel
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
// Button events
void QMainPanel::pushButtonMinimizeClicked() {
  ShowWindow( parentWindow(), SW_MINIMIZE );
}
/************************ Ende QMainPanel *******************************************************/

//*************************************************************************************************
// FunktionsName:   pushButtonMaximizeClicked
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void QMainPanel::pushButtonMaximizeClicked() 
{
    WINDOWPLACEMENT wp;
    wp.length = sizeof( WINDOWPLACEMENT );
    GetWindowPlacement( parentWindow(), &wp );
    if ( wp.showCmd == SW_MAXIMIZE ) 
    {
        ShowWindow( parentWindow(), SW_RESTORE );
    } else {
        ShowWindow( parentWindow(), SW_MAXIMIZE );
    }
}
/************************ Ende pushButtonMaximizeClicked *******************************************************/

//*************************************************************************************************
// FunktionsName:   pushButtonCloseClicked
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void QMainPanel::pushButtonCloseClicked() 
{        
    writeSettings();
    qDestructor();
    PostQuitMessage( 0 );
}
/************************ Ende pushButtonCloseClicked *******************************************************/

//*************************************************************************************************
// FunktionsName:   Events
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
#if QT_VERSION >= 0x050000
  bool QMainPanel::nativeEvent( const QByteArray &, void *msg, long * ) {
#else
  bool QMainPanel::winEvent( MSG *message, long * ) {
#endif
#if QT_VERSION >= 0x050000
  MSG *message = ( MSG * )msg;
#endif
  switch( message->message ) {
    case WM_SYSKEYDOWN: {
      if ( message->wParam == VK_SPACE ) {
        RECT winrect;
        GetWindowRect( windowHandle, &winrect );
        TrackPopupMenu( GetSystemMenu( windowHandle, false ), TPM_TOPALIGN | TPM_LEFTALIGN, winrect.left + 5, winrect.top + 5, 0, windowHandle, NULL);
        break;
      }
    }
    case WM_KEYDOWN: {
      if ( message->wParam == VK_F5 ||
           message->wParam == VK_F6 ||
           message->wParam == VK_F7
         ) {
        SendMessage( windowHandle, WM_KEYDOWN, message->wParam, message->lParam );
        break;
      }
    }
  }
  return false;
}
/************************ Ende Events *******************************************************/  

//*************************************************************************************************
// FunktionsName:   mousePressEvent
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void QMainPanel::mousePressEvent( QMouseEvent *event ) {
  if ( event->button() == Qt::LeftButton ) {
     ReleaseCapture();
     SendMessage( windowHandle, WM_NCLBUTTONDOWN, HTCAPTION, 0 );
  }

  if ( event->type() == QEvent::MouseButtonDblClick ) {
    if (event -> button() == Qt::LeftButton) {
      WINDOWPLACEMENT wp;
      wp.length = sizeof( WINDOWPLACEMENT );
      GetWindowPlacement( parentWindow(), &wp );
      if ( wp.showCmd == SW_MAXIMIZE ) {
        ShowWindow( parentWindow(), SW_RESTORE );
      } else {
        ShowWindow( parentWindow(), SW_MAXIMIZE );
      }
    }
  }
}
/************************ Ende mousePressEvent *******************************************************/

//*************************************************************************************************
// FunktionsName:   createTitleBar
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
QWidget* QMainPanel::createTitleBar(const QString &Title)
{
    // Horizontal layout
    QHBoxLayout *horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setSpacing(5);
    horizontalLayout->setMargin(0);

    //Window Icon
    QPixmap qIco = qApp->windowIcon().pixmap(10, 10);
    qIco = qIco.scaledToHeight(20, Qt::SmoothTransformation);
    QLabel *iconLbl = new QLabel(this);
    iconLbl->setObjectName("windowIcon");
    iconLbl->setPixmap(qIco);
    horizontalLayout->addSpacing(5);
    horizontalLayout->addWidget(iconLbl);
    horizontalLayout->addSpacing(5);

    // Window title
    QLabel *windowTitle = new QLabel(this);
    windowTitle->setObjectName("windowTitle");
    windowTitle->setText(Title);
    windowTitle->setAttribute(Qt::WA_TransparentForMouseEvents);
    horizontalLayout->addWidget(windowTitle);
    horizontalLayout->addStretch();

    // System buttons
    // Minimize
    QPushButton *pushButtonMinimize = new QPushButton("", this);
    pushButtonMinimize->setObjectName("pushButtonMinimize");
    horizontalLayout->addWidget(pushButtonMinimize);
    QObject::connect(pushButtonMinimize, SIGNAL(clicked()), this, SLOT(pushButtonMinimizeClicked()));

    // Maximize
    QPushButton *pushButtonMaximize = new QPushButton("", this);
    pushButtonMaximize->setObjectName("pushButtonMaximize");
    horizontalLayout->addWidget(pushButtonMaximize);
    QObject::connect(pushButtonMaximize, SIGNAL(clicked()), this, SLOT(pushButtonMaximizeClicked()));

    // Close
    QPushButton *pushButtonClose = new QPushButton("", this);
    pushButtonClose->setObjectName("pushButtonClose");
    horizontalLayout->addWidget(pushButtonClose);
    horizontalLayout->addSpacing(8);
    QObject::connect(pushButtonClose, SIGNAL(clicked()), this, SLOT(pushButtonCloseClicked()));

    // Alles als TitleBar zusammenfassen
    QWidget *TitleBar = new QWidget(this);
    TitleBar->setObjectName("TitleBar");
    TitleBar->setFixedHeight(25);
    TitleBar->setLayout(horizontalLayout);

    return TitleBar;
}
/************************ Ende createTitleBar *******************************************************/

//*************************************************************************************************
// FunktionsName:   createScrollBar
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
QScrollArea* QMainPanel::createScrollBar()
{
    // Main panel scroll area
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setObjectName("mainPanelScrollArea");
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    return scrollArea;
}
/************************ Ende createScrollBar *******************************************************/

//*************************************************************************************************
// FunktionsName:   loadTheme
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void QMainPanel::loadTheme()
{
    // Aussehen der Applikation
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QFile file(mStyleSheetFile);
    if (file.open(QFile::ReadOnly))
    {
        QString styleSheet = (file.readAll());
        qApp->setStyleSheet(styleSheet);
    }
}
/************************ Ende loadTheme ********************************************************/

//*************************************************************************************************
// FunktionsName:   readSettings
/// \details        Liest die letzten Einstellungen der Fensterposition und Groeﬂe. Stellt diese dann ein
/// \param          -
/// \return         -
//*************************************************************************************************
void QMainPanel::readSettings()
{
    QSettings settings(qApp->organizationName(), qApp->applicationName());
    QPoint pos = settings.value("pos", QPoint(100, 200)).toPoint();
    QSize size = settings.value("size", QSize(cWidth, cHeight)).toSize();
    mStyleSheetFile = settings.value("style", ":/css/jungheinrich.css").toString();
    
    for (int i = 0; i < 7; i++)
    {
        mBegin[i]->setTime(QTime::fromString(settings.value(mWBeg[i], cDefBegin).toString(), "hh:mm:ss"));
        mEnd[i]->setTime(QTime::fromString(settings.value(mWEnd[i], cDefEnd).toString(), "hh:mm:ss"));
    }
        
    QString Programs = settings.value("Programme").toString();
    if (Programs.endsWith(";"))
        Programs.remove(Programs.size() - 1, 1);

    if (Programs.size() > 0)
        mPrograms = Programs.split(";");

    mGespZeit = settings.value("gespielteZeit", cDefGesp).toString();
    mSpZeit = settings.value("Spielzeit", cDefZeit).toString();
    mShowMsg = settings.value("BallonMsg").toInt();
    mSrvLogPfad = settings.value("ServerPfad", "").toString();

    mPwd->TicketCrypto(&mTickets, true);
    for (int i = 0; i < mTickets.size(); i++)
    {
        QString strHlp = mTickets[i];
        QStringList lstTckt = strHlp.split(";");
        mTickets[i] = lstTckt[0];
    }
    mParent->setWindowPos(pos.x(), pos.y(), size.width(), size.height());

#ifdef MAILSERVER
    mailServer = settings.value("mailServer", "Name@smtpServer.de").toString();
    mailPort = settings.value("mailPort", "465").toString();
    mailUser = settings.value("mailUser", "User@Server.de").toString();
    QByteArray data = settings.value("mailPwd").toByteArray();
    mailPwd = mPwd->DecryptPwd(data);
    mailEmpfanger = settings.value("mailEmpfanger", QStringLiteral("Empf‰nger@Server.de")).toString();
    mailCurlLog = settings.value("mailCurlLog", false).toBool();
#endif
}
/************************ Ende readSettings ******************************************************/

//*************************************************************************************************
// FunktionsName:   writeSettings
/// \details        Schreibt die letzten Einstellungen der Fensterposition und Groeﬂe.
/// \param          -
/// \return         -
//*************************************************************************************************
void QMainPanel::writeSettings()
{
    QStringList Programme;
    for (int i = 0; i < mPrgmVbox->count(); i++)
    {   // Vom Vertikalem LAyout ¸ber die horizontalen Layouts, auf das LineEdit an Column 2
        QLayoutItem* hLayItem = mPrgmVbox->itemAt(i)->layout()->itemAt(2);
        QLineEdit* LineEdit = qobject_cast<QLineEdit*>(hLayItem->widget());
        if (!LineEdit->text().isEmpty())
            Programme.append(LineEdit->text());
    }  
    QSettings settings(qApp->organizationName(), qApp->applicationName());

    RECT rect = mParent->getWindowSize();    
    QPoint pos(rect.left, rect.top);
    QSize size(rect.right - rect.left, rect.bottom - rect.top);
    settings.setValue("pos", pos);
    
    for (int i = 0; i < 7; i++)
    {
        settings.setValue(mWBeg[i], mBegin[i]->time().toString());
        settings.setValue(mWEnd[i], mEnd[i]->time().toString());
    }

    mSrvLogPfad = mServerPath->text();    
    settings.setValue("Programme", Programme.join(";"));
    settings.setValue("gespielteZeit", mGespielteZeit->text());
    settings.setValue("Spielzeit", mSpielZeit->text());        
    settings.setValue("style", mStyleSheetFile);    
    settings.setValue("BallonMsg", mShowMsg);
    settings.setValue("ServerPfad", mSrvLogPfad);

#ifdef MAILSERVER
    settings.setValue("mailServer", mSmtpServer->text());
    settings.setValue("mailPort", mSmtpPort->text());
    settings.setValue("mailUser", mMailUser->text());
    QByteArray encrypted = mPwd->EncryptPwd(mMailPwd->text());
    settings.setValue("mailPwd", encrypted);
    settings.setValue("mailEmpfanger", mRecipient->text());
    settings.setValue("mailCurlLog", mailCurlLog);
#endif
}
/************************ Ende writeSettings *****************************************************/

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
// hier beginnt der Teil eingeh‰ngten Konfig
//*************************************************************************************************

//*************************************************************************************************
// FunktionsName:   createCentralWidget
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
QWidget* QMainPanel::createCentralWidget(QWidget* centralWidget)
{    
    QWidget* Center = new QWidget(centralWidget);
    QGridLayout *grid = new QGridLayout();

    grid->addWidget(Programme(), 0, 0, 1, 1);
    grid->addWidget(WochenZeit(), 0, 1, 1, 1);
    grid->addWidget(showTicket(), 2, 0, 1, 2);
#ifdef MAILSERVER
    grid->addWidget(MailServer(), 3, 0, 1, 2);
#endif
    grid->addWidget(SpielZeit(), 4, 0, 1, 1);
    grid->addWidget(Einstellungen(), 4, 1, 1, 1);
    
    Center->setLayout(grid);
    
    return Center;
}
/************************ Ende createCentralWidget *******************************************************/

//*************************************************************************************************
// FunktionsName:   qDestructor
/// \details        Destructor
/// \param          -
/// \return         -
//*************************************************************************************************
void QMainPanel::qDestructor()
{
}
/************************ Ende qDestructor *******************************************************/

/************************ Ende QMainPanel.cpp *******************************************************/