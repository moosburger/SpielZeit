/****************************************************************************
** Meta object code from reading C++ file 'cWinApiStuff.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Source/header/cWinApiStuff.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cWinApiStuff.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_cCallBackHandler_t {
    QByteArrayData data[5];
    char stringdata[64];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_cCallBackHandler_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_cCallBackHandler_t qt_meta_stringdata_cCallBackHandler = {
    {
QT_MOC_LITERAL(0, 0, 16), // "cCallBackHandler"
QT_MOC_LITERAL(1, 17, 14), // "sigMouseActive"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 17), // "sigKeyBoardActive"
QT_MOC_LITERAL(4, 51, 12) // "slotRunHooks"

    },
    "cCallBackHandler\0sigMouseActive\0\0"
    "sigKeyBoardActive\0slotRunHooks"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_cCallBackHandler[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       3,    1,   32,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   35,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void cCallBackHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        cCallBackHandler *_t = static_cast<cCallBackHandler *>(_o);
        switch (_id) {
        case 0: _t->sigMouseActive((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->sigKeyBoardActive((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->slotRunHooks(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (cCallBackHandler::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&cCallBackHandler::sigMouseActive)) {
                *result = 0;
            }
        }
        {
            typedef void (cCallBackHandler::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&cCallBackHandler::sigKeyBoardActive)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject cCallBackHandler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_cCallBackHandler.data,
      qt_meta_data_cCallBackHandler,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *cCallBackHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *cCallBackHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_cCallBackHandler.stringdata))
        return static_cast<void*>(const_cast< cCallBackHandler*>(this));
    return QObject::qt_metacast(_clname);
}

int cCallBackHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void cCallBackHandler::sigMouseActive(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void cCallBackHandler::sigKeyBoardActive(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_cWinApiStuff_t {
    QByteArrayData data[4];
    char stringdata[49];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_cWinApiStuff_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_cWinApiStuff_t qt_meta_stringdata_cWinApiStuff = {
    {
QT_MOC_LITERAL(0, 0, 12), // "cWinApiStuff"
QT_MOC_LITERAL(1, 13, 15), // "slotMouseActive"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 18) // "slotKeyBoardActive"

    },
    "cWinApiStuff\0slotMouseActive\0\0"
    "slotKeyBoardActive"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_cWinApiStuff[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x0a /* Public */,
       3,    1,   27,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void cWinApiStuff::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        cWinApiStuff *_t = static_cast<cWinApiStuff *>(_o);
        switch (_id) {
        case 0: _t->slotMouseActive((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->slotKeyBoardActive((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject cWinApiStuff::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_cWinApiStuff.data,
      qt_meta_data_cWinApiStuff,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *cWinApiStuff::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *cWinApiStuff::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_cWinApiStuff.stringdata))
        return static_cast<void*>(const_cast< cWinApiStuff*>(this));
    return QObject::qt_metacast(_clname);
}

int cWinApiStuff::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
