#########################################################################
# spooky vers 1.00 - Internationalisierung - NAS-soundserver 12.02.2005
#########################################################################

TEMPLATE = app
TARGET = spooky

INCLUDEPATH += . ./german

# CONFIG += qt debug

# Input
HEADERS +=  german/lang.h config.h gamelib.h window.h wiese.h lala.h 
SOURCES +=  main_nas.cpp gamelib.cpp window.cpp wiese.cpp lala.cpp 

##################################################################################