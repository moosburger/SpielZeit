/*< Jungheinrich > ********************************************************************************/
/*!
*	\brief      Headerdatei zu cNativeEvent.cpp
*	\details
*
*	\file		cNativeEvent.h
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
* cNativeEvent.h
*/

#ifndef NATIVEEVENT_H
#define NATIVEEVENT_H

/**************************************************************************************************
* Includes
**************************************************************************************************/
#include <QAbstractNativeEventFilter>
#include <QApplication>

/**************************************************************************************************
* Defines
**************************************************************************************************/
QT_BEGIN_NAMESPACE
class cLogFile;
QT_END_NAMESPACE

//*************************************************************************************************
// KlassenName:     cLogFile
/// \details        -
//*************************************************************************************************

class cNativeEvent : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT

public:
    explicit cNativeEvent(QObject * parent = 0) : QObject(parent) {};
    //virtual ~cNativeEvent();

    virtual bool nativeEventFilter(const QByteArray &, void *, long *) Q_DECL_OVERRIDE;
    void registerLogger(cLogFile* pLogger);

signals:
    void sigGoingSuspend();
    void sigResume();

private:
    cLogFile* mLogger;
    bool mIsSuspend;
    bool mIsResume;

};
/************************ Ende Klasse cLogFile *************************************************/
#endif 
/************************ Ende cLogFile.h ******************************************************/