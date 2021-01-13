/****************************************************************************
** Meta object code from reading C++ file 'cSendMail.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Source/header/cSendMail.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cSendMail.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_cSendMailWorker_t {
    QByteArrayData data[14];
    char stringdata[146];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_cSendMailWorker_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_cSendMailWorker_t qt_meta_stringdata_cSendMailWorker = {
    {
QT_MOC_LITERAL(0, 0, 15), // "cSendMailWorker"
QT_MOC_LITERAL(1, 16, 10), // "sigRetStat"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 7), // "retStat"
QT_MOC_LITERAL(4, 36, 9), // "sigRetMsg"
QT_MOC_LITERAL(5, 46, 6), // "strMsg"
QT_MOC_LITERAL(6, 53, 12), // "slotSendMail"
QT_MOC_LITERAL(7, 66, 10), // "strSubject"
QT_MOC_LITERAL(8, 77, 7), // "strZeit"
QT_MOC_LITERAL(9, 85, 7), // "strSpZt"
QT_MOC_LITERAL(10, 93, 7), // "strMain"
QT_MOC_LITERAL(11, 101, 14), // "slotReSendMail"
QT_MOC_LITERAL(12, 116, 14), // "slotReadStdOut"
QT_MOC_LITERAL(13, 131, 14) // "slotReadStdErr"

    },
    "cSendMailWorker\0sigRetStat\0\0retStat\0"
    "sigRetMsg\0strMsg\0slotSendMail\0strSubject\0"
    "strZeit\0strSpZt\0strMain\0slotReSendMail\0"
    "slotReadStdOut\0slotReadStdErr"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_cSendMailWorker[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    1,   47,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    4,   50,    2, 0x0a /* Public */,
      11,    0,   59,    2, 0x0a /* Public */,
      12,    0,   60,    2, 0x08 /* Private */,
      13,    0,   61,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QString,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    7,    8,    9,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void cSendMailWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        cSendMailWorker *_t = static_cast<cSendMailWorker *>(_o);
        switch (_id) {
        case 0: _t->sigRetStat((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->sigRetMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->slotSendMail((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 3: _t->slotReSendMail(); break;
        case 4: _t->slotReadStdOut(); break;
        case 5: _t->slotReadStdErr(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (cSendMailWorker::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&cSendMailWorker::sigRetStat)) {
                *result = 0;
            }
        }
        {
            typedef void (cSendMailWorker::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&cSendMailWorker::sigRetMsg)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject cSendMailWorker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_cSendMailWorker.data,
      qt_meta_data_cSendMailWorker,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *cSendMailWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *cSendMailWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_cSendMailWorker.stringdata))
        return static_cast<void*>(const_cast< cSendMailWorker*>(this));
    return QObject::qt_metacast(_clname);
}

int cSendMailWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void cSendMailWorker::sigRetStat(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void cSendMailWorker::sigRetMsg(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_cSendMail_t {
    QByteArrayData data[12];
    char stringdata[112];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_cSendMail_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_cSendMail_t qt_meta_stringdata_cSendMail = {
    {
QT_MOC_LITERAL(0, 0, 9), // "cSendMail"
QT_MOC_LITERAL(1, 10, 11), // "sigSendMail"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 10), // "strSubject"
QT_MOC_LITERAL(4, 34, 7), // "strZeit"
QT_MOC_LITERAL(5, 42, 7), // "strSpZt"
QT_MOC_LITERAL(6, 50, 7), // "strMain"
QT_MOC_LITERAL(7, 58, 13), // "sigReSendMail"
QT_MOC_LITERAL(8, 72, 13), // "slotRetStatus"
QT_MOC_LITERAL(9, 86, 7), // "retStat"
QT_MOC_LITERAL(10, 94, 10), // "slotRetMsg"
QT_MOC_LITERAL(11, 105, 6) // "strMsg"

    },
    "cSendMail\0sigSendMail\0\0strSubject\0"
    "strZeit\0strSpZt\0strMain\0sigReSendMail\0"
    "slotRetStatus\0retStat\0slotRetMsg\0"
    "strMsg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_cSendMail[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   34,    2, 0x06 /* Public */,
       7,    0,   43,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   44,    2, 0x0a /* Public */,
      10,    1,   47,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,    4,    5,    6,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::QString,   11,

       0        // eod
};

void cSendMail::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        cSendMail *_t = static_cast<cSendMail *>(_o);
        switch (_id) {
        case 0: _t->sigSendMail((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 1: _t->sigReSendMail(); break;
        case 2: _t->slotRetStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->slotRetMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (cSendMail::*_t)(QString , QString , QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&cSendMail::sigSendMail)) {
                *result = 0;
            }
        }
        {
            typedef void (cSendMail::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&cSendMail::sigReSendMail)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject cSendMail::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_cSendMail.data,
      qt_meta_data_cSendMail,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *cSendMail::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *cSendMail::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_cSendMail.stringdata))
        return static_cast<void*>(const_cast< cSendMail*>(this));
    return QObject::qt_metacast(_clname);
}

int cSendMail::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void cSendMail::sigSendMail(QString _t1, QString _t2, QString _t3, QString _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void cSendMail::sigReSendMail()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
