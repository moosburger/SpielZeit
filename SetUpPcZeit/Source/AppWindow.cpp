/*< > ********************************************************************************/
/*!
*	\brief      erzeugt und instanziert die GUI
*	\details    Beinhaltet File Save und Open. Erzeugt alle Menues
*
*	\file		AppWindow.cpp
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
* </ History ></ > ******************************************************************/

/**
* \addtogroup Modules-HandCoded
* \addtogroup Main
* \ingroup Modules-HandCoded
* AppWindow.cpp
*/

/**************************************************************************************************
* Includes
**************************************************************************************************/
#include <QtWidgets>
#include <QPrinter>
#include <QPrintDialog>
#include <QTinyAES>
#include <random>

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
// FunktionsName:   Init
/// \details        InitFunktion
/// \param          -
/// \return         -
//*************************************************************************************************
void QMainPanel::Init()
{
    mTickets.clear();

    for (int i = 0; i < 7; i++)
    {
        mBegin.append(new QTimeEdit());
        mBegin[i]->setDisplayFormat("hh:mm:ss");
        mEnd.append(new QTimeEdit());
        mEnd[i]->setDisplayFormat("hh:mm:ss");
    }
    mWBeg << cMoBeg << cDiBeg << cMiBeg << cDoBeg << cFrBeg << cSaBeg << cSoBeg;
    mWEnd << cMoEnd << cDiEnd << cMiEnd << cDoEnd << cFrEnd << cSaEnd << cSoEnd;

    for (int i = 0; i < cTICKETCNT; i++)
        mTckt.append(new QLabel("tckt" + QString::number(i)));
}
/************************ Ende Init *******************************************************/

//*************************************************************************************************
// FunktionsName:   slotDelIt
/// \details        -
/// \param          -
/// \return         -
//************************************************************************************************
void QMainPanel::setParameter()
{
    int xPos, yPos, width, height;
    
    mSpielZeit->setText(mSpZeit);
    mGespielteZeit->setText(mGespZeit);
    
    mParent->getWindowPos(&xPos, &yPos, &width, &height);
    if (mPrograms.size() > cProgramCnt)
    {
        int SizeAdd = cHeightAdd * (mPrograms.size() - cProgramCnt); // Eine leere für neue Programme
        height += SizeAdd;
        mParent->setWindowPos(xPos, yPos, width, height);
    }

    // Tickets
    mDruckButton->setVisible((mTickets.size() > 0));
    mGenButton->setVisible((mTickets.size() == 0));

    for (int i = 0; i < cTICKETCNT; i++)
        mTckt[i]->setVisible((mTickets.size() > 0));
}
/************************ Ende slotDelIt ****************************************************/

//*************************************************************************************************
// FunktionsName:   SpielZeit
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
QGroupBox *QMainPanel::SpielZeit()
{
    QGroupBox *groupBox = new QGroupBox(QStringLiteral(" SpielZeit"));
    groupBox->setAlignment(Qt::AlignHCenter);
    QLabel* lbSpiel = new QLabel("max. Spielzeit (min):");
    QLabel* lbGespiel = new QLabel("gespielt (min):");
    lbSpiel->setFixedWidth(100);
    lbGespiel->setFixedWidth(100);

    QPushButton* mDelButton = new QPushButton(QStringLiteral("&Löschen"));
    mDelButton->setFixedWidth(100);

    mSpielZeit = new QLineEdit();
    mGespielteZeit = new QLineEdit();
    mSpielZeit->setFixedWidth(30);
    mGespielteZeit->setFixedWidth(30);

    QBoxLayout* hBox1 = new QBoxLayout(QBoxLayout::LeftToRight);
    hBox1->addWidget(lbSpiel, 0, Qt::AlignLeft);
    hBox1->addWidget(mSpielZeit, 0, Qt::AlignLeft);

    QBoxLayout* hBox2 = new QBoxLayout(QBoxLayout::LeftToRight);
    hBox2->addWidget(lbGespiel, 0, Qt::AlignLeft);
    hBox2->addWidget(mGespielteZeit, 0, Qt::AlignLeft);

    QGridLayout *grid = new QGridLayout();
    grid->addLayout(hBox1, 0, 0, 1, 1);
    grid->addLayout(hBox2, 1, 0, 1, 1);
    grid->addWidget(mDelButton, 2, 0, 1, 1);
    grid->setContentsMargins(10, 15, 10, 5);
    
    groupBox->setFixedHeight(80);
    groupBox->setLayout(grid);

    QObject::connect(mDelButton, SIGNAL(clicked(bool)), this, SLOT(slotDelIt(bool)));

    return groupBox;
}
/************************ Ende SpielZeit *****************************************************/

