/************************************************************************************************************
 This file ist part of ujas Spooky Shooter which is published under GPLv2
 27.01.2005: V.0.80: first playable version 
 13.02.2005: V.0.9:  added language files, more sprites, more landscape, scrolling, aRts-Sound, key handles
 to do: add rats and garbacge can, update language files
*************************************************************************************************************/

#include "wiese.h"
#include <math.h>
#include "stdio.h"


flieger::flieger(int i) { id=i; z=id; x=0; y=0; vx=0; vy=0; bildbase=0; basewav=0; phase=-1; }
void flieger::set_koords(double xx,double yy) { x=xx; y=yy; p_sprite->move(x,y); }
void flieger::set_tiefe(double zz) { z=zz; p_sprite->setZ(z); }
void flieger::move_it() { x=x+vx; y=y+vy; p_sprite->move(x,y); }

// ===============================================================================================================

wiese::wiese(QWidget* parent)
{ int i;
// printf("erstelle Objekt wiese\n");
  oberer=parent;
  aktiv=false;
  pausiert=false;

  mond  =new flieger(0); mond->vx=1; mond->y=0; mond->y=OFY+8;
  spinne=new flieger(1); spinne->vy=4; spinne->z=31; spinne->phase=-1;
  katze =new flieger(2); katze->z=21;
  waber=new flieger(3);  waber->z=21;
  clown=new flieger(4);  clown->z=21;

  // Ebenen 2-9,12-19,22-29: Bahnen fuer Hexen, Geiser, Kuerbisse:
  for (i=0; i<4; i++) { hexe[i]=new flieger(i+5); kuerbis[i]=new flieger(i+9); }
  for (i=0; i<8; i++) { bat[i]=new flieger(i+13); geist[i]=new flieger(i+21);  }
  

// printf("Level 2:\n");
  pilz=new flieger(29); 
  floh=new flieger(30); 
  for (i=0; i<3; i++) skorpion[i]=new flieger(31+i);
  skorpion[0]->y=double(YMAX-64);  skorpion[0]->z=27.8;
  skorpion[1]->y=double(YMAX-128); skorpion[1]->z=23.8;
  skorpion[2]->y=double(YMAX-144); skorpion[2]->z=17.8;
 
  // level 3:
  monster=new flieger(33);
  monster->z=29.8; // direkt hinter Trog
  
   
  for (i=0; i<40; i++) psprite[i]=new flieger(40+i); 
  punkte=0;
  tic=35;
  
// printf("starte Timer\n");
  // Sekundenticker und Ani-Ticker: ---  
  timrec1=new QTimer(); connect(timrec1,SIGNAL(timeout()),this,SLOT(move_mond())); timrec1->start(200);
  timrec2=new QTimer(); connect(timrec1,SIGNAL(timeout()),this,SLOT(move_all())); timrec1->start(35);
//  printf("feddich!\n");
}

void wiese::move_all()
{ if (aktiv)
  { int i;
    if (spinne->phase<0) { if (rand()%1000<1) starte_spinne(); } else move_spinne();
    if (katze->phase<0)  { if (rand()%100<1)  starte_katze(); }  else move_katze();
    move_waber();
    move_clown();
    move_floh();
    move_monster();
    if (rand()%500<1) starte_kuerbis(); for (i=0; i<4; i++) if (kuerbis[i]->phase>=0) move_kuerbis(i);
    if (rand()%250<1) starte_hexe();    for (i=0; i<4; i++) if (hexe[i]->phase>=0) move_hexe(i);
    if (rand()%250<1) starte_geist();   for (i=0; i<8; i++) if (geist[i]->phase>=0) move_geist(i);
    for (i=0; i<8; i++) if (bat[i]->phase>=0) move_bat(i);
    for (i=0; i<3; i++) move_skorpion(i);
    for (i=0; i<40; i++) 
    { if (psprite[i]->phase>=0) psprite[i]->phase--;
      if (psprite[i]->phase<0) psprite[i]->p_sprite->hide();
    }  
  }
}  


// Mond wandert langsam über den Himmel, 2.Durchgang mit Sternbild Orion, 3. mit Venus, danach gameover:
// -------------------------------------------------------------------------------------------------------------
void wiese::move_mond() 
{ if (aktiv) 
  { mond->move_it(); 
    if (mond->x>XMAX) 
    { mond->phase++; 
      if (waber->phase>=0) { waber->phase=-1; waber->p_sprite->hide(); }
      waber->count=0;
      if (mond->phase<3) { mond->set_koords(-OFX,8); mond->p_sprite->setFrame(mond->phase); } else game_over();
    }
  }
}

