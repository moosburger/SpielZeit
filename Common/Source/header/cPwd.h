/*< Jungheinrich > ********************************************************************************/
/*!
*	\brief      Headerdatei zu DriveFilter.cpp
*	\details
*
*	\file		cPwd.h
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
* cPwd.h
*/

#ifndef PWD_H
#define PWD_H

/**************************************************************************************************
* Includes
**************************************************************************************************/
#include <QtWidgets>

/**************************************************************************************************
* Defines
**************************************************************************************************/
QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

#define cTICKETCNT 10

/*! Enum der möglichen Fehler */
enum eERROR
{
    ERR_OK,     /*!< kein Fehler */
    ERR_NOK,    /*!< Fehler aufgetreten*/
    ERR_IGNORE  /*!< Fehler, kann aber ignoriert werden */
};

//*************************************************************************************************
// KlassenName:     cPwd
/// \details        -
//*************************************************************************************************

class cPwdHandler : public QObject
{
    Q_OBJECT

public:
    cPwdHandler();
    virtual ~cPwdHandler();

    int PwdAbfrage(const QString Title);
    int PwdAbfrage(QWidget*);
    bool ChangePwd(const QString Title);
    bool ChangePwd(QWidget* pThis);
    bool TicketCrypto(QStringList* pTicket, bool bDir);
    QString DecryptPwd(QByteArray data);
    QByteArray EncryptPwd(QString text);

private:
    int mPwdAbf(const QString Title, QWidget* pThis = NULL);
    bool mChangePwd(const QString Title, QWidget* pThis = NULL);
    QString mPwd;

};
/************************ Ende Klasse cLogFile *************************************************/
#endif 
/************************ Ende cLogFile.h ******************************************************/