//*************************************************************************************************
// FunktionsName:   WochenZeit
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
QGroupBox *QMainPanel::WochenZeit()
{
    QGroupBox *groupBox = new QGroupBox(QStringLiteral(" zeitlicher Rahmen"));
    groupBox->setAlignment(Qt::AlignHCenter);

    QLabel* lbRahmen = new QLabel("Zeitrahmen von:");
    QList<QLabel*>wTag;
    QLabel* lbBis = new QLabel("bis:");

    QMetaObject metaObject = QMainPanel::staticMetaObject;
    QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("WochenTag"));
    for (int i = 1; i < 8; i++)
        wTag.append(new QLabel(QString(metaEnum.valueToKey(i)) + ":"));

    lbBis->setFixedWidth(35);
    foreach(QLabel* pDay, wTag)
        pDay->setFixedWidth(25);

    QList<QBoxLayout*> hBox;
    hBox.append(GetHBox(lbRahmen, lbBis));

    for (int u = 0; u < 7; u++)
        hBox.append(GetHBox(wTag[u], mBegin[u], mEnd[u]));

    QBoxLayout* vBox = new QBoxLayout(QBoxLayout::TopToBottom);
    vBox->addStretch();
    foreach(QBoxLayout* pHbox, hBox)
        vBox->addLayout(pHbox);

    vBox->setContentsMargins(10, 15, 10, 5);
    vBox->addStretch();
    groupBox->setLayout(vBox);

    return groupBox;
}
/************************ Ende WochenZeit *****************************************************/

//*************************************************************************************************
// FunktionsName:   Programme
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
QGroupBox *QMainPanel::Programme()
{
    QGroupBox *groupBox = new QGroupBox(QStringLiteral(" Programme"));
    groupBox->setAlignment(Qt::AlignHCenter);
    
    mPrgmVbox = new QBoxLayout(QBoxLayout::TopToBottom);
    QString strHlp;
    for (int i = 0; i <= mPrograms.size(); i++)
    {
        QLabel* lbSpiel = new QLabel();
        strHlp = "Spiel ";
        strHlp.append(QString::number(i+1));
        strHlp.append(" :");
        lbSpiel->setText(strHlp);
        lbSpiel->setFixedWidth(45);

        QLineEdit* Programm = new QLineEdit();
        if (i < mPrograms.size())   // Eine leere für neue Programme
            Programm->setText(mPrograms[i].trimmed());

        QBoxLayout* hBox = new QBoxLayout(QBoxLayout::LeftToRight);
        hBox->addStretch(1);
        hBox->addWidget(lbSpiel);
        hBox->addWidget(Programm);
        hBox->addStretch(1);
        mPrgmVbox->addLayout(hBox);

        QObject::connect(Programm, SIGNAL(returnPressed()), this, SLOT(slotEditFinished()));
    }
    mPrgmVbox->setContentsMargins(10, 15, 10, 5);
    groupBox->setLayout(mPrgmVbox);
    return groupBox;
}
/************************ Ende Programme *****************************************************/

