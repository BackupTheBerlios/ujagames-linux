#################################################################################
# spooky vers 1.00 - Internationalisierung - aRts soundserver 15.02.2005
#################################################################################

TEMPLATE = app
TARGET = spooky

INCLUDEPATH += . ./english ./arts_plugin $(ARTSINCLUDE)
LIBS        += -lsoundserver_idl -L$(KDELIBS)

# CONFIG += qt debug

# Input
HEADERS +=  english/lang.h config.h gamelib.h window.h wiese.h lala_arts.h arts_plugin/arts.h
SOURCES +=  main.cpp gamelib.cpp window.cpp wiese.cpp lala_arts.cpp arts_plugin/arts.cpp

##################################################################################