void wiese::mond_getroffen() { emit play_lala(sound_on,0); for (int i=0; i<8; i++) starte_bat(); }


// rund um die Spinne: Bewegungsmuster: erst runter, dann hoch mit je Speed 8 ----------------------------
// -------------------------------------------------------------------------------------------------------
void wiese::starte_spinne() 
{ int k=rand()%XMAX; 
  spinne->vx=0; spinne->vy=8; 
  spinne->set_koords(k,-OFY); spinne->p_sprite->show();
  spinne->phase=0;
}

void wiese::move_spinne() 
{ if (spinne->phase>=0)
  { spinne->move_it();
    if ((spinne->vy>0) && (spinne->y>YMAX)) spinne->vy=-8;
    else if ((spinne->vy<0) && (spinne->y<-OFY)) { spinne->phase=-1; spinne->p_sprite->hide(); }
  }  
}

void wiese::spinne_getroffen()
{ int gain=100;
  punkte=punkte+gain;
  spinne->phase=-1; spinne->p_sprite->hide();
  emit punkte_changed(punkte);
  zeige_punkte(spinne,gain);
  emit play_lala(sound_on,1);
}
// ---------------------------------------------------------------------------------------------------------

// rund um die Katze, sie erscheint an den 13 Fenstern des Schlosses und verscwindet wieder:
// ------------------------------------------------------------------------------------------
void wiese::starte_katze() 
{ int kx[13]={288,340, 196,246,296,342,382, 144,132,144, 448,436,448}; 
  int ky[13]={168,168, 216,216,216,216,216, 164,210,268, 194,224,276};
  int k=rand()%13; 
  if (k<2) katze->p_sprite->setZ(11); else katze->p_sprite->setZ(21);
  katze->set_koords(kx[k],ky[k]); katze->p_sprite->show();
  katze->phase=50;
}

void wiese::move_katze() // Countdown auf Phase
{ if (katze->phase>=0)
  { katze->phase--;
    if (katze->phase<0) katze->p_sprite->hide();
  }   
}

void wiese::katze_getroffen()
{ int gain=50;
  katze->phase=-1; katze->p_sprite->hide();
  punkte=punkte+gain;
  emit punkte_changed(punkte);
  emit play_lala(sound_on,2);
  zeige_punkte(katze,gain);

}
// ---------------------------------------------------------------------------------------------------------

// Waberpott und Ghoul, Wabern (4 Phasen) wird durch Topfcount gestartet, 
// bei 3 weiteren Treffern in den Schaum steigt der Ghoul auf mit Speed 8
// Der Waberpott kann 1x pro Mond-Durchlauf gestartet werden
// ----------------------------------------------------------------------------------------------------------
void wiese::starte_waber()
{ double xx=(topf->x()),yy=(topf->y())-16;
  waber->count=0;
  waber->phase=200; 
  waber->set_koords(xx,yy);
  waber->p_sprite->setFrame(0); 
  waber->p_sprite->show(); 
}

void wiese:: move_waber()
{ if (waber->phase>=0)
  { if (waber->count<3)
    { waber->phase--;
      int iw=(waber->phase/2)%4;
      if (waber->phase<0) waber->p_sprite->hide(); else waber->p_sprite->setFrame(iw);
    }
    else 
    { waber->move_it(); 
      if (waber->y<-OFY) { waber->phase=-1; waber->p_sprite->hide(); }
    }  
  }  
}

void wiese::pott_getroffen()
{ if (waber->phase>=0)
  { waber->count++;
    if (waber->count>2) { waber->vx=0; waber->vy=-8; waber->p_sprite->setFrame(4); }
    emit play_lala(sound_on,3);
  }
}

void wiese::ghoul_getroffen()
{ int gain=500;
  punkte=punkte+gain;
  emit punkte_changed(punkte);
  waber->phase=-1; 
  waber->p_sprite->hide();
  emit play_lala(sound_on,4); 
  zeige_punkte(waber,gain);
}

// ------------------------------------------------------------------------------------------------------------

// Clown: ausgelöst durch Schuss an Tür, erscheint auf Turmspitzen, wackelt a la Jack-in-the-Box:
// ------------------------------------------------------------------------------------------------
void wiese::starte_clown()
{ tuer->setFrame(1);
  emit play_lala(sound_on,5);
  clown->phase=32; 
  int k=rand()%2;
  clown->set_koords(schloss[2+k]->x(),schloss[2+k]->y()-48);
  clown->p_sprite->setFrame(2); 
  clown->p_sprite->show(); 
}

void wiese:: move_clown()
{ if (clown->phase>=0)
  { clown->phase--;
    int iw=(clown->phase/2)%4;
    if (clown->phase<0) { clown->p_sprite->hide(); tuer->setFrame(0); } else clown->p_sprite->setFrame(iw);
  }  
}

void wiese::clown_getroffen()
{ int gain=20;
  punkte=punkte+gain;
  emit punkte_changed(punkte);
  clown->phase=-1; 
  clown->p_sprite->hide();
  tuer->setFrame(0); 
  emit play_lala(sound_on,6);
  zeige_punkte(clown,gain);
}

// ===============================================================================================================
// ab hier die "richtigen" Flugsprites, die auf 24 Bahnen zu 3 Tiefengruppen herumschwirren: Hexe, Geist, Kuerbis
// ---------------------------------------------------------------------------------------------------------------

// Aussuchen einer freien Bahn aus einer der 3 Gruppen - 
// 1. freie Bahn in Gruppen bestimmen, 2. wenn mehrere Gruppen freie Bahnen haben, Zufallsgruppe wählen
// ----------------------------------------------------------------------------------------------------
int wiese::freie_bahn()
{ int i,k,k1=-1,k2=-1,k3=-1;
  bool frei[32];
  for (i=0; i<32; i++) frei[i]=true;  
  for (i=0; i<4; i++)
  { k=int(hexe[i]->p_sprite->z());    if (hexe[i]->phase>=0)    frei[k]=false;
    k=int(kuerbis[i]->p_sprite->z()); if (kuerbis[i]->phase>=0) frei[k]=false;
  }  
  for (i=0; i<8; i++)
  { k=int(bat[i]->p_sprite->z());   if (bat[i]->phase>=0) frei[k]=false;
    k=int(geist[i]->p_sprite->z()); if (geist[i]->phase>=0) frei[k]=false;
  }  
  for (i=2; i<10; i++) 
  { if (frei[i]) k1=i;
    if (frei[i+10]) k2=i+10;
    if (frei[i+20]) k3=i+20;
  }  
  if (k2<0) { k2=k3; k3=-1; }
  if (k1<0) { k1=k2; k2=k3; k3=k1; }
  if ((k1>=0) && (k2>=0) && (k3>=0)) { i=rand()%3; if (i==0) i=k1; else if (i==1) i=k2; else i=k3; }
  else if ((k1>=0) && (k2>=0)) { i=rand()%2; if (i==0) i=k1; else i=k2; }
  else if (k1>=0) i=k1;
  else i=-1;
  return i;
}
// --------------------------------------
int wiese::flughoehe(int bahn)
{ int k=OFY; 
  if (bahn<10) k=k+rand()%160; else if (bahn<20) k=k+rand()%192; else k=k+rand()%216;
  return k;
}


// Kuerbis: einmal waagerecht durch die Landschaft, dabei rollen
// ----------------------------------------------------------------
int wiese::freier_kuerbis()
{ int i,k=-1;
  for (i=0; i<4; i++) if (kuerbis[i]->phase<0) k=i;
  return k;
}

void wiese::starte_kuerbis()
{ int nr=freier_kuerbis(),bahn=freie_bahn();
  if ((nr>=0) && (bahn>=0))
  { kuerbis[nr]->z=bahn; 
    kuerbis[nr]->p_sprite->setZ(bahn); kuerbis[nr]->set_koords((XMAX+OFX)*(rand()%2),flughoehe(bahn));
    if (bahn<10)      { kuerbis[nr]->bildbase=0;  kuerbis[nr]->vx=2; }
    else if (bahn<20) { kuerbis[nr]->bildbase=8;  kuerbis[nr]->vx=3; }
    else              { kuerbis[nr]->bildbase=16; kuerbis[nr]->vx=4; }
    if (kuerbis[nr]->x>0) kuerbis[nr]->vx=-kuerbis[nr]->vx;
    kuerbis[nr]->phase=0;
    kuerbis[nr]->count=0;
    kuerbis[nr]->p_sprite->setFrame(kuerbis[nr]->phase+kuerbis[nr]->bildbase); 
    kuerbis[nr]->p_sprite->show(); 
  }  
}