//*************************************************************************************************
// FunktionsName:   Einstellungen
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
QGroupBox *QMainPanel::showTicket()
{
    QGroupBox* TicketBox = new QGroupBox();
    TicketBox->setTitle(QStringLiteral(" Tickets (für %1 min, oder jetzt )").arg(mSpZeit));
    TicketBox->setAlignment(Qt::AlignHCenter);
    QGridLayout *grid = new QGridLayout();
    grid->setContentsMargins(10, 15, 10, 5);

    mDruckButton = new QPushButton();
    mDruckButton->setIcon(QIcon(":/images/Printer.png"));
    mDruckButton->setIconSize(QSize(27, 27));
    mDruckButton->setFixedSize(30, 30);
    mGenButton = new QPushButton(QStringLiteral("&Ticket"));
    mGenButton->setFixedSize(30, 30);

    int eVal = 1;
    int sVal = 0;
    
    for (int i = 0; i < cTICKETCNT; i++)
    {
        mTckt[i] = new QLabel("");
        if (i < mTickets.size())
            mTckt[i]->setText(mTickets[i]);

        if (i == cTICKETCNT / 2)
        {
            sVal = 0;
            eVal = 2;
        }
        grid->addWidget(mTckt[i], eVal, sVal++, 1, 1, 0);
    }
    // GenerierButton
    grid->addWidget(mGenButton, 1, 6, 2, 1, Qt::AlignRight);
    QObject::connect(mGenButton, SIGNAL(clicked(bool)), this, SLOT(slotGenTicket(bool)));

    // Drucken Button
    grid->addWidget(mDruckButton, 1, 6, 2, 1, Qt::AlignRight);
    QObject::connect(mDruckButton, SIGNAL(clicked(bool)), this, SLOT(slotDruckeTicket(bool)));
    
    TicketBox->setLayout(grid);
    return TicketBox;
}

//*************************************************************************************************
// FunktionsName:   Einstellungen
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
QGroupBox *QMainPanel::Einstellungen()
{
    QGroupBox *groupBox = new QGroupBox(QStringLiteral(" Einstellungen"));
    groupBox->setAlignment(Qt::AlignHCenter);
    QPushButton* mSafeButton = new QPushButton(QStringLiteral("&Speichern"));
    QPushButton* mEndButton = new QPushButton(QStringLiteral("&Beenden"));
    QPushButton* PwdChange = new QPushButton(QStringLiteral("&Passwort"));
    mSafeButton->setFixedWidth(50);
    mEndButton->setFixedWidth(50);
    PwdChange->setFixedWidth(50);

    mStyle = new QComboBox(this);
    mStyle->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    mStyle->setMaxVisibleItems(10);
    QListView* qAbstractView = new QListView(this);
    mStyle->setView(qAbstractView);
    mStyle->addItem("Default");
    mStyle->addItem("Gray");
    mStyle->addItem("Coffee");
    mStyle->addItem("dark");
    mStyle->addItem("lightgray");
    mStyle->setEditable(false);

    QFileInfo file(mStyleSheetFile);
    QString style = file.baseName().toLower();
    if (style == "jungheinrich")
        style = "lightgray";

    int idx = mStyle->findText(style, Qt::MatchRegExp);
    mStyle->setCurrentIndex(idx);

    QCheckBox *Ballon = new QCheckBox(QStringLiteral("Ballontips anzeigen"));
    if ((mShowMsg & 2) == 2)
        Ballon->setChecked(true);

    QCheckBox *PopUp = new QCheckBox(QStringLiteral("PopUp Nachrichten"));
    if ((mShowMsg & 1) == 1)
        PopUp->setChecked(true);

    // Pfad auf ServerLaufwerk
    mServerPath = new QLineEdit(mSrvLogPfad);
    mServerPath->setFixedWidth(170);
    QLabel* SvLbl = new QLabel(QStringLiteral("ServerPfad:"));
    QObject::connect(mServerPath, SIGNAL(returnPressed()), this, SLOT(slotSrvPthFinished()));
    
    QBoxLayout* hBox2 = new QBoxLayout(QBoxLayout::LeftToRight);
    hBox2->addWidget(Ballon);
    hBox2->addWidget(PopUp);

    QBoxLayout* hBox3 = new QBoxLayout(QBoxLayout::LeftToRight);
    hBox3->addWidget(SvLbl, 0, Qt::AlignLeft);
    hBox3->addWidget(mServerPath, 0, Qt::AlignLeft);

    QBoxLayout* hBox4 = new QBoxLayout(QBoxLayout::LeftToRight);
    hBox4->addWidget(mSafeButton, 0, Qt::AlignLeft);
    hBox4->addWidget(mStyle, 0, Qt::AlignRight);
    hBox4->addWidget(PwdChange, 0, Qt::AlignRight);
    hBox4->addWidget(mEndButton, 0, Qt::AlignRight);

    QBoxLayout* vBox = new QBoxLayout(QBoxLayout::TopToBottom);
    vBox->addLayout(hBox2);
    vBox->addLayout(hBox3);
    vBox->addLayout(hBox4);
    vBox->setContentsMargins(10, 15, 10, 5);

    groupBox->setFixedHeight(80);
    groupBox->setLayout(vBox);

    QObject::connect(Ballon, SIGNAL(stateChanged(int)), this, SLOT(slotBallonChanged(int)));
    QObject::connect(PopUp, SIGNAL(stateChanged(int)), this, SLOT(slotPopUpchanged(int)));
    QObject::connect(mStyle, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotComboBox(QString)));
    QObject::connect(mSafeButton, SIGNAL(clicked(bool)), this, SLOT(slotSafeIt(bool)));
    QObject::connect(mEndButton, SIGNAL(clicked(bool)), this, SLOT(slotExit(bool)));
    QObject::connect(PwdChange, SIGNAL(clicked(bool)), this, SLOT(slotPwdChange(bool)));

    return groupBox;
}
/************************ Ende Einstellungen *****************************************************/

