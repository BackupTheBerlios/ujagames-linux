// Soundmodule NAS

#include "lala.h"

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
    { case 0: sf=sf+"mond.wav"; break;
      case 1: sf=sf+"spinne1.wav"; break;
      case 2: sf=sf+"miau.wav"; break;
      case 3: sf=sf+"boing.wav"; break;
      case 4: sf=sf+"ghoul.wav"; break;
      case 5: sf=sf+"mauer.wav"; break;
      case 6: sf=sf+"au.wav"; break;
      case 7: sf=sf+"kuerbis.wav"; break;
      case 8: sf=sf+"hexe.wav"; break;
      case 9: sf=sf+"geist.wav"; break;
      case 10: sf=sf+"bat.wav"; break;
      case 11: sf=sf+"nachladen.wav"; break;
      case 12: sf=sf+"schuss.wav"; break;
      case 13: sf=sf+"flop.wav"; break;
      case 14: sf=sf+"floh.wav"; break;
      case 15: sf=sf+"skorpion.wav"; break;
      case 16: sf=sf+"witch.wav"; break;
      case 17: sf=sf+"nein.wav"; break;
      case 18: sf=sf+"ratte.wav"; break;
      case 19: sf=sf+"ziels.wav"; break;
      case 20: sf=sf+"flaemmchen.wav"; break;
    }
    sou[i]=new QSound(sf);
  }
}

// slot:
void lala::mach_lala(bool sound_on,int nr) { if (sound_on) if (QSound::isAvailable()) sou[nr]->play(); }

