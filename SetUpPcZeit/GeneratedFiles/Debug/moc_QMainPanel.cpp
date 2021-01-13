/****************************************************************************
** Meta object code from reading C++ file 'QMainPanel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Source/header/QMainPanel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QMainPanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QMainPanel_t {
    QByteArrayData data[24];
    char stringdata[277];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QMainPanel_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QMainPanel_t qt_meta_stringdata_QMainPanel = {
    {
QT_MOC_LITERAL(0, 0, 10), // "QMainPanel"
QT_MOC_LITERAL(1, 11, 25), // "pushButtonMinimizeClicked"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 25), // "pushButtonMaximizeClicked"
QT_MOC_LITERAL(4, 64, 22), // "pushButtonCloseClicked"
QT_MOC_LITERAL(5, 87, 10), // "slotSafeIt"
QT_MOC_LITERAL(6, 98, 8), // "slotExit"
QT_MOC_LITERAL(7, 107, 9), // "slotDelIt"
QT_MOC_LITERAL(8, 117, 16), // "slotEditFinished"
QT_MOC_LITERAL(9, 134, 18), // "slotSrvPthFinished"
QT_MOC_LITERAL(10, 153, 12), // "slotComboBox"
QT_MOC_LITERAL(11, 166, 17), // "slotBallonChanged"
QT_MOC_LITERAL(12, 184, 16), // "slotPopUpchanged"
QT_MOC_LITERAL(13, 201, 13), // "slotPwdChange"
QT_MOC_LITERAL(14, 215, 13), // "slotGenTicket"
QT_MOC_LITERAL(15, 229, 16), // "slotDruckeTicket"
QT_MOC_LITERAL(16, 246, 9), // "WochenTag"
QT_MOC_LITERAL(17, 256, 2), // "Mo"
QT_MOC_LITERAL(18, 259, 2), // "Di"
QT_MOC_LITERAL(19, 262, 2), // "Mi"
QT_MOC_LITERAL(20, 265, 2), // "Do"
QT_MOC_LITERAL(21, 268, 2), // "Fr"
QT_MOC_LITERAL(22, 271, 2), // "Sa"
QT_MOC_LITERAL(23, 274, 2) // "So"

    },
    "QMainPanel\0pushButtonMinimizeClicked\0"
    "\0pushButtonMaximizeClicked\0"
    "pushButtonCloseClicked\0slotSafeIt\0"
    "slotExit\0slotDelIt\0slotEditFinished\0"
    "slotSrvPthFinished\0slotComboBox\0"
    "slotBallonChanged\0slotPopUpchanged\0"
    "slotPwdChange\0slotGenTicket\0"
    "slotDruckeTicket\0WochenTag\0Mo\0Di\0Mi\0"
    "Do\0Fr\0Sa\0So"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QMainPanel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       1,  116, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x0a /* Public */,
       3,    0,   85,    2, 0x0a /* Public */,
       4,    0,   86,    2, 0x0a /* Public */,
       5,    1,   87,    2, 0x0a /* Public */,
       6,    1,   90,    2, 0x0a /* Public */,
       7,    1,   93,    2, 0x0a /* Public */,
       8,    0,   96,    2, 0x0a /* Public */,
       9,    0,   97,    2, 0x0a /* Public */,
      10,    1,   98,    2, 0x0a /* Public */,
      11,    1,  101,    2, 0x0a /* Public */,
      12,    1,  104,    2, 0x0a /* Public */,
      13,    1,  107,    2, 0x0a /* Public */,
      14,    1,  110,    2, 0x0a /* Public */,
      15,    1,  113,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,

 // enums: name, flags, count, data
      16, 0x0,    7,  120,

 // enum data: key, value
      17, uint(QMainPanel::Mo),
      18, uint(QMainPanel::Di),
      19, uint(QMainPanel::Mi),
      20, uint(QMainPanel::Do),
      21, uint(QMainPanel::Fr),
      22, uint(QMainPanel::Sa),
      23, uint(QMainPanel::So),

       0        // eod
};

void QMainPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QMainPanel *_t = static_cast<QMainPanel *>(_o);
        switch (_id) {
        case 0: _t->pushButtonMinimizeClicked(); break;
        case 1: _t->pushButtonMaximizeClicked(); break;
        case 2: _t->pushButtonCloseClicked(); break;
        case 3: _t->slotSafeIt((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->slotExit((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->slotDelIt((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->slotEditFinished(); break;
        case 7: _t->slotSrvPthFinished(); break;
        case 8: _t->slotComboBox((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->slotBallonChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->slotPopUpchanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->slotPwdChange((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->slotGenTicket((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->slotDruckeTicket((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject QMainPanel::staticMetaObject = {
    { &QWinWidget::staticMetaObject, qt_meta_stringdata_QMainPanel.data,
      qt_meta_data_QMainPanel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QMainPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QMainPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QMainPanel.stringdata))
        return static_cast<void*>(const_cast< QMainPanel*>(this));
    return QWinWidget::qt_metacast(_clname);
}

int QMainPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWinWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
