// Soundmodule aRts 2

#include "lala.h"

// ======== sounds: =========================================================
bool init_QSound();

lala::lala() 
{ QString sf;
  // printf("setze arts-sound\n"); 
  if (!init_QSound()>0) printf("aRts-Soundserver not found, trying NAS!\n"); 
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
    sou[i]=sf;
  }
}

// slot, Unterschiedlich zu NAS:
void lala::mach_lala(bool sound_on,int nr) { if (sound_on) if (QSound::isAvailable()) QSound::play(sou[nr]); } 

