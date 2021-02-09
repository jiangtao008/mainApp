#-------------------------------------------------
#
# Project created by QtCreator 2020-11-02T22:01:40
#
#-------------------------------------------------

QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = myApp
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11

include(subWindows/subWinAbout/subWinAbout.pri)
include(subWindows/subWinCamera/subWinCamera.pri)
include(subWindows/subWinCar/subWinCar.pri)
include(subWindows/subWinMonitor/subWinMonitor.pri)
include(subWindows/subWinSetting/subWinSetting.pri)
include(subWindows/subWinUserManage/subWinUserManage.pri)

include(comm/comm.pri)

include(raspiDevice/raspiDevice.pri)

#添加包含路劲（ 头文件）
INCLUDEPATH += \
        inputWin \
        raspiDevice \
        subWindows

#添加源文件
SOURCES += \
        main.cpp \
        mainwindow.cpp \
        status.cpp \
        inputWin/inputwin.cpp \
        inputWin/face.cpp \
        inputWin/fpc1020a.cpp \
        inputWin/password.cpp \

#添加头文件
HEADERS += \
        mainwindow.h \
        status.h \
        inputWin/inputwin.h \
        inputWin/face.h \
        inputWin/fpc1020a.h \
        inputWin/password.h \

#界面文件
FORMS += \
        inputWin/inputwin.ui \
        mainwindow.ui \
        status.ui \

INCLUDEPATH += /usr/include/opencv \
                /usr/include/opencv2

LIBS += -lwiringPi
LIBS += /usr/lib/arm-linux-gnueabihf/libopencv_highgui.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_core.so    \
        /usr/lib/arm-linux-gnueabihf/libopencv_imgcodecs.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_imgproc.so  \
        /usr/lib/arm-linux-gnueabihf/libopencv_videoio.so

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc





