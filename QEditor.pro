# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

#TARGET = QEditor

QT = core gui widgets

HEADERS = \
   $$PWD/header/EditorWidget.h \
   $$PWD/header/LineNumberArea.h \
   $$PWD/header/MainWindow.h \
   $$PWD/header/msg.h \
   $$PWD/header/SideBar.h \
   $$PWD/header/Utility.h

SOURCES = \
   $$PWD/source/EditorWidget.cpp \
   $$PWD/source/LineNumberArea.cpp \
   $$PWD/source/MainWindow.cpp \
   $$PWD/source/msg.cpp \
   $$PWD/source/SideBar.cpp \
   $$PWD/source/Utility.cpp \
   $$PWD/main.cpp

RESOURCES += \
    defaultResources.qrc