void wiese:: move_kuerbis(int nr)
{ if (kuerbis[nr]->phase>=0)
  { kuerbis[nr]->count++;
    if (kuerbis[nr]->count%1==0) // Drehphase
    { if (kuerbis[nr]->vx>0) kuerbis[nr]->phase++; else kuerbis[nr]->phase=kuerbis[nr]->phase+7;
      kuerbis[nr]->phase=kuerbis[nr]->phase%8;
      kuerbis[nr]->p_sprite->setFrame(kuerbis[nr]->phase+kuerbis[nr]->bildbase); 
    }  
    kuerbis[nr]->move_it();
    if (kuerbis[nr]->x>XMAX) if (kuerbis[nr]->vx>0) { kuerbis[nr]->phase=-1; kuerbis[nr]->p_sprite->hide(); }
    if (kuerbis[nr]->x<-OFX) if (kuerbis[nr]->vx<0) { kuerbis[nr]->phase=-1; kuerbis[nr]->p_sprite->hide(); }
  }  
}

void wiese::kuerbis_getroffen(int nr)
{ int gain=0;
  switch(kuerbis[nr]->bildbase)
  { case 0: gain=20; break;
    case 8: gain=10; break;
    case 16: gain=5; break;
  } 
  punkte=punkte+gain; 
  emit punkte_changed(punkte);
  kuerbis[nr]->phase=-1; 
  kuerbis[nr]->p_sprite->hide(); 
  emit play_lala(sound_on,7);
  zeige_punkte(kuerbis[nr],gain);

}

// Hexen: einmal waagerecht durch die Landschaft
// ----------------------------------------------------------------
int wiese::freie_hexe()
{ int i,k=-1;
  for (i=0; i<4; i++) if (hexe[i]->phase<0) k=i;
  return k;
}

void wiese::starte_hexe()
{ int nr=freie_hexe(),bahn=freie_bahn(),fh=flughoehe(bahn);
  // Platz für Schwingung schaffen:
  if (fh<OFY) fh=OFY;
  if (fh>YMAX-OFY) fh=YMAX-OFY;
  
  if ((nr>=0) && (bahn>=0))
  { hexe[nr]->phase=rand()%2;
    hexe[nr]->z=bahn; hexe[nr]->set_tiefe(bahn); hexe[nr]->set_koords((XMAX+OFX)*hexe[nr]->phase,fh);
    if (bahn<10)      { hexe[nr]->bildbase=0; hexe[nr]->vx=4; }
    else if (bahn<20) { hexe[nr]->bildbase=2; hexe[nr]->vx=5; }
    else              { hexe[nr]->bildbase=4; hexe[nr]->vx=6; }
    if (hexe[nr]->phase>0) hexe[nr]->vx=-hexe[nr]->vx;    
    hexe[nr]->count=0;
    hexe[nr]->p_sprite->setFrame(hexe[nr]->phase+hexe[nr]->bildbase); 
    hexe[nr]->p_sprite->show(); 
  }  
}

void wiese::move_hexe(int nr)
{ if (hexe[nr]->phase>=0)
  { hexe[nr]->count++;
    double yy=double(hexe[nr]->vx)*sin(double(hexe[nr]->count)*7.0/88.0)/4+hexe[nr]->y;
    hexe[nr]->x=hexe[nr]->x+hexe[nr]->vx;
    hexe[nr]->set_koords(hexe[nr]->x,yy);
    if (hexe[nr]->x>XMAX+OFX) if (hexe[nr]->vx>0) { hexe[nr]->phase=-1; hexe[nr]->p_sprite->hide(); }
    if (hexe[nr]->x<0)   if (hexe[nr]->vx<0) { hexe[nr]->phase=-1; hexe[nr]->p_sprite->hide(); }
  }  
}

void wiese::hexe_getroffen(int nr)
{ int gain=-100;
  punkte=punkte+gain;
  emit punkte_changed(punkte);
  hexe[nr]->phase=-1; 
  hexe[nr]->p_sprite->hide(); 
  emit play_lala(sound_on,8);
  zeige_punkte(hexe[nr],gain);
}


