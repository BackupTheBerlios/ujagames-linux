// Header aRts-Sound  fuer aRts-Plugin von Kevin Krammer


#include "arts.h"

#ifndef LALA_H
#define LALA_H 

class lala:public QObject
{ 
  Q_OBJECT
	
  public:
  lala();

  private:
  const static int num_sounds=21;
  QString sou[num_sounds]; // Unterschied zu NAS
  
  public slots:
  void mach_lala(bool,int);
};

#endif
// ----------------------------------
