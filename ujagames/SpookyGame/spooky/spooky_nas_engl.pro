# spooky, NAS, english
##################################
TEMPLATE = app
TARGET = spooky

INCLUDEPATH += . ./english ./nas

# CONFIG += qt debug

# Input
HEADERS +=  english/lang.h config.h gamelib.h window.h wiese.h nas/lala.h
SOURCES +=  main.cpp gamelib.cpp window.cpp wiese.cpp nas/lala.cpp