// Geist: einmal waagerecht durch die Landschaft, dabei flattern
// ----------------------------------------------------------------
int wiese::freier_geist()
{ int i,k=-1;
  for (i=0; i<4; i++) if (geist[i]->phase<0) k=i;
  return k;
}

void wiese::starte_geist()
{ int nr=freier_geist(),bahn=freie_bahn(),fh=flughoehe(bahn);
  if (bahn<20) if (fh>OFY) fh=OFY;
  if ((nr>=0) && (bahn>=0))
  { geist[nr]->z=bahn; 
    geist[nr]->set_tiefe(bahn); geist[nr]->set_koords(XMAX*(rand()%2),fh);
    if (bahn<10)      { geist[nr]->bildbase=0;  geist[nr]->vx=1; }
    else if (bahn<20) { geist[nr]->bildbase=1;  geist[nr]->vx=2; }
    else              { geist[nr]->bildbase=2;  geist[nr]->vx=3; }
    if (geist[nr]->x>0) geist[nr]->vx=-geist[nr]->vx;
    geist[nr]->phase=0;
    geist[nr]->count=0;
    geist[nr]->p_sprite->setFrame(geist[nr]->phase+geist[nr]->bildbase); 
    geist[nr]->p_sprite->show(); 
  }  
}

void wiese:: move_geist(int nr)
{ if (geist[nr]->phase>=0)
  { geist[nr]->count++;
    geist[nr]->vy=4*cos(double(geist[nr]->count)/24);
    geist[nr]->move_it();
    if (geist[nr]->x>XMAX) if (geist[nr]->vx>0) { geist[nr]->phase=-1; geist[nr]->p_sprite->hide(); }
    if (geist[nr]->x<-OFX) if (geist[nr]->vx<0) { geist[nr]->phase=-1; geist[nr]->p_sprite->hide(); }
  }  
}

void wiese::geist_getroffen(int nr)
{ int gain=0;
  switch (geist[nr]->bildbase)
  { case 0: gain=20; break;
    case 1: gain=10; break;
    case 2: gain=5; break;
  } 
  punkte=punkte+gain; 
  emit punkte_changed(punkte);
  geist[nr]->phase=-1; 
  geist[nr]->p_sprite->hide(); 
  emit play_lala(sound_on,9);
  zeige_punkte(geist[nr],gain);

}

// ----------------------------------------------------------------------------------------------------------------
// 8 Bats: Start aus Mond, Flug schräg, dabei nach vorne kommen
// -----------------------------------------------------------------------------------
int wiese::freier_bat()
{ int i,k=-1;
  for (i=0; i<8; i++) if (bat[i]->phase<0) k=i;
  return k;
}

void wiese::starte_bat()
{ int nr=freier_bat();
  if (nr>=0)
  { bat[nr]->phase=0;
    bat[nr]->set_tiefe(0.5+nr*0.05); bat[nr]->set_koords(mond->x+32,mond->y+64);
    bat[nr]->vy=double((rand()%8))/8+1;
    bat[nr]->vx=rand()%2; if (bat[nr]->vx==0) bat[nr]->vx=-1;
    bat[nr]->vx=bat[nr]->vx*nr/4;
    bat[nr]->p_sprite->setFrame(0); 
    bat[nr]->p_sprite->show(); 
  }  
}

void wiese:: move_bat(int nr)
{ if (bat[nr]->phase>=0)
  { bat[nr]->move_it();
    if ((bat[nr]->y>240) && (bat[nr]->phase<3)) 
    { bat[nr]->set_tiefe(22.5+nr); 
      bat[nr]->p_sprite->setFrame(2);
      bat[nr]->vx=2*bat[nr]->vx;
      bat[nr]->phase=3;
    }
    else if ((bat[nr]->y>192) && (bat[nr]->phase<2)) 
    { bat[nr]->p_sprite->setZ(12.5+nr); 
      bat[nr]->p_sprite->setFrame(1); 
      bat[nr]->vx=2*bat[nr]->vx;
      bat[nr]->phase=2;
    }
    else if ((bat[nr]->y>96) && (bat[nr]->phase<1)) 
    { bat[nr]->set_tiefe(2.5+nr); 
      bat[nr]->vx=2*bat[nr]->vx;
      bat[nr]->vx=2*bat[nr]->vx;
      bat[nr]->phase=1;
    }
    if (bat[nr]->x>XMAX) if (bat[nr]->vx>0) { bat[nr]->phase=-1; bat[nr]->p_sprite->hide(); }
    if (bat[nr]->x<-OFX) if (bat[nr]->vx<0) { bat[nr]->phase=-1; bat[nr]->p_sprite->hide(); }
    if (bat[nr]->y>YMAX) { bat[nr]->phase=-1; bat[nr]->p_sprite->hide(); }
  }  
}

