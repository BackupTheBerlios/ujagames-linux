# bricks.pro - generiert von bash
##################################
TEMPLATE = app
TARGET = bricks

INCLUDEPATH += . ./german ./arts ./arts_plugin /usr/kde/3.4/include/arts/
LIBS        += -lsoundserver_idl -L/usr/kde/3.4/lib/

CONFIG += qt

# Input
HEADERS +=  german/lang.h config.h game.h gamelib.h levels.h window.h arts/lala.h arts_plugin/arts.h
SOURCES +=  main.cpp game.cpp gamelib.cpp window.cpp levels.cpp arts/lala.cpp arts_plugin/arts.cpp
