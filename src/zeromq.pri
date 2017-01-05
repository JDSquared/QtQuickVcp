include(../paths.pri)
include(../3rdparty/nzmqt/nzmqt.pri)

!isEmpty(ZEROMQ_INCLUDE_PATH): INCLUDEPATH += $$ZEROMQ_INCLUDE_PATH
!isEmpty(ZEROMQ_LIB_PATH): LIBS += -L$$ZEROMQ_LIB_PATH
!isEmpty(ZEROMQ_LIB_FLAGS): LIBS += $$ZEROMQ_LIB_FLAGS

!win32: LIBS += -lzmq
win32: CONFIG(debug, debug|release) {
    LIBS += -llibzmq_d
}
else {
    LIBS += -llibzmq
}

ios: {
    LIBS += -L$$LIBSODIUM_LIB_PATH
    LIBS += -Bstatic -lsodium
}
