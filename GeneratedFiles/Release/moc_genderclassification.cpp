/****************************************************************************
** Meta object code from reading C++ file 'genderclassification.h'
**
** Created: Wed 28. Aug 16:25:57 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../gui/genderclassification.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'genderclassification.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_gui__genderclassification[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      27,   26,   26,   26, 0x0a,
      50,   26,   26,   26, 0x0a,
      68,   26,   26,   26, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_gui__genderclassification[] = {
    "gui::genderclassification\0\0"
    "captureTrainingImage()\0trainRecognizer()\0"
    "recognizeFaces()\0"
};

void gui::genderclassification::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        genderclassification *_t = static_cast<genderclassification *>(_o);
        switch (_id) {
        case 0: _t->captureTrainingImage(); break;
        case 1: _t->trainRecognizer(); break;
        case 2: _t->recognizeFaces(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData gui::genderclassification::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject gui::genderclassification::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_gui__genderclassification,
      qt_meta_data_gui__genderclassification, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &gui::genderclassification::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *gui::genderclassification::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *gui::genderclassification::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_gui__genderclassification))
        return static_cast<void*>(const_cast< genderclassification*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int gui::genderclassification::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
