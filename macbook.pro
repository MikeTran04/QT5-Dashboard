QT += core gui widgets webengine webenginewidgets charts

CONFIG += c++17
CONFIG -= app_bundle  # Remove if you want a .app bundle

INCLUDEPATH += headers

SOURCES += \
    src/cpp/dht22lib.cpp \
    src/cpp/sensorworker.cpp \
    src/cpp/hcsr501.cpp \
    src/cpp/main.cpp \
    src/cpp/brightness_control.cpp \
    src/cpp/active_mode.cpp \
    src/cpp/off_mode.cpp \
    src/cpp/dashboard.cpp \
    src/cpp/weeklyWeather.cpp \
    src/cpp/weatherStation.cpp \
    src/cpp/trendGraphPage.cpp \
    src/cpp/outdoorTemp.cpp \
    src/cpp/hourlyWeather.cpp \
    src/cpp/alertPage.cpp \
    src/cpp/piStats.cpp \
    src/cpp/dateTimePage.cpp \
    src/cpp/sensorWidget.cpp \
    src/cpp/WebViewPage.cpp \
    src/cpp/YouTubePage.cpp \
    src/cpp/YouTubeMusicPage.cpp \
    src/cpp/motionSensorPage.cpp \
    src/cpp/statswidget.cpp

HEADERS += \
    headers/dht22lib.h \
    headers/sensorworker.h \
    headers/hcsr501.h \
    headers/dashboard.h \
    headers/brightness_control.h \
    headers/active_mode.h \
    headers/off_mode.h \
    headers/mywebengineview.h \
    headers/weeklyWeather.h \
    headers/weatherStation.h \
    headers/trendGraphPage.h \
    headers/outdoorTemp.h \
    headers/hourlyWeather.h \
    headers/alertPage.h \
    headers/piStats.h \
    headers/lightsensorworker.h \
    headers/dateTimePage.h \
    headers/sensorWidget.h \
    headers/WebViewPage.h \
    headers/YouTubeMusicPage.h \
    headers/YouTubePage.h \
    headers/motionSensorPage.h \
    headers/statswidget.h

RESOURCES += \
    resources/resources.qrc

# Include paths for wiringPi and Qt
INCLUDEPATH += \
    $$PWD/headers \
    /usr/local/include \
    /usr/include/aarch64-linux-gnu/qt5 \
    /usr/include/aarch64-linux-gnu/qt5/QtCore \
    /usr/include/aarch64-linux-gnu/qt5/QtWidgets \
    /usr/include/aarch64-linux-gnu/qt5/QtGui

# Ensure macOS ARM64 build
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.13
QMAKE_CXXFLAGS += -arch arm64
QMAKE_LFLAGS += -arch arm64

# Use macOS-specific toolchain
QMAKE_CC = /usr/bin/clang
QMAKE_CXX = /usr/bin/clang++
QMAKE_LINK = /usr/bin/clang++

# Fix SDK compatibility warning
CONFIG += sdk_no_version_check