void wiese::bat_getroffen(int nr)
{ int gain=100;
  punkte=punkte+gain;
  emit punkte_changed(punkte);
  bat[nr]->phase=-1; 
  bat[nr]->p_sprite->hide(); 
  emit play_lala(sound_on,10);
  zeige_punkte(bat[nr],gain);
}

// ----------------------------------------------------------------------------------------------------------------

// rechte Seite:

void wiese::starte_pilz()
{ pilz->p_sprite->show();
  pilz->phase=0;
}

void wiese::pilz_getroffen()
{ int gain=5;
  punkte=punkte+gain;
  emit punkte_changed(punkte);
  pilz->phase=-1; 
  pilz->p_sprite->hide(); 
  zeige_punkte(pilz,gain);
  starte_floh();
}

// ---

void wiese::starte_floh()
{ floh->phase=0;
  floh->count=0;
  floh->vx=-2;
  floh->set_tiefe(29.8);
  floh->p_sprite->setFrame(1);
  floh->set_koords(1280,(YMAX-64));
  floh->p_sprite->show();
}

void wiese::move_floh()
{ if (floh->phase>=0)
  { double yadd;
    floh->count++;
    yadd=16*cos(double(floh->count)/24);
    yadd=yadd*yadd+96;
    if (floh->x>1280)   if (floh->vx>0) { floh->vx=-2; floh->p_sprite->setFrame(1); }
    if (floh->x<680) if (floh->vx<0) { floh->vx=2; floh->p_sprite->setFrame(0); }
    floh->x=floh->x+floh->vx;
    floh->set_koords(floh->x,yadd);
  }  
}

void wiese::floh_getroffen()
{ int gain=200;
  punkte=punkte+gain;
  emit punkte_changed(punkte);
  floh->phase=-1; 
  floh->p_sprite->hide();
  emit play_lala(sound_on,14);
  zeige_punkte(floh,gain);
  starte_pilz();
}


// ---

void wiese::starte_skorpion()
{ 
  skorpion[0]->set_koords(1200,skorpion[0]->y); skorpion[0]->vx=-2;
  skorpion[1]->set_koords(640,skorpion[1]->y); skorpion[1]->vx=2;
  skorpion[2]->set_koords(960,skorpion[2]->y); if (rand()%2<1) skorpion[2]->vx=-2; else skorpion[2]->vx=-2;
  
  for (int i=0; i<3; i++)
  { skorpion[i]->phase=0;
    skorpion[i]->count=0;
    skorpion[i]->vx=-2;
    if (skorpion[i]->vx<0) skorpion[i]->bildbase=2; else skorpion[i]->bildbase=0;
    if (i==0) skorpion[i]->bildbase=skorpion[i]->bildbase+4;
    skorpion[i]->p_sprite->setFrame(skorpion[i]->bildbase);
    skorpion[i]->p_sprite->setZ(skorpion[i]->z);
    skorpion[i]->p_sprite->show();
  }
}

void wiese::move_skorpion(int nr)
{   skorpion[nr]->phase=(skorpion[nr]->phase++)%2;
    skorpion[nr]->p_sprite->setFrame(skorpion[nr]->bildbase+skorpion[nr]->phase);
    if (skorpion[nr]->x>1200) if (skorpion[nr]->vx>0) 
    { skorpion[nr]->vx=-2; skorpion[nr]->bildbase=2;
      if (nr==0) { skorpion[nr]->bildbase=skorpion[nr]->bildbase+4; skorpion[nr]->vx=-4; }
      skorpion[nr]->p_sprite->setFrame(skorpion[nr]->bildbase); 
      skorpion[nr]->phase=0;
      skorpion[nr]->p_sprite->show();
    }
    double wende=640;
    if (nr>0) wende=wende+144;
    if (skorpion[nr]->x<wende) if (skorpion[nr]->vx<0) 
    { skorpion[nr]->bildbase=0;
      if (nr==0) { skorpion[nr]->bildbase=skorpion[nr]->bildbase+4; skorpion[nr]->vx=4; }
      skorpion[nr]->vx=2; 
      skorpion[nr]->p_sprite->setFrame(skorpion[nr]->bildbase); 
      skorpion[nr]->phase=0;
      skorpion[nr]->p_sprite->show();
    }
    skorpion[nr]->move_it();
}