//*************************************************************************************************
// FunktionsName:   MailServer
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
QGroupBox *QMainPanel::MailServer()
{
    QGroupBox *groupBox = new QGroupBox(QStringLiteral(" MailServer"));
    groupBox->setAlignment(Qt::AlignHCenter);

    // MailEmpfänger
    QLabel* Lbl1 = new QLabel(QStringLiteral("Smtp Server:"));
    mSmtpServer = new QLineEdit(mailServer);
    mSmtpServer->setFixedWidth(122);

    QLabel* Lbl2 = new QLabel(QStringLiteral("Port:"));
    mSmtpPort = new QLineEdit(mailPort);
    mSmtpPort->setFixedWidth(25);

    QLabel* Lbl4 = new QLabel(QStringLiteral("Benutzer:"));
    mMailUser = new QLineEdit(mailUser);
    mMailUser->setFixedWidth(100);

    QLabel* Lbl5 = new QLabel(QStringLiteral("Passwort:"));
    mMailPwd = new QLineEdit(mailPwd);
    mMailPwd->setFixedWidth(65);

    QLabel* Lbl6 = new QLabel(QStringLiteral("Empfänger:"));
    mRecipient = new QLineEdit(mailEmpfanger);
    mRecipient->setFixedWidth(100);
    
    QLabel* Lbl7 = new QLabel(QStringLiteral("Log"));
    QCheckBox *cUrlLog = new QCheckBox();
    if ((mailCurlLog) == true)
        cUrlLog->setChecked(true);

    QBoxLayout* hBox1 = new QBoxLayout(QBoxLayout::LeftToRight);
    hBox1->addWidget(Lbl1);
    hBox1->addWidget(mSmtpServer);
    hBox1->addStretch();
    hBox1->addWidget(Lbl4);
    hBox1->addWidget(mMailUser);
    hBox1->addWidget(Lbl7);

    QBoxLayout* hBox2 = new QBoxLayout(QBoxLayout::LeftToRight);
    hBox2->addWidget(Lbl2);
    hBox2->addWidget(mSmtpPort);
    hBox2->addWidget(Lbl5);
    hBox2->addWidget(mMailPwd);
    hBox2->addStretch();
    hBox2->addWidget(Lbl6);
    hBox2->addWidget(mRecipient);
    hBox2->addWidget(cUrlLog);

    QBoxLayout* vBox = new QBoxLayout(QBoxLayout::TopToBottom);
    vBox->addLayout(hBox1);
    vBox->addLayout(hBox2);
    vBox->setContentsMargins(10, 15, 10, 5);

    groupBox->setLayout(vBox);    
    return groupBox;
}
/************************ Ende MailServer *****************************************************/

