// Header aRts-Sound  fuer Kevin Kramser aRts-Plugin


#include "arts.h"

#ifndef LALA_H
#define LALA_H 

class lala:public QObject
{ 
  Q_OBJECT
	
  public:
  lala();

  private:
  const static int num_sounds=8;
  QString sou[num_sounds]; // Unterschied zu NAS
  
  public slots:
  void mach_lala(bool,int);
};

#endif
// ----------------------------------
