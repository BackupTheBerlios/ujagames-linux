/***********************************************
Levels fuer schafe/Lämmerlinge (c) uja
***********************************************/
// Implementierung:

#include "levels.h"
#include <stdio.h>

void leveldata::set_level(QString* buff)
{  QString buff1;
   titel=buff->section(';',0,0);
   buff1=buff->section(';',1,1);   
   levelcode=buff1.stripWhiteSpace();
}

// Levels reinziehen, so wie sie sind, decodiert wird spaeter:
level::level(QString levelfile)
{ int gelesen,buffsize=SMAX+64;
  anz_levels=0;
  QString buff;
  QFile fh(levelfile);
  if (fh.open(IO_ReadOnly))
  { do
    { gelesen=fh.readLine(buff,buffsize);
      if (gelesen>SMAX)    
      { my_level[anz_levels]=new leveldata();
        my_level[anz_levels]->set_level(&buff);
        anz_levels++;
      }
    } while ((gelesen>-1) && (anz_levels<MAXLEVELS));       
    fh.close();    
  } else printf("Levelfile not found!\n");
}
