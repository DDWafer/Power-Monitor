/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qt_3/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[80];
    char stringdata0[1278];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 13), // "WriteTimestop"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 9), // "delaymsec"
QT_MOC_LITERAL(4, 36, 4), // "msec"
QT_MOC_LITERAL(5, 41, 10), // "timerEvent"
QT_MOC_LITERAL(6, 52, 12), // "QTimerEvent*"
QT_MOC_LITERAL(7, 65, 23), // "on_actionExit_triggered"
QT_MOC_LITERAL(8, 89, 26), // "on_actionSetting_triggered"
QT_MOC_LITERAL(9, 116, 7), // "setpage"
QT_MOC_LITERAL(10, 124, 5), // "index"
QT_MOC_LITERAL(11, 130, 27), // "on_actionsave_all_triggered"
QT_MOC_LITERAL(12, 158, 30), // "on_actionRestore_All_triggered"
QT_MOC_LITERAL(13, 189, 29), // "on_actionBackup_ALL_triggered"
QT_MOC_LITERAL(14, 219, 27), // "on_actionLoad_ALL_triggered"
QT_MOC_LITERAL(15, 247, 23), // "on_actionAuto_triggered"
QT_MOC_LITERAL(16, 271, 10), // "dataBackup"
QT_MOC_LITERAL(17, 282, 10), // "closeEvent"
QT_MOC_LITERAL(18, 293, 12), // "QCloseEvent*"
QT_MOC_LITERAL(19, 306, 5), // "event"
QT_MOC_LITERAL(20, 312, 10), // "serial_end"
QT_MOC_LITERAL(21, 323, 11), // "rule_extend"
QT_MOC_LITERAL(22, 335, 8), // "ReadData"
QT_MOC_LITERAL(23, 344, 12), // "serial_close"
QT_MOC_LITERAL(24, 357, 9), // "save_data"
QT_MOC_LITERAL(25, 367, 8), // "timedata"
QT_MOC_LITERAL(26, 376, 8), // "voltdata"
QT_MOC_LITERAL(27, 385, 9), // "voltdata2"
QT_MOC_LITERAL(28, 395, 13), // "current_valid"
QT_MOC_LITERAL(29, 409, 9), // "loopcount"
QT_MOC_LITERAL(30, 419, 4), // "hour"
QT_MOC_LITERAL(31, 424, 9), // "save_slot"
QT_MOC_LITERAL(32, 434, 19), // "on_download_clicked"
QT_MOC_LITERAL(33, 454, 21), // "on_save_clicked_after"
QT_MOC_LITERAL(34, 476, 15), // "on_save_clicked"
QT_MOC_LITERAL(35, 492, 15), // "on_load_clicked"
QT_MOC_LITERAL(36, 508, 17), // "on_excute_clicked"
QT_MOC_LITERAL(37, 526, 17), // "graph_down_replot"
QT_MOC_LITERAL(38, 544, 16), // "on_clear_clicked"
QT_MOC_LITERAL(39, 561, 9), // "test_text"
QT_MOC_LITERAL(40, 571, 21), // "on_download_2_clicked"
QT_MOC_LITERAL(41, 593, 17), // "on_save_2_clicked"
QT_MOC_LITERAL(42, 611, 23), // "on_save_2_clicked_after"
QT_MOC_LITERAL(43, 635, 17), // "on_load_2_clicked"
QT_MOC_LITERAL(44, 653, 19), // "on_excute_2_clicked"
QT_MOC_LITERAL(45, 673, 19), // "graph_down_replot_2"
QT_MOC_LITERAL(46, 693, 18), // "on_clear_2_clicked"
QT_MOC_LITERAL(47, 712, 11), // "test_text_2"
QT_MOC_LITERAL(48, 724, 21), // "on_download_3_clicked"
QT_MOC_LITERAL(49, 746, 17), // "on_save_3_clicked"
QT_MOC_LITERAL(50, 764, 23), // "on_save_3_clicked_after"
QT_MOC_LITERAL(51, 788, 17), // "on_load_3_clicked"
QT_MOC_LITERAL(52, 806, 19), // "on_excute_3_clicked"
QT_MOC_LITERAL(53, 826, 19), // "graph_down_replot_3"
QT_MOC_LITERAL(54, 846, 18), // "on_clear_3_clicked"
QT_MOC_LITERAL(55, 865, 11), // "test_text_3"
QT_MOC_LITERAL(56, 877, 21), // "on_download_4_clicked"
QT_MOC_LITERAL(57, 899, 17), // "on_save_4_clicked"
QT_MOC_LITERAL(58, 917, 23), // "on_save_4_clicked_after"
QT_MOC_LITERAL(59, 941, 17), // "on_load_4_clicked"
QT_MOC_LITERAL(60, 959, 19), // "on_excute_4_clicked"
QT_MOC_LITERAL(61, 979, 19), // "graph_down_replot_4"
QT_MOC_LITERAL(62, 999, 18), // "on_clear_4_clicked"
QT_MOC_LITERAL(63, 1018, 11), // "test_text_4"
QT_MOC_LITERAL(64, 1030, 21), // "on_download_5_clicked"
QT_MOC_LITERAL(65, 1052, 17), // "on_save_5_clicked"
QT_MOC_LITERAL(66, 1070, 23), // "on_save_5_clicked_after"
QT_MOC_LITERAL(67, 1094, 17), // "on_load_5_clicked"
QT_MOC_LITERAL(68, 1112, 19), // "on_excute_5_clicked"
QT_MOC_LITERAL(69, 1132, 19), // "graph_down_replot_5"
QT_MOC_LITERAL(70, 1152, 18), // "on_clear_5_clicked"
QT_MOC_LITERAL(71, 1171, 11), // "test_text_5"
QT_MOC_LITERAL(72, 1183, 9), // "tab_1_ini"
QT_MOC_LITERAL(73, 1193, 9), // "tab_2_ini"
QT_MOC_LITERAL(74, 1203, 9), // "tab_3_ini"
QT_MOC_LITERAL(75, 1213, 9), // "tab_4_ini"
QT_MOC_LITERAL(76, 1223, 9), // "tab_5_ini"
QT_MOC_LITERAL(77, 1233, 11), // "stop_excute"
QT_MOC_LITERAL(78, 1245, 12), // "clear_excute"
QT_MOC_LITERAL(79, 1258, 19) // "on_action_triggered"

    },
    "MainWindow\0WriteTimestop\0\0delaymsec\0"
    "msec\0timerEvent\0QTimerEvent*\0"
    "on_actionExit_triggered\0"
    "on_actionSetting_triggered\0setpage\0"
    "index\0on_actionsave_all_triggered\0"
    "on_actionRestore_All_triggered\0"
    "on_actionBackup_ALL_triggered\0"
    "on_actionLoad_ALL_triggered\0"
    "on_actionAuto_triggered\0dataBackup\0"
    "closeEvent\0QCloseEvent*\0event\0serial_end\0"
    "rule_extend\0ReadData\0serial_close\0"
    "save_data\0timedata\0voltdata\0voltdata2\0"
    "current_valid\0loopcount\0hour\0save_slot\0"
    "on_download_clicked\0on_save_clicked_after\0"
    "on_save_clicked\0on_load_clicked\0"
    "on_excute_clicked\0graph_down_replot\0"
    "on_clear_clicked\0test_text\0"
    "on_download_2_clicked\0on_save_2_clicked\0"
    "on_save_2_clicked_after\0on_load_2_clicked\0"
    "on_excute_2_clicked\0graph_down_replot_2\0"
    "on_clear_2_clicked\0test_text_2\0"
    "on_download_3_clicked\0on_save_3_clicked\0"
    "on_save_3_clicked_after\0on_load_3_clicked\0"
    "on_excute_3_clicked\0graph_down_replot_3\0"
    "on_clear_3_clicked\0test_text_3\0"
    "on_download_4_clicked\0on_save_4_clicked\0"
    "on_save_4_clicked_after\0on_load_4_clicked\0"
    "on_excute_4_clicked\0graph_down_replot_4\0"
    "on_clear_4_clicked\0test_text_4\0"
    "on_download_5_clicked\0on_save_5_clicked\0"
    "on_save_5_clicked_after\0on_load_5_clicked\0"
    "on_excute_5_clicked\0graph_down_replot_5\0"
    "on_clear_5_clicked\0test_text_5\0tab_1_ini\0"
    "tab_2_ini\0tab_3_ini\0tab_4_ini\0tab_5_ini\0"
    "stop_excute\0clear_excute\0on_action_triggered"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      67,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  349,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,  350,    2, 0x08 /* Private */,
       5,    1,  353,    2, 0x08 /* Private */,
       7,    0,  356,    2, 0x08 /* Private */,
       8,    0,  357,    2, 0x08 /* Private */,
       9,    1,  358,    2, 0x08 /* Private */,
      11,    0,  361,    2, 0x08 /* Private */,
      12,    0,  362,    2, 0x08 /* Private */,
      13,    0,  363,    2, 0x08 /* Private */,
      14,    0,  364,    2, 0x08 /* Private */,
      15,    0,  365,    2, 0x08 /* Private */,
      16,    0,  366,    2, 0x08 /* Private */,
      17,    1,  367,    2, 0x08 /* Private */,
      20,    0,  370,    2, 0x08 /* Private */,
      21,    0,  371,    2, 0x08 /* Private */,
      22,    0,  372,    2, 0x08 /* Private */,
      23,    0,  373,    2, 0x08 /* Private */,
      24,    6,  374,    2, 0x08 /* Private */,
      31,    0,  387,    2, 0x08 /* Private */,
      32,    0,  388,    2, 0x08 /* Private */,
      33,    0,  389,    2, 0x08 /* Private */,
      34,    0,  390,    2, 0x08 /* Private */,
      35,    0,  391,    2, 0x08 /* Private */,
      36,    0,  392,    2, 0x08 /* Private */,
      37,    0,  393,    2, 0x08 /* Private */,
      38,    0,  394,    2, 0x08 /* Private */,
      39,    0,  395,    2, 0x08 /* Private */,
      40,    0,  396,    2, 0x08 /* Private */,
      41,    0,  397,    2, 0x08 /* Private */,
      42,    0,  398,    2, 0x08 /* Private */,
      43,    0,  399,    2, 0x08 /* Private */,
      44,    0,  400,    2, 0x08 /* Private */,
      45,    0,  401,    2, 0x08 /* Private */,
      46,    0,  402,    2, 0x08 /* Private */,
      47,    0,  403,    2, 0x08 /* Private */,
      48,    0,  404,    2, 0x08 /* Private */,
      49,    0,  405,    2, 0x08 /* Private */,
      50,    0,  406,    2, 0x08 /* Private */,
      51,    0,  407,    2, 0x08 /* Private */,
      52,    0,  408,    2, 0x08 /* Private */,
      53,    0,  409,    2, 0x08 /* Private */,
      54,    0,  410,    2, 0x08 /* Private */,
      55,    0,  411,    2, 0x08 /* Private */,
      56,    0,  412,    2, 0x08 /* Private */,
      57,    0,  413,    2, 0x08 /* Private */,
      58,    0,  414,    2, 0x08 /* Private */,
      59,    0,  415,    2, 0x08 /* Private */,
      60,    0,  416,    2, 0x08 /* Private */,
      61,    0,  417,    2, 0x08 /* Private */,
      62,    0,  418,    2, 0x08 /* Private */,
      63,    0,  419,    2, 0x08 /* Private */,
      64,    0,  420,    2, 0x08 /* Private */,
      65,    0,  421,    2, 0x08 /* Private */,
      66,    0,  422,    2, 0x08 /* Private */,
      67,    0,  423,    2, 0x08 /* Private */,
      68,    0,  424,    2, 0x08 /* Private */,
      69,    0,  425,    2, 0x08 /* Private */,
      70,    0,  426,    2, 0x08 /* Private */,
      71,    0,  427,    2, 0x08 /* Private */,
      72,    0,  428,    2, 0x08 /* Private */,
      73,    0,  429,    2, 0x08 /* Private */,
      74,    0,  430,    2, 0x08 /* Private */,
      75,    0,  431,    2, 0x08 /* Private */,
      76,    0,  432,    2, 0x08 /* Private */,
      77,    0,  433,    2, 0x08 /* Private */,
      78,    0,  434,    2, 0x08 /* Private */,
      79,    0,  435,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void, 0x80000000 | 18,   19,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Int, QMetaType::Int, QMetaType::Int,   25,   26,   27,   28,   29,   30,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->WriteTimestop(); break;
        case 1: _t->delaymsec((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->timerEvent((*reinterpret_cast< QTimerEvent*(*)>(_a[1]))); break;
        case 3: _t->on_actionExit_triggered(); break;
        case 4: _t->on_actionSetting_triggered(); break;
        case 5: _t->setpage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->on_actionsave_all_triggered(); break;
        case 7: _t->on_actionRestore_All_triggered(); break;
        case 8: _t->on_actionBackup_ALL_triggered(); break;
        case 9: _t->on_actionLoad_ALL_triggered(); break;
        case 10: _t->on_actionAuto_triggered(); break;
        case 11: { bool _r = _t->dataBackup();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 12: _t->closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        case 13: _t->serial_end(); break;
        case 14: _t->rule_extend(); break;
        case 15: _t->ReadData(); break;
        case 16: _t->serial_close(); break;
        case 17: _t->save_data((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 18: _t->save_slot(); break;
        case 19: _t->on_download_clicked(); break;
        case 20: _t->on_save_clicked_after(); break;
        case 21: _t->on_save_clicked(); break;
        case 22: _t->on_load_clicked(); break;
        case 23: _t->on_excute_clicked(); break;
        case 24: _t->graph_down_replot(); break;
        case 25: _t->on_clear_clicked(); break;
        case 26: _t->test_text(); break;
        case 27: _t->on_download_2_clicked(); break;
        case 28: _t->on_save_2_clicked(); break;
        case 29: _t->on_save_2_clicked_after(); break;
        case 30: _t->on_load_2_clicked(); break;
        case 31: _t->on_excute_2_clicked(); break;
        case 32: _t->graph_down_replot_2(); break;
        case 33: _t->on_clear_2_clicked(); break;
        case 34: _t->test_text_2(); break;
        case 35: _t->on_download_3_clicked(); break;
        case 36: _t->on_save_3_clicked(); break;
        case 37: _t->on_save_3_clicked_after(); break;
        case 38: _t->on_load_3_clicked(); break;
        case 39: _t->on_excute_3_clicked(); break;
        case 40: _t->graph_down_replot_3(); break;
        case 41: _t->on_clear_3_clicked(); break;
        case 42: _t->test_text_3(); break;
        case 43: _t->on_download_4_clicked(); break;
        case 44: _t->on_save_4_clicked(); break;
        case 45: _t->on_save_4_clicked_after(); break;
        case 46: _t->on_load_4_clicked(); break;
        case 47: _t->on_excute_4_clicked(); break;
        case 48: _t->graph_down_replot_4(); break;
        case 49: _t->on_clear_4_clicked(); break;
        case 50: _t->test_text_4(); break;
        case 51: _t->on_download_5_clicked(); break;
        case 52: _t->on_save_5_clicked(); break;
        case 53: _t->on_save_5_clicked_after(); break;
        case 54: _t->on_load_5_clicked(); break;
        case 55: _t->on_excute_5_clicked(); break;
        case 56: _t->graph_down_replot_5(); break;
        case 57: _t->on_clear_5_clicked(); break;
        case 58: _t->test_text_5(); break;
        case 59: _t->tab_1_ini(); break;
        case 60: _t->tab_2_ini(); break;
        case 61: _t->tab_3_ini(); break;
        case 62: _t->tab_4_ini(); break;
        case 63: _t->tab_5_ini(); break;
        case 64: _t->stop_excute(); break;
        case 65: _t->clear_excute(); break;
        case 66: _t->on_action_triggered(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::WriteTimestop)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 67)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 67;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 67)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 67;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::WriteTimestop()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
