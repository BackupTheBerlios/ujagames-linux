#########################################################################
# spooky vers 1.00 - Internationalisierung - NAS-soundserver 12.02.2005
#########################################################################

TEMPLATE = app
TARGET = spooky

INCLUDEPATH += . ./german ./nas

CONFIG += qt

# Input
HEADERS +=  german/lang.h config.h gamelib.h window.h wiese.h nas/lala.h 
SOURCES +=  main.cpp gamelib.cpp window.cpp wiese.cpp nas/lala.cpp 

##################################################################################
