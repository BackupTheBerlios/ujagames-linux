#################################################################################
# spooky vers 1.00 - Internationalisierung - NAS or no soundserver 14.02.2005
#################################################################################

TEMPLATE = app
TARGET = spooky

INCLUDEPATH += . ./english 

# CONFIG += qt debug

# Input english/lang.h config.h gamelib.h window.h wiese.h lala.h 
SOURCES +=  main_nas.cpp gamelib.cpp window.cpp wiese.cpp lala.cpp 

##################################################################################