void wiese::skorpion_getroffen(int nr)
{ int gain=20;
  punkte=punkte+gain;
  emit punkte_changed(punkte);
  skorpion[nr]->phase=-1; 
  skorpion[nr]->p_sprite->hide();
  emit play_lala(sound_on,15);
  zeige_punkte(skorpion[nr],gain);
}


// Level 3:
// ----------------------------------------
void wiese::starte_monster() // durch nachladen
{ int iw;
  // tuer->setFrame(1);
//  emit play_lala(sound_on,5);
  monster->phase=32;
  if (rand()%3==0)iw=116+rand()%24; else iw=400+rand()%112;
  monster->set_koords(scheune[0]->x()+iw,320);
  monster->p_sprite->setFrame(7); 
  monster->p_sprite->show(); 
}

void wiese:: move_monster()
{ if (monster->phase>=0) monster->phase--;
  if (monster->phase>24) { monster->y=monster->y-8;; monster->p_sprite->move(monster->x,monster->y); }
  else if (monster->phase>8) { int iw=7-(monster->phase/2)%8; monster->p_sprite->setFrame(iw); }
  else if (monster->phase>=0) { monster->y=monster->y+8; monster->p_sprite->move(monster->x,monster->y); }
  else monster->p_sprite->hide();
}

void wiese::monster_getroffen()
{ int gain=100;
  punkte=punkte+gain;
  emit punkte_changed(punkte);
  monster->phase=-1; 
  monster->p_sprite->hide();
//  tuer->setFrame(0); 
  emit play_lala(sound_on,17);
  zeige_punkte(monster,gain);
}





// === User interface: ============================================================================================
// ----------------------------------------------------------------------------------------------------------------
void wiese::pause()       { aktiv=false; pausiert=true;  p_pause->show(); }
void wiese::weiter()      { aktiv=true;  pausiert=false; p_pause->hide(); emit focus2playfield();  }
void wiese::game_over()   { weiter(); aktiv=false; p_gover->show(); emit gameover(punkte); }


void wiese::neues_spiel()
{ int i;
  punkte=0; 
  vorrat=0;
  aktiv=false;
  emit punkte_changed(punkte);
  emit bullets_changed(vorrat);
  p_gover->hide();
  shooter->show();
  mond->phase=0;
  mond->p_sprite->setFrame(0);
  mond->vx=0.5;
  mond->set_koords(-OFX,8);
  katze->phase=-1;  katze->p_sprite->hide();
  spinne->phase=-1; spinne->p_sprite->hide();
  waber->phase=-1;  waber->count=0; waber->p_sprite->hide();
  clown->phase=-1;  clown->p_sprite->hide();
  floh->phase=-1;   floh->p_sprite->hide();
  starte_pilz();
  starte_skorpion();
  
  tuer->setFrame(0);
  for (i=0; i<4; i++)
  { hexe[i]->phase=-1; hexe[i]->p_sprite->hide();
    kuerbis[i]->phase=-1; kuerbis[i]->p_sprite->hide();
  }
  for (i=0; i<8; i++)
  { bat[i]->phase=-1; bat[i]->p_sprite->hide();
    geist[i]->phase=-1; geist[i]->p_sprite->hide();
  }
  for (i=0; i<40; i++) psprite[i]->p_sprite->hide(); 
  emit play_lala(sound_on,16);
  weiter();
}

void wiese::nachladen() 
{ if (aktiv) { vorrat=NUMBULLETS; emit bullets_changed(vorrat); emit play_lala(sound_on,11); starte_monster(); } }

void wiese::hover(int ix,int iy) { shooter->move(ix-24,iy-24); hotspot->move(ix,iy); emit zeige_view(ix,iy); }

