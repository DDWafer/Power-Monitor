/****************************************************************************
** Meta object code from reading C++ file 'newwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qt_3/newwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'newwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_newwindow_t {
    QByteArrayData data[11];
    char stringdata0[140];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_newwindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_newwindow_t qt_meta_stringdata_newwindow = {
    {
QT_MOC_LITERAL(0, 0, 9), // "newwindow"
QT_MOC_LITERAL(1, 10, 7), // "newdata"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 12), // "close_serial"
QT_MOC_LITERAL(4, 32, 27), // "on_OpenSerialButton_clicked"
QT_MOC_LITERAL(5, 60, 8), // "ReadData"
QT_MOC_LITERAL(6, 69, 10), // "timerEvent"
QT_MOC_LITERAL(7, 80, 12), // "QTimerEvent*"
QT_MOC_LITERAL(8, 93, 11), // "serial_lose"
QT_MOC_LITERAL(9, 105, 28), // "QSerialPort::SerialPortError"
QT_MOC_LITERAL(10, 134, 5) // "error"

    },
    "newwindow\0newdata\0\0close_serial\0"
    "on_OpenSerialButton_clicked\0ReadData\0"
    "timerEvent\0QTimerEvent*\0serial_lose\0"
    "QSerialPort::SerialPortError\0error"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_newwindow[] = {

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
       1,    0,   44,    2, 0x06 /* Public */,
       3,    0,   45,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   46,    2, 0x08 /* Private */,
       5,    0,   47,    2, 0x08 /* Private */,
       6,    1,   48,    2, 0x08 /* Private */,
       8,    1,   51,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    2,
    QMetaType::Void, 0x80000000 | 9,   10,

       0        // eod
};

void newwindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        newwindow *_t = static_cast<newwindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newdata(); break;
        case 1: _t->close_serial(); break;
        case 2: _t->on_OpenSerialButton_clicked(); break;
        case 3: _t->ReadData(); break;
        case 4: _t->timerEvent((*reinterpret_cast< QTimerEvent*(*)>(_a[1]))); break;
        case 5: _t->serial_lose((*reinterpret_cast< QSerialPort::SerialPortError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (newwindow::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&newwindow::newdata)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (newwindow::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&newwindow::close_serial)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject newwindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_newwindow.data,
      qt_meta_data_newwindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *newwindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *newwindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_newwindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int newwindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
void newwindow::newdata()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void newwindow::close_serial()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
