/****************************************************************************
** Meta object code from reading C++ file 'EnvelopeLibraryWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "src/EnvelopeLibraryWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EnvelopeLibraryWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_EnvelopeLibraryWindow_t {
    QByteArrayData data[10];
    char stringdata0[174];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EnvelopeLibraryWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EnvelopeLibraryWindow_t qt_meta_stringdata_EnvelopeLibraryWindow = {
    {
QT_MOC_LITERAL(0, 0, 21), // "EnvelopeLibraryWindow"
QT_MOC_LITERAL(1, 22, 19), // "onTreeItemActivated"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 11), // "QModelIndex"
QT_MOC_LITERAL(4, 55, 5), // "index"
QT_MOC_LITERAL(5, 61, 22), // "onTreeSelectionChanged"
QT_MOC_LITERAL(6, 84, 21), // "onValueEntriesChanged"
QT_MOC_LITERAL(7, 106, 22), // "onNewEnvelopeTriggered"
QT_MOC_LITERAL(8, 129, 28), // "onDuplicateEnvelopeTriggered"
QT_MOC_LITERAL(9, 158, 15) // "onSaveTriggered"

    },
    "EnvelopeLibraryWindow\0onTreeItemActivated\0"
    "\0QModelIndex\0index\0onTreeSelectionChanged\0"
    "onValueEntriesChanged\0onNewEnvelopeTriggered\0"
    "onDuplicateEnvelopeTriggered\0"
    "onSaveTriggered"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EnvelopeLibraryWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x08 /* Private */,
       5,    0,   47,    2, 0x08 /* Private */,
       6,    0,   48,    2, 0x08 /* Private */,
       7,    0,   49,    2, 0x08 /* Private */,
       8,    0,   50,    2, 0x08 /* Private */,
       9,    0,   51,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void EnvelopeLibraryWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<EnvelopeLibraryWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onTreeItemActivated((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 1: _t->onTreeSelectionChanged(); break;
        case 2: _t->onValueEntriesChanged(); break;
        case 3: _t->onNewEnvelopeTriggered(); break;
        case 4: _t->onDuplicateEnvelopeTriggered(); break;
        case 5: _t->onSaveTriggered(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject EnvelopeLibraryWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_EnvelopeLibraryWindow.data,
    qt_meta_data_EnvelopeLibraryWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *EnvelopeLibraryWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EnvelopeLibraryWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_EnvelopeLibraryWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int EnvelopeLibraryWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
