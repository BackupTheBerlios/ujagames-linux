// Header aRts-Sound  fuer aRts-Plugin von Kevin Krammer


#include "arts.h"

#ifndef LALA_H
#define LALA_H 

class lala:public QObject
{ 
  Q_OBJECT
	
  public:
  lala();

  void lade_sounds();

  private:
  QString sou[20];
  
  public slots:
  void mach_lala(bool,int);
};

#endif
// ----------------------------------