//*************************************************************************************************
// FunktionsName:   GetHBox
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
QBoxLayout* QMainPanel::GetHBox(QLabel* pRahmen, QLabel* pBis)
{
    QBoxLayout* hBox = new QBoxLayout(QBoxLayout::LeftToRight);
    hBox->addStretch(1);
    hBox->addWidget(pRahmen);
    hBox->addStretch(1);
    hBox->addWidget(pBis);
    hBox->addStretch(1);

    return hBox;
}
/************************ Ende GetHBox *****************************************************/

//*************************************************************************************************
// FunktionsName:   GetHBox
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
QBoxLayout* QMainPanel::GetHBox(QLabel* pTag, QTimeEdit* pVon, QTimeEdit* pBis)
{
    QBoxLayout* hBox = new QBoxLayout(QBoxLayout::LeftToRight);
    hBox->addStretch(1);
    hBox->addWidget(pTag);
    hBox->addWidget(pVon, 0, Qt::AlignRight);
    hBox->addStretch(1);
    hBox->addWidget(pBis);
    hBox->addStretch(1);

    return hBox;
}
/************************ Ende GetHBox *****************************************************/

//*************************************************************************************************
// FunktionsName:   SrvPthValid
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
bool QMainPanel::SrvPthValid()
{
    bool retVal = true;
    mSrvLogPfad = QDir::cleanPath(mServerPath->text());
    mServerPath->setText(mSrvLogPfad);

    QDir SrvPfad(mSrvLogPfad);
    if (!SrvPfad.exists())
    {
        retVal = false;
        QMessageBox::information(NULL, cTitel, QStringLiteral("Ungültige Pfadangabe\n%1").arg(mSrvLogPfad), QMessageBox::Ok);
    }
    qDebug() << mSrvLogPfad;
    return retVal;
}
/************************ Ende SrvPthValid ****************************************************/

//*************************************************************************************************
// FunktionsName:   genTicket
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void QMainPanel::slotGenTicket(bool checked)
{
    using namespace std;
    random_device rd;   // non-deterministic generator
    mt19937 gen(rd());  // to seed mersenne twister.

    for (int i = 0; i < cTICKETCNT; ++i)
        mTickets.append(QString::number(gen()));

    //Verschlüsseln und in Reg speichern
    mPwd->TicketCrypto(&mTickets, false);
    
    // Butten verstecken
    mGenButton->setVisible((mTickets.size() == 0));

    //Rest anzeigen
    for (int i = 0; i < cTICKETCNT; i++)
    {
        mTckt[i]->setVisible((mTickets.size() > 0));
        mTckt[i]->setText(mTickets[i]);
    }
    mDruckButton->setVisible((mTickets.size() > 0));
    mDruckButton->setFocus();
}
/************************ Ende genTicket *****************************************************/

