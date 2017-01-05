MACHINETALK_PATH = $$OUT_PWD/../machinetalk
INCLUDEPATH += $$PWD

!win32 {
    LIBS += -L$$MACHINETALK_PATH
} else {
    CONFIG(debug, debug|release) {
        LIBS += -L$$MACHINETALK_PATH/debug
    }
    else {
        LIBS += -L$$MACHINETALK_PATH/release
    }
}
#macx: {
#    LIBS += -Bstatic
#}
LIBS += -lmachinetalk
