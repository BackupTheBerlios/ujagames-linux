//#include <qsound.h>
#include <qsound.h>
#ifndef LALA_H
#define LALA_H 

class lala:public QObject
{ 
  Q_OBJECT
	
  public:
  lala();

  void lade_sounds();

  private:
  QSound* sou[20];
  
  public slots:
  void mach_lala(bool,int);
};

#endif
// ----------------------------------