//*************************************************************************************************
// FunktionsName:   slotDruckeTicket
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void QMainPanel::slotDruckeTicket(bool checked)
{
    int xOff = 0;
    int yOff = 10;
    int xWidth = 300;
    int yHeight = 150;

    QPrinter *printer = new QPrinter();
    printer->setColorMode(QPrinter::GrayScale);

    QPrintDialog *pdlg = new QPrintDialog(printer, this);
    if (pdlg->exec() == QDialog::Accepted)
    {
        bool Ok;
        QPainter *painter = new QPainter(printer);
        QFont *qfont = new QFont("Times", 14, QFont::Bold, false);
        painter->setFont(*qfont);

        //Überschrift
        painter->drawText(QRectF(30, 30, 700, 50), Qt::AlignLeft,
            QStringLiteral("Tickets für Pc Spiele.").arg(mSpZeit));

        qfont->setPointSize(12);
        qfont->setWeight(QFont::Normal);
        painter->setFont(*qfont);

        painter->drawText(QRectF(30, 70, 700, 300), Qt::AlignLeft, 
            QStringLiteral("Tickets dienen der Verlängerung der abgelaufenen Spielzeit oder zum Spielen außerhalb der\nfestgelegten Zeit.\n\
                           \nJedes Ticket verlängert die Spielzeit einmalig um %1 Minuten.\
                           \nJede Ticket-Nummer kann nur einmal verwendet werden.").arg(mSpZeit));

        for (int i = 0; i < mTickets.size(); i++)
        {
            qfont->setPointSize(12);
            qfont->setWeight(QFont::Normal);
            painter->setFont(*qfont);

            int cnt = 1;
            QString strTckt;
            for (int g = mTickets[i].size() - 1; g >= 0; g--)
            {
                strTckt.prepend(mTickets[i].at(g));
                int mod = cnt++ % 3;
                if (mod == 0)
                    strTckt.prepend(" ");
            }
            cnt = i;
            if (i > 4)
            {
                xOff = xWidth + 10;
                cnt -= 5;
            }
            yOff = cnt * 160;
            painter->drawRect(QRectF(30 + xOff, 200 + yOff, xWidth, yHeight));

            painter->drawText(QRectF(30 + xOff, 230 + yOff, xWidth, 24), Qt::AlignCenter,
                QStringLiteral("Verlängerung der Spielzeit um %1 min").arg(mSpZeit));

            qfont->setPointSize(10);
            painter->setFont(*qfont);
            painter->drawText(QRectF(30 + xOff, 260 + yOff, xWidth, 24), Qt::AlignCenter,
                QStringLiteral("Ticket-Nummer OHNE Leerzeichen eingeben:"));

            qfont->setPointSize(14);
            qfont->setWeight(QFont::Bold);
            painter->setFont(*qfont);
            painter->drawText(QRectF(30 + xOff, 290 + yOff, xWidth, 24), Qt::AlignCenter,
                QStringLiteral("%1").arg(strTckt));
        }
        painter->end(); 
    }
    show();
}
/************************ Ende slotDruckeTicket ****************************************************/

//*************************************************************************************************
// FunktionsName:   slotComboBox
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void QMainPanel::slotComboBox(QString file)
{
    file = file.toLower();
    if (file == "lightgray")
        file = "jungheinrich";

    mStyleSheetFile = ":/css/" + file + ".css";

    loadTheme();
}
/************************ Ende slotComboBox ****************************************************/

//*************************************************************************************************
// FunktionsName:   slotBallonChanged
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void QMainPanel::slotPwdChange(bool checked)
{
    if (!mPwd->ChangePwd(cTitel))
        QMessageBox::information(NULL, cTitel, QStringLiteral("Passwort wurde nicht geändert"), QMessageBox::Ok);
}
/************************ Ende slotBallonChanged ****************************************************/

//*************************************************************************************************
// FunktionsName:   slotBallonChanged
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void QMainPanel::slotBallonChanged(int checked)
{
    if (checked == Qt::Unchecked)
        mShowMsg = (mShowMsg & 1); // 11 & 01 = 01

    if (checked == Qt::Checked)
        mShowMsg = (mShowMsg | 2); // 00 | 10 = 10
}
/************************ Ende slotBallonChanged ****************************************************/

