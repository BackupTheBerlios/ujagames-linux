/***********************************************
Levels fuer sbricks (c) uja
***********************************************/
#ifndef LEVELS
#define LEVELS

#include "config.h"
#include <qfile.h>
#include <qstring.h>


// Einzellevel:
class leveldata
{ 
  public:
  QString titel;
  QString levelcode;
  int f0[SMAX];
  void set_level(QString*);
  
};

// Gesamtheit der Levels:
class level
{ 
  public:
  level::level(QString levelfile);
  leveldata* my_level[MAXLEVELS];
  int anz_levels;
};

#endif
