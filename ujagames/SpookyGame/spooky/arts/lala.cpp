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
      case 20: sf=sf+"flamme.wav"; break;
    }
    sou[i]=sf;
  }
}

// slot, Unterschiedlich zu NAS:
void lala::mach_lala(bool sound_on,int nr) { if (sound_on) if (QSound::isAvailable()) QSound::play(sou[nr]); } 