//*************************************************************************************************
// FunktionsName:   slotPopUpchanged
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void QMainPanel::slotPopUpchanged(int checked)
{
    if (checked == Qt::Unchecked)
        mShowMsg = (mShowMsg & 2);  // 11 & 10 = 10

    if (checked == Qt::Checked)
        mShowMsg = (mShowMsg | 1);  // 00 | 01 = 01
}
/************************ Ende slotPopUpchanged ****************************************************/

//*************************************************************************************************
// FunktionsName:   slotExit
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void QMainPanel::slotExit(bool checked)
{
    pushButtonCloseClicked();
}
/************************ Ende slotExit ****************************************************/

//*************************************************************************************************
// FunktionsName:   slotSafeIt
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void QMainPanel::slotSafeIt(bool checked)
{
    if (!SrvPthValid())
        return;

    writeSettings();
}
/************************ Ende slotSafeIt ****************************************************/

//*************************************************************************************************
// FunktionsName:   slotDelIt
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void QMainPanel::slotDelIt(bool checked)
{
    mGespielteZeit->setText("0");
}
/************************ Ende slotDelIt ****************************************************/

//*************************************************************************************************
// FunktionsName:   slotSrvPthFinished
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void QMainPanel::slotSrvPthFinished()
{
    SrvPthValid();
}
/************************ Ende slotSrvPthFinished ****************************************************/

//*************************************************************************************************
// FunktionsName:   slotEditFinished
/// \details        -
/// \param          -
/// \return         -
//*************************************************************************************************
void QMainPanel::slotEditFinished()
{
    this->blockSignals(true);
    bool bExist = false;

    QLineEdit* LineEdit = qobject_cast<QLineEdit*>(sender());
    if (!LineEdit)
        return;

    if (LineEdit->text().isEmpty())
        return;

    int i = mPrgmVbox->count(); // Spacing und Safe/Ende button werden insgesamt abgezogen
    // Inhalt der VBox holen
    QList<QObject*> pList = mPrgmVbox->children();
    for (int u = 0; u < i; u++)
    {
        QBoxLayout* Box1 = qobject_cast<QBoxLayout*>(pList.at(u));
        QLayoutItem* Lay = Box1->itemAt(2);
        if (Lay == NULL)
            break;

        QLineEdit* stre1 = qobject_cast<QLineEdit*>(Lay->widget());
        // Gibts den Eintrag, und ist es nicht der letzte
        if ((LineEdit->text() == stre1->text()) && (u < i-1))
            bExist = true;
    }

    if (bExist)
    {
        this->blockSignals(false);
        return;
    }

    QLabel* lbSpiel = new QLabel();
    QString strHlp = "Spiel ";
    strHlp.append(QString::number(i + 1)); // für den neuen Eintrag
    strHlp.append(" :");
    lbSpiel->setText(strHlp);
    lbSpiel->setFixedWidth(45);

    QLineEdit* Programm = new QLineEdit(); 
    QBoxLayout* hBox = new QBoxLayout(QBoxLayout::LeftToRight);
    hBox->addStretch(1);
    hBox->addWidget(lbSpiel);
    hBox->addWidget(Programm);
    hBox->addStretch(1);
    mPrgmVbox->insertLayout(i + 1, hBox);
    
    QObject::connect(Programm, SIGNAL(returnPressed()), this, SLOT(slotEditFinished()));  

    // Nächsten Eintrag editMode aktivieren
    Programm->setFocus(Qt::TabFocusReason);
    
    if (i > cProgramCnt)
    {
        RECT rect = mParent->getWindowSize();
        QSize size(rect.right - rect.left, rect.bottom - rect.top);
        size.setHeight(size.height() + cHeightAdd);
        mParent->setWindowSize(size.width(), size.height());
    }
    
    this->blockSignals(false);
}
/************************ Ende slotDelIt ****************************************************/

/************************ Ende AppWindow.cpp *******************************************************/