// ---------------------------------------------------------------------------------------------
void wiese::keyhandle(int cc)
{ int delta=16;
  if (cc==Qt::Key_P) { if (pausiert) weiter(); else pause(); }
  else if (cc==Qt::Key_N) neues_spiel();
  else if (cc==Qt::Key_Q) oberer->close();
  else if (cc==Qt::Key_T) { pause(); oberer->setWindowState(Qt::WindowMinimized); } /// TBIC-Taste
  else if (aktiv)
  { if ((cc==Qt::Key_Right) || (cc==Qt::Key_S)) emit scrollit(delta);
  else if ((cc==Qt::Key_Left) || (cc==Qt::Key_A)) emit scrollit(-delta);
  else if (cc==Qt::Key_Space) nachladen();  
  }   
}
// ------------------------------------------------------------------------------------------------
void wiese::shooterupdate(int dd) 
{ if (dd!=0) { shooter->move(shooter->x()+dd,shooter->y()); hotspot->move(hotspot->x()+dd,hotspot->y()); } }

void wiese::klick(int ix,int iy)
{ int i=ix+iy; // Compiler das Maul stopfen, dabei an klick-Routine anpassen
  double max=-999; 
  bool gefunden=false;
  emit focus2playfield();
  if (pausiert) weiter();
  else if (aktiv)
  { // weiter(); // keyboard-Patch
    if (vorrat>0)
    { emit play_lala(sound_on,12);      
      // neue Routine:
      QCanvasItemList treffer=hotspot->collisions(true);
      QCanvasItem* gotcha=0;
      QCanvasItemList::Iterator iter;
      for (iter= treffer.begin(); iter != treffer.end(); iter++) 
      { if ( (*iter)->z()>max) if ( (*iter)->z()<33) // 33=Fadenkreuz 
        { gotcha=*iter;
	  max=gotcha->z();
        }
      } 
      for (i=0; i<4; i++) if (!gefunden)
      { if (gotcha==kuerbis[i]->p_sprite) { gefunden=true; kuerbis_getroffen(i); }
         else if (gotcha==hexe[i]->p_sprite) { gefunden=true; hexe_getroffen(i); }
      }
      for (i=0; i<8; i++) if (!gefunden)
      { if (gotcha==bat[i]->p_sprite) { gefunden=true; bat_getroffen(i); }
        else if (gotcha==geist[i]->p_sprite) { gefunden=true; geist_getroffen(i); } 
      }
      for (i=0; i<3; i++) if (!gefunden) if (gotcha==skorpion[i]->p_sprite) { gefunden=true; skorpion_getroffen(i); }
      if (!gefunden)
      { if (gotcha==mond->p_sprite) mond_getroffen();
        else if (gotcha==spinne->p_sprite) spinne_getroffen(); 
        else if (gotcha==katze->p_sprite)  katze_getroffen();
        else if (gotcha==topf) { if (waber->count<1)  starte_waber(); } // einmal pro Durchgang
        else if (gotcha==waber->p_sprite) { if (waber->count>2) ghoul_getroffen(); else pott_getroffen(); }
        else if (gotcha==tuer) { if (clown->phase<0) starte_clown(); }
        else if (gotcha==clown->p_sprite) clown_getroffen();
        else if (gotcha==floh->p_sprite) floh_getroffen();
        else if (gotcha==pilz->p_sprite) pilz_getroffen();
	else if (gotcha==monster->p_sprite) monster_getroffen();
      }
      vorrat--;
      emit bullets_changed(vorrat);
    } else emit play_lala(sound_on,13);
  }
}

void wiese::zeige_punkte(flieger* sprit,int pkt)
{ int k=-1;
  int id=sprit->id;
  switch (pkt)
  { case -100: k=0; break;
    case 5: k=1; break;
    case 10: k=2; break;     
    case 20: k=3; break;
    case 50: k=4; break;
    case 100: k=5; break;     
    case 200: k=6; break;
    case 500: k=7; break;
  }
  psprite[id]->phase=30;
  psprite[id]->p_sprite->setZ(sprit->p_sprite->z());
  psprite[id]->p_sprite->move(sprit->p_sprite->x(),sprit->p_sprite->y(),k);
  psprite[id]->p_sprite->show();
}

void wiese::set_sound(int nr) 
{ // sound_on=((QSound::isAvailable()) && (nr==0)); 
  sound_on=(nr==0); 
  if (sound_on) { emit sound_setting0(true); emit sound_setting1(false); }
  else  { emit sound_setting0(false); emit sound_setting1(true); }
}

// ==================================================================================================================
