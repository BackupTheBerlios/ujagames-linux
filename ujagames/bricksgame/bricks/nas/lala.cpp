// Soundmodule NAS

#include "lala.h"
#include <stdio.h>

// ======== sounds: =========================================================

lala::lala()
{ QString sf;
  // printf("setze nas-sound\n"); 
  if (!QSound::isAvailable()) printf("No soundserver found - no sound!\n");
  // Sounds muessen trotzdem erstellt werden sonst crash beim game
  // Qt-lib faengt den nosoundserver auf.
  for (int i=0; i<num_sounds; i++)
  { sf="./sounds/";
    switch (i)
    { case 0: sf=sf+"gong.wav"; break;        // Start
      case 1: sf=sf+"paddel.wav"; break;
      case 2: sf=sf+"brick.wav"; break;
      case 3: sf=sf+"floater.wav"; break;
      case 4: sf=sf+"goodie.wav"; break; // goodie gefangen
      case 5: sf=sf+"lazer.wav"; break;
      case 6: sf=sf+"nachladen.wav"; break; // Level over
      case 7: sf=sf+"nachladen.wav"; break; // gameover
    }
    sou[i]=new QSound(sf);
  }
}

// slot:
void lala::mach_lala(bool sound_on,int nr) { if (sound_on) if (QSound::isAvailable()) sou[nr]->play(); }

