TEMPLATE  = lib
CONFIG   += qt warn_on debug thread

HEADERS  += arts.h

SOURCES  += arts.cpp

LANGUAGE  = C++

TARGET    = qds_arts
VERSION   = 0.1.0
DESTDIR   = ../../install/lib

ARTSINCLUDE=$$(ARTSINCLUDE)
isEmpty(ARTSINCLUDE) {
    error( Please set ARTSINCLUDE to point to aRts include directory)
}

ARTSLIBS=$$(ARTSLIBS)
isEmpty(ARTSLIBS) {
    error( Please set ARTSLIBS to point to ARTS lib directory)
}

INCLUDEPATH += $(ARTSINCLUDE)

!isEmpty(ARTSLIBS) {
    LIBS        += -L$(ARTSLIBS) -lsoundserver_idl
}

unix {
  UI_DIR      = .ui
  MOC_DIR     = .moc
  OBJECTS_DIR = .obj
}
