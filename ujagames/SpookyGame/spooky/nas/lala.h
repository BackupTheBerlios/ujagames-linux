//#include <qsound.h>
#include <qsound.h>
#ifndef LALA_H
#define LALA_H 

class lala:public QObject
{ 
  Q_OBJECT	
  
  public:
  lala();
  
  private:
  const static int num_sounds=21;
  QSound* sou[num_sounds];

  public slots:
  void mach_lala(bool,int);
};

#endif
// ----------------------------------
