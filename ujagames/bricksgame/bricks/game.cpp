// Bricks by uja - The game (c) uja 1993(TurboPascal), 2003(Javascript), 2005(C++/Qt3)
// -----------------------------------------------------------------------------------------------------------===---------------
// 25.02.05: Grafiken, Levels von ujas javascript-Spielen, weitere Grafiken, komplettes User-Interface
// 01.03.05: Weil eigene Sprites zu langsam reagierten, Versuch, mit Qt-Spriteroutinen auszukommen
// 05.03.05: Sound NAS
// 07.03.05: Weil Qt-Kollisionsroutinen ohne Collision-Interrupt/Slots arbeiten
//           und trotz Timersync so eine exakte Auswertung zum Scheitern verurteilt war, eigenen Sprites wieder verwendet
// 08.03.05: erste spielbare Version 0.1.0 mit normalem Paddel und Floaters
//           Goodies: +50, -50, Leim, kleiner Paddel, groﬂer Paddel, normaler Paddel, Freeze, Speed+, Speed-, Speed normal,
//           Ball weg, Ball hinzu, Levelup mit Exit
// 09.03.05: Version 0.2.0  Ballspeed erniedrigt, Laser, Tripleball, Leimpaddel/Tripelball abgestimmt, aRts-Sound, Pause/weiter
// 10.03.05: Version 0.2.1  Buttons fuer Neu,Pause/weiter,Tilt auf Oberflaeche, deutsche Installationsbatches
// 11.03.05: Version 0.3.0  eigene Kollisionsroutinen, da Qt::collidesWith zu grosszuegig mit Raendern umging und zu oft meldete
// 13.03.05: Version 0.5.0  Tastatursteuerung
// ------------------------------------------------------------------------------------------------------------------------------
// todo: Level-Editor und einige Userlevels, Wechsler-Goodie (1s)
// ==============================================================================================================================
#include <float.h>
#include <math.h>
#include "game.h"
#include "lala.h"

// eigene Collision detect: collides with scheint auch zu gehen, aber nach der schlechten Erfahrung bleibt das Ganze erstmal drin
// ------------------------------------------------------------------------------------------------------------------------
bool ball2ball(double x1,double y1,double breite1, double x2,double y2,double breite2)
{ double mx1=x1+breite1/2,my1=y1+breite1/2,mx2=x2+breite2/2,my2=y2+breite2/2;
  double dx=mx1-mx2,dy=my1-my2,dz=(breite1+breite2)/2;
  bool crash=(dx*dx+dy*dy<dz*dz);
  return crash;
}

bool brick2brick(double x1,double y1,double breite1,double hoehe1,double x2,double y2,double breite2,double hoehe2)
{ bool crash=((x1>(x2-breite1)) && (x1<(x2+breite2)) && (y1>(y2-hoehe1)) && (y1<(y2+hoehe2))); 
  return crash;
}

bool ball2brick(double x1,double y1,double breite1,double x2,double y2,double breite2,double hoehe2)
{ bool crash=brick2brick(x1,y1,breite1,breite1,x2,y2,breite2,hoehe2); 
  if (crash) // Auswertung:
  { double mx1=x1+breite1/2,my1=y1+breite1/2,dx,dy,dz=breite1*breite1/4;
    crash=((x1>(x2-breite1)) && (x1<(x2+breite2)) && (my1>y2) && (my1<(y2+hoehe2)));
    if (!crash) crash=((mx1>x2) && (mx1<(x2+breite2)) && (y1>(y2-breite1)) && (y1<(y2+hoehe2)));
    // 4 Ecken, bei Zeitproblemen weglassen:
    if (!crash) { dx=x1-x2; dx=dx*dx; dy=y1-y2; dy=dy*dy; crash=(dx+dy<dz); }
    if (!crash) { dx=x1-x2; dx=dx*dx; dy=y1-y2-hoehe2; dy=dy*dy; crash=(dx+dy<dz); }
    if (!crash) { dx=x1-x2-breite2; dx=dx*dx; dy=y1-y2; dy=dy*dy; crash=(dx+dy<dz); }
    if (!crash) { dx=x1-x2-breite2; dx=dx*dx; dy=y1-y2-hoehe2; dy=dy*dy; crash=(dx+dy<dz); }
  }
  return crash;
}

// neue Richtung bestimmen via Eintritt Punkt mit vx/vy bei Momentaufnahme px/py in Rechteck
void reflexion(double px,double py,double vx,double vy,double oben,double rechts,double unten,double links,double &vxneu,double &vyneu)
{ double radius=8.0;
  double eps=0.001; // bisschen groesser nehmen, kann Berechnung abkuerzen
  // Sonderfaelle: vx<eps oder vy<eps, wobei 2. Fall nicht vorkommen duerfte:
  if (fabs(vx)<eps)      { vyneu=-vy; vxneu=vx; }
  else if (fabs(vy)<eps) { vyneu=vy; vxneu=-vx; }
  else
  { 
    // Bestimmung der Richtungsgleichung: y=alpha*x+b
    int i,k1,k2;
    double x[4],y[4]; for (i=0; i<4; i++) { x[i]=-DBL_MAX; y[i]=-DBL_MAX; } // Koo oben unten rechts links
    double alpha1,alpha2,b1,b2,xx,yy,dx,dy;
    alpha1=vy/vx; alpha2=vx/vy; b1=py-alpha1*px; b2=px-alpha2*py;

    // Schnitte mit den Seiten, sortieren nach oben rechts unten links:
    x[0]=alpha2*oben+b2;   if ((x[0]>=(links-radius)) && (x[0]<=(rechts+radius))) y[0]=oben;
    y[1]=alpha1*rechts+b1; if ((y[1]>=(oben -radius)) && (y[1]<=(unten +radius))) x[1]=rechts;
    x[2]=alpha2*unten+b2;  if ((x[2]>=(links-radius)) && (x[2]<=(rechts+radius))) y[2]=unten;
    y[3]=alpha1*links+b1;  if ((y[3]>=(oben -radius)) && (y[3]<=(unten +radius))) x[3]=links;
    // Eintrittspunkt bestimmen: Punkt mit dem kleinsten Abstand zum Startpunkt, Sonderfall: Kante ergibt 2 Punkte 
    xx=px-vx; yy=py-vy; // Startpunkt vor Treffer: 
    alpha1=DBL_MAX; k1=-1; k2=-1; 
    for (i=0; i<4; i++) if ((x[i]>-DBL_MAX) && (y[i]>-DBL_MAX))
    { dx=x[i]-xx; dx=dx*dx;
      dy=y[i]-yy; dx=dx+dy*dy;
      if (dx<alpha1) { k1=i; k2=-1; alpha1=dx; }
      else if (dx==alpha1) { k2=i; alpha1=dx; }
    }
    
// neue Geschwindigkeit bestimmen: ist k1<0, so kam ein "falsches Signal" an (Qt::collidesWith()) und kann ignoriert werden
    if (k2<0) switch (k1) // dieser Teil trsegt die Hauptlast:
    { case 0: vxneu=vx;        vyneu=-fabs(vy); break;
      case 1: vxneu=fabs(vx);  vyneu=vy; break;
      case 2: vxneu=vx;        vyneu=fabs(vy); break;
      case 3: vxneu=-fabs(vx); vyneu=vy; break;
      default: vxneu=vx;       vyneu=vy;
    }
    else switch(k2) // dieser Teil wird eher selten durchlaufen, da Stossbereich (theoretisch) nur ein Punkt
    { case 0: switch (k1)
             { case 1: if (vx<0) if (vy>0) { vxneu=-vx; vyneu=-vy; } break;
               case 3: if (vx>0) if (vy>0) { vxneu=-vx; vyneu=-vy; } break;
             }
             break;
      case 1: switch (k1)
              { case 0: if (vx<0) if (vy>0) { vxneu=-vx; vyneu=-vy; } break;
                case 2: if (vx<0) if (vy<0) { vxneu=-vx; vyneu=-vy; } break;
              }
              break;
      case 2: switch (k1)
              { case 1: if (vx<0) if (vy<0) { vxneu=-vx; vyneu=-vy; } break;
                case 3: if (vx>0) if (vy<0) { vxneu=-vx; vyneu=-vy; } break;
              }
              break;
      case 3: switch (k1)
              { case 0: if (vx>0) if (vy>0) { vxneu=-vx; vyneu=-vy; } break;
                case 2: if (vx>0) if (vy<0) { vxneu=-vx; vyneu=-vy; } break;
              }
              break;
    }
  }
}

// ====================================================================================================================
// bewaehrte eigene sprites:
// back to the roots: -------------------------------------------------------------------------------------------------
flieger::flieger(int nr)                 { id=nr; x=0; y=0; z=nr; vx=0; vy=0; phase=-1; }
void flieger::set_koords(double xx,double yy) { x=xx; y=yy; p_sprite->move(x,y); } 
void flieger::set_tiefe(double zz)            { z=zz; p_sprite->setZ(z); }
void flieger::set_koords(int xx,int yy)       { x=xx; y=yy; p_sprite->move(x,y); } 
void flieger::set_tiefe(int zz)               { z=zz; p_sprite->setZ(z); }

void flieger::set_phase(int nr) 
{ phase=nr; 
  if (nr<0) p_sprite->hide(); else { p_sprite->setFrame(nr); breite=p_sprite->width(); hoehe=p_sprite->height(); } 
}

void flieger::move_it()                       { x=x+vx; y=y+vy; p_sprite->move(x,y); }
void flieger::set_vx(double xx)               { vx=xx; }
void flieger::set_vy(double yy)               { vy=yy; }
void flieger::set_speed(double xx,double yy)  { vx=xx; vy=yy; }
void flieger::sprite_off()                    { phase=-1; p_sprite->hide(); }
void flieger::sprite_on()                     { set_phase(0); set_tiefe(id); p_sprite->show(); }
// ==============================================================================================================================

game::game(window* parent,char* name) // braucht die Subclass wegen Keyboard-Polling
{ int i,k=0;
  oberer=parent;
  my_levelliste=new level(QString(LEVELFILE));
  paddel=new flieger(k); k++;
  for (i=0; i<NUM_BALLS; i++)    { ball[i]   =new flieger(k); k++; }
  for (i=0; i<NUM_FLOATERS; i++) { floater[i]=new flieger(k); k++; }
  for (i=0; i<NUM_GOODIES; i++)  { goodie[i] =new flieger(k); k++; }
  for (i=0; i<NUM_LASERS; i++)   { laser[i]  =new flieger(k); k++; }
  level_id=0;
  tileset=0;
  aktiv=false;
  timrec1=new QTimer(); connect(timrec1,SIGNAL(timeout()),this,SLOT(move_all())); timrec1->start(tic_no);
  timrec2=new QTimer(); connect(timrec2,SIGNAL(timeout()),this,SLOT(poll_it()));  timrec2->start(10);
}

// key polling an oberer, das einiz wahre Spielgefuehl:
void game::poll_it() { if (oberer->keydn>0) keyhandle(oberer->keydn); }
void game::grab_focus() { oberer->setFocus(); }

void game::keyhandle(int cc) 
{ if (cc==Qt::Key_P) { if (pausiert) weiter(); else pause(); oberer->keydn=-1; }
  else if (cc==Qt::Key_N) { neues_spiel();     oberer->keydn=-1; }
  else if (cc==Qt::Key_Q) { oberer->close();   oberer->keydn=-1; }
  else if (cc==Qt::Key_T) { tilten();          oberer->keydn=-1; }
  else if (aktiv) 
  { int pspeed=int(0.5*ball[0]->breite),pmin=0,pmax=XMAX*DX,pakt=int(paddel->x+paddel->breite/2); // sync auf Paddelmitte
    if ((cc==Qt::Key_Space) || (cc==Qt::Key_Up)) { geklickt(0,0); oberer->keydn=-1; }
    else if ((cc==Qt::Key_Left)  || (cc==Qt::Key_J)) { pakt=pakt-pspeed; if (pakt<pmin) pakt=pmin; hover(pakt,0); }
    else if ((cc==Qt::Key_Right) || (cc==Qt::Key_K)) { pakt=pakt+pspeed; if (pakt>pmax) pakt=pmax; hover(pakt,0); }
  }
}

// -----------------------------------------------------------------------------------------------------------------------
// interner slot: game main loop
// ----------------------------------------------------------------------------------------------------------------------- 
void game::move_all()
{ bool level_feddich=false;
  int i,j,j1,k;
  int patt[4]; 

  if (aktiv) 
  { for (i=0; i<NUM_GOODIES; i++) if (goodie[i]->phase>=0) // Goodie fangen
    { goodie[i]->move_it(); // nur nach unten
      if (goodie[i]->y>(paddel->y+paddel->hoehe)) goodie[i]->sprite_off();
      else if (goodie[i]->y>(paddel->y-goodie[i]->hoehe))
           if (goodie[i]->p_sprite->collidesWith(paddel->p_sprite)) goodie_auf_paddel(i);
    }
    for (i=0; i<NUM_FLOATERS; i++)
    {  if (floater[i]->phase>=0) 
       { floater[i]->move_it(); 
         floater[i]->set_phase((floater[i]->phase+1)%24);
         if (floater[i]->x<0) floater[i]->vx=-floater[i]->vx; 
	 else if (floater[i]->x>XMAX*DX) floater[i]->vx=-floater[i]->vx;
         if (floater[i]->y<0) floater[i]->y=fabs(floater[i]->vy); 
	 else if (floater[i]->y>(YMAX+5)*DY) floater[i]->sprite_off(); 
         if (floater[i]->p_sprite->collidesWith(paddel->p_sprite)) 
         { punkte=punkte+50; emit punkte_changed(punkte); emit play_lala(sound_on,3);
           floater[i]->sprite_off();
         }
       }
       else if (rand()%10000<2) 
       { floater[i]->set_koords(rand()%(XMAX*DX),0);
         floater[i]->set_speed(((rand()%4)-2),2.0);
         floater[i]->sprite_on();
       }
    }

    // Laserschuesse:
    int ix,iy;
    for (i=0; i<NUM_LASERS; i++) if (laser[i]->phase>=0)
    { laser[i]->move_it();
      if ((laser[i]->y<0) || (laser[i]->y>paddel->y)) laser[i]->sprite_off(); 
      if (laser[i]->phase>=0) for (j=0; j<NUM_FLOATERS; j++) if (laser[i]->p_sprite->collidesWith(floater[j]->p_sprite))
      { punkte=punkte+50; emit punkte_changed(punkte);
        floater[j]->sprite_off();
        laser[i]->sprite_off();
      }
      if (laser[i]->phase>=0)
      { ix=int((laser[i]->x+1)/DX); iy=int(laser[i]->y/DY); ix=ix+XMAX*iy;
        if (ix>=0) if (ix<SMAX) if (block[ix]->isVisible())
        { laser[i]->sprite_off();
          if (f[ix]>0) // 0;  feste Bloecke 
          { if (block[ix]->frame()==11)   // 11: poly-Block erledigt
            { punkte=punkte+10*(12-f[ix]); 
              emit punkte_changed(punkte); 
              block[ix]->hide();
            }
            else if (block[ix]->frame()>7) block[ix]->setFrame(block[ix]->frame()+1); // poly-Block 8-10
            else { block[ix]->hide(); punkte=punkte+5; emit punkte_changed(punkte); } // normale Bloecke
            level_feddich=check_levelende(); 
          }
        } 
      }
    }

    // Balltreffer: Wand, Brick oder Floater
    for (i=0; i<NUM_BALLS; i++) if (ball[i]->phase>=0) 
    { ball[i]->move_it();
      // zu kleine Steigung korrigieren
      if (fabs(ball[i]->vy)<2.5) if (ball[i]->vx!=0)
      { if (ball[i]->vy<0) ball[i]->vy=-2.5; else ball[i]->vy=2.5;
        ball[i]->vx=sqrt(64.0-6.25);
      }
      // Crash mit Floater:
      for (j=0; j<NUM_FLOATERS; j++) if (floater[j]->phase>=0) if (ball[i]->p_sprite->collidesWith(floater[j]->p_sprite))
      { punkte=punkte+100;
        emit punkte_changed(punkte);
        emit play_lala(sound_on,3);
        floater[j]->sprite_off();
        ball[i]->set_vx(-ball[i]->vx);
      }
      // Ball - Raender::
      if (ball[i]->x<0) ball[i]->set_vx(fabs(ball[i]->vx)); 
      else if (ball[i]->x>(xmax-ball[i]->breite)) ball[i]->set_vx(-fabs(ball[i]->vx)); 
      if (ball[i]->y<0) ball[i]->set_vy(fabs(ball[i]->vy));
      else if (ball[i]->y>(YMAX+5)*DY) ball_weg(i);

      // Ball trifft Paddel:
      else if (ball[i]->y>(paddel->y-ball[i]->hoehe)) 
      { // if (ball2brick(ball[i]->x,ball[i]->y,ball[i]->breite,paddel->x,paddel->y,paddel->breite,paddel->hoehe))
        if (ball[i]->p_sprite->collidesWith(paddel->p_sprite))
        { ball[i]->set_koords(ball[i]->x,paddel->y-ball[i]->hoehe); 
          if (paddel->phase==3) // Leim
          {  emit play_lala(sound_on,3);
             if (paddelball>=0) starte_ball_von_paddel(paddelball);
             paddelball=i; ball[i]->set_speed(0,0); paddeldelta=ball[i]->x-paddel->x;
          }
          else { emit play_lala(sound_on,1); starte_ball_von_paddel(i); }
        }
      }

      // Ball - Brick -----------------------------------------------------------------------------------------------
      else if (ball[i]->y<(YMAX*DY))
      { bool rrand=false;
        // 1. Blocksprites eingrenzen auf ix,ix+1,iy,iy+1 mit Abfrage auf rechten Rand und unteren Rand zum weiteren Eingrenzen:
        for (j=0; j<4; j++) patt[j]=-1; // Crash pattern, max. 4 Bricks
        int ix=int(ball[i]->x/DX); int iy=int(ball[i]->y/DY);
        k=0; j1=ix+XMAX*iy;
        if (block[j1]->isVisible()) 
        if (ball2brick(ball[i]->x,ball[i]->y,ball[i]->breite,block[j1]->x(),block[j1]->y(),(DX),(DY)))  { patt[k]=j1; k++; }
        if (ix<(XMAX-1))
        { if (block[j1+1]->isVisible())
          if (ball2brick(ball[i]->x,ball[i]->y,ball[i]->breite,block[j1+1]->x(),block[j1+1]->y(),(DX),(DY)))  { patt[k]=j1+1; k++; }
        }
        else if (ball[i]->x>xmax-ball[i]->breite) rrand=true; 
        if (iy<(YMAX-1)) 
        { j1=j1+XMAX;
          if (block[j1]->isVisible()) 
          if (ball2brick(ball[i]->x,ball[i]->y,ball[i]->breite,block[j1]->x(),block[j1]->y(),(DX),(DY)))  { patt[k]=j1; k++; }
          if (ix<(XMAX-1)) if (block[j1+1]->isVisible())
          if (ball2brick(ball[i]->x,ball[i]->y,ball[i]->breite,block[j1+1]->x(),block[j1+1]->y(),(DX),(DY)))  { patt[k]=j1+1; k++; }
        }
        // 2. Trefferpattern (max.4)  auswerten, neue Richtung bestimmen:
        if (patt[0]>=0)
        { int ip=patt[0];
          double mx,my;
          double vxalt=ball[i]->vx,vyalt=ball[i]->vy;
          mx=ball[i]->x+ball[i]->breite/2; 
          my=ball[i]->y+ball[i]->hoehe/2;
          emit play_lala(sound_on,2);
          if ((patt[1]<0) && (!rrand)) // ein einzelner Stein getroffen
          { double vxneu,vyneu; 
            reflexion(mx,my,ball[i]->vx,ball[i]->vy,block[ip]->topEdge(),
                      block[ip]->rightEdge(),block[ip]->bottomEdge(),block[ip]->leftEdge(),vxneu,vyneu);
            ball[i]->vx=vxneu;
            ball[i]->vy=vyneu;
          }
          else if ((patt[2]<0) && (!rrand)) // 2 Steine getroffen
          { int dd=patt[0]-patt[1];
            if (fabs(dd)==1) ball[i]->set_vy(-vyalt); // nebeneinander
            else if (fabs(dd)==XMAX) ball[i]->set_vx(-vxalt); // uebereinander
            else { ball[i]->set_speed(-vxalt,-vyalt); ball[i]->move_it(); }// Bewegung neue Richtung, um Blockaden zu verhindern
          }
          else { ball[i]->set_speed(-vxalt,-vyalt); ball[i]->move_it(); }
          // 3. Punkte kassieren, getroffene Bloecke entfernen, Goodies starten, Endcheck: 
          for (j=0; j<4; j++) if (patt[j]>=0) 
          { if (f[patt[j]]>0) // 0;  feste Bloecke 
            { if (block[patt[j]]->frame()==11)                      // 11: poly-Block erledigt
              { punkte=punkte+10*(12-f[patt[j]]); 
                emit punkte_changed(punkte); 
                block[patt[j]]->hide();
              }
              else if (block[patt[j]]->frame()>7) block[patt[j]]->setFrame(block[patt[j]]->frame()+1); // poly-Block 8-10
              else                              // 1-7:  normale Bloecke
              { block[patt[j]]->hide(); punkte=punkte+5; emit punkte_changed(punkte);
                if (rand()%100<15) starte_goodie(block[patt[0]]->x()+8,block[patt[0]]->y());
              }
              level_feddich=check_levelende(); 
            }
          }
        }
      }
    }
  }
  if (level_feddich) level_up();
}

bool game::check_levelende() 
{ int i,k=0; 
  for (i=0; i<SMAX; i++) if (f[i]>0) if (block[i]->isVisible()) k++;  
  emit rest_changed(k);
  reststeine=k;
  return (k==0);
}

void game::level_up()
{ aktlevel++;
  punkte=punkte+100*aktlevel+5*reststeine;
  vorrat0=vorrat; punkte0=punkte; 
  if (aktlevel>=my_levelliste->anz_levels) { punkte=punkte+punkte; emit messi(_KEINELEVELS); game_over(); }
  else starte_level(aktlevel);
}

void game::starte_level(int nr)
{ int i;
  aktiv=false;
  aktlevel=nr;
  vorrat0=vorrat;
  punkte0=punkte;
  p_gover->hide();
  tuer->hide();
  reststeine=0;
  QString code=".0123456789abcdef";  
  QString my_code=my_levelliste->my_level[aktlevel]->levelcode;
  for (i=0; i<SMAX; i++) 
  { f[i]=code.find(my_code.at(i))-1; 
    if (f[i]<0) block[i]->hide(); else { block[i]->setFrame(f[i]); block[i]->show(); }
    if (f[i]>0) reststeine++;
  }
  for (i=0; i<NUM_BALLS; i++)    ball[i]->sprite_off();
  for (i=0; i<NUM_GOODIES; i++)  goodie[i]->sprite_off();
  for (i=0; i<NUM_FLOATERS; i++) floater[i]->sprite_off();
  for (i=0; i<NUM_LASERS; i++)   laser[i]->sprite_off();

  paddel->set_koords(288,((YMAX+4)*DY)); 
  paddel->sprite_on(); 
  init_ball();
  emit rest_changed(reststeine);
  emit punkte_changed(punkte);
  emit levelname_changed(my_levelliste->my_level[aktlevel]->titel);
  emit play_lala(sound_on,0);
  weiter();
}

// ---------- Ballbewegungen --------------------------------------
// ----------------------------------------------------------------
void game::init_ball()
{ timrec1->changeInterval(15);
  ball[0]->set_speed(0,0);
  ball[0]->sprite_on();
  paddel->sprite_on();
  double x=paddel->x+paddel->breite/2-ball[0]->hoehe/2;
  double y=paddel->y-ball[0]->hoehe;
  ball[0]->set_koords(x,y);
  follow_paddel=true;
  paddelball=0;
  paddeldelta=ball[0]->x-paddel->x;
  vorrat--;
  emit vorrat_changed(vorrat);
}

void game::ball_weg(int nr)
{ ball[nr]->sprite_off();
  int i,k=0; for (i=0; i<NUM_BALLS; i++) if (ball[i]->phase>=0) k++;
  if (k==0) { if (vorrat<1) game_over(); else init_ball(); }
}

void game::starte_ball_von_paddel(int nr)
{ double excentr,dx,dy;
  if ((ball[nr]->vy==0) && (ball[nr]->vx==0)) ball[nr]->vy=4; // Start und Leimtopf
  excentr=ball[nr]->x-paddel->x+0.5*(ball[nr]->breite-paddel->breite);
  excentr=5*excentr/paddel->breite; // -2.5 bis 2.5
  dx=excentr+ball[nr]->vx; 
  if (dx>7.5) dx=7.5; else if (dx<-7.5) dx=-7.5;
  dy=-sqrt(64.0-dx*dx); // reflexion: sicherstellen, dass aufwaerts
  ball[nr]->set_speed(dx,dy);
  paddelball=-1;
}

// Goodies:
// ----------------------------------------------------------------
void game::starte_goodie(double xx,double yy)
{ int i,k;
  // freien goodie finden:
  k=-1; for (i=0; i<NUM_GOODIES; i++) if (goodie[i]->phase<0) k=i;
  if (k>=0)
  { goodie[k]->set_phase(rand()%15);
    goodie[k]->set_koords(xx,yy);
    goodie[k]->p_sprite->show();
    goodie[k]->set_speed(0,0.1*(20+k));
  }
}

void game::goodie_auf_paddel(int nr)
{ int i,k;
  punkte=punkte+10;
  emit punkte_changed(punkte);
  emit play_lala(sound_on,4);
  if (goodie[nr]->phase==15) goodie[nr]->set_phase(rand()%15);
  switch (goodie[nr]->phase)
  { case 0: punkte=punkte+50; emit punkte_changed(punkte); break;
    case 1: punkte=punkte-50; emit punkte_changed(punkte); break;
    case 2: vorrat++; if (vorrat>7) vorrat=7; emit vorrat_changed(vorrat); break;
    case 3: vorrat--; if (vorrat<0) vorrat=0; emit vorrat_changed(vorrat); break;
    case 4: paddel->set_phase(1); follow_paddel=false; if (paddelball>=0) { starte_ball_von_paddel(paddelball); paddelball=-1; } break;
    case 5: paddel->set_phase(2); follow_paddel=false; if (paddelball>=0) { starte_ball_von_paddel(paddelball); paddelball=-1; } break;
    case 6: paddel->set_phase(0); follow_paddel=false; if (paddelball>=0) { starte_ball_von_paddel(paddelball); paddelball=-1; } break;
    case 7: paddel->set_phase(3); follow_paddel=true; break;
    case 8: paddel->set_phase(4); follow_paddel=false; if (paddelball>=0) { starte_ball_von_paddel(paddelball); paddelball=-1; } break;
    case 9: paddel->set_phase(5); QTimer::singleShot(1000,this,SLOT(unfreeze_paddel())); break;
    case 10: tuer->show(); break;
    case 11: timrec1->changeInterval(tic_hi); QTimer::singleShot(12000,this,SLOT(normal_speed())); break;
    case 12: timrec1->changeInterval(tic_lo); QTimer::singleShot(12000,this,SLOT(normal_speed())); break;
    case 13: timrec1->changeInterval(tic_no); break;
    case 14: k=-1;
             if (follow_paddel) if (paddelball>=0) { starte_ball_von_paddel(paddelball); paddelball=-1; }
             k=0;
             for (i=0; i<NUM_BALLS; i++) if (ball[i]->phase<0)
             { ball[i]->set_koords(ball[k]->x,ball[k]->y);
               if (k==0) { ball[i]->set_speed(2.5,3.2); k++; } else ball[i]->set_speed(-3.2,2.5); 
               ball[i]->sprite_on();
             }
             break;
  }
  goodie[nr]->sprite_off();
}

// ==============================================================================================================================
// slots:
// ------------------------------------------------------------------------------------------------------------------------------
void game::game_over()   { aktiv=false; p_gover->show(); emit punkte_changed(punkte); emit gameover(punkte); } 
void game::pause()       { if (aktiv) { aktiv=false; pausiert=true;  p_pause->show(); emit messi(_PAUSED); } }
void game::weiter()      { aktiv=true; pausiert=false; p_pause->hide(); grab_focus(); }
void game::restart()     { vorrat=vorrat0; punkte=punkte0; starte_level(aktlevel); }
void game::neues_spiel() { punkte=0; vorrat=startbaelle; emit messi(""); starte_level(0); } 
void game::unfreeze_paddel() { paddel->set_phase(0); if (paddelball>=0) starte_ball_von_paddel(paddelball); }
void game::normal_speed()    { timrec1->changeInterval(tic_no); }
void game::toggle_pause()    { if (pausiert) weiter(); else pause(); }


void game::tilten()
{ double xx,yy,dd;
  for (int i=0; i<NUM_BALLS; i++) if (ball[i]->phase>=0) 
  { xx=(rand()%40)/10; yy=ball[i]->vy; dd=xx*xx+yy*yy; dd=sqrt(64/dd); 
    xx=xx*dd; yy=yy*dd;
    ball[i]->set_speed(xx,yy);
  }
}

void game::hover(int ix,int iy) 
{ if (aktiv) if (paddel->phase!=5)
  { bool dummy; dummy=((ix) && (iy)); // warnung abfangen
    double x=double(ix)-0.5*paddel->breite; 
    paddel->set_koords(x,double((YMAX+4)*DY)); 
    if (follow_paddel) if (paddelball>=0) 
    { ball[paddelball]->set_speed(0,0);
       ball[paddelball]->set_koords(paddel->x+paddeldelta,paddel->y-ball[paddelball]->hoehe);
    }
    if (tuer->isVisible()) if (paddel->p_sprite->collidesWith(tuer)) level_up();
  }
}

void game::geklickt(int ix,int iy)
{ bool dummy; dummy=((ix) && (iy)); // warnung abfangen
  int k=0;
  if (aktiv) 
  { if (paddel->phase!=5)
    { if (paddel->phase==4) for (int i=0; i<NUM_LASERS; i++) if (k<2) if (laser[i]->phase<0)
      { k++;
        laser[i]->set_speed(0,-DY);
	laser[i]->set_koords(paddel->x+k*32-16,paddel->y);
        laser[i]->sprite_on();
        emit play_lala(sound_on,5);
      }
      if (paddelball>=0) starte_ball_von_paddel(paddelball);
    }
  }
  else if (pausiert) weiter();
}

// Parameter settings
// ------------------

void game::set_sound(int nr) 
{ sound_on=((QSound::isAvailable()) && (nr==0)); 
  if (sound_on) { emit sound_setting0(true); emit sound_setting1(false); }
  else  { emit sound_setting0(false); emit sound_setting1(true); }
  grab_focus();
}

void game::set_background(int nr)
{ QString ima;
  switch (nr)
  { case 0: ima=BACKIMA0; break;     // leer
    case 1: ima=BACKIMA1; break;     // Original BG 
    case 2: ima=BACKIMA2; break; 
    case 3: ima=BACKIMA3; break; 
  }
  QCanvas* aktcanvas=p_pause->canvas();
  aktcanvas->setBackgroundPixmap(QPixmap(ima));
  bgbild=nr;
  grab_focus();
}

void game::set_tileset(int nr)
{ switch (nr)
  { case 0: blockarray=blockarray0; break;
    case 1: blockarray=blockarray1; break;
    case 2: blockarray=blockarray2; break;
  }
  for (int i=0; i<SMAX; i++) block[i]->setSequence(blockarray);
  tileset=nr;
  grab_focus();
}

void game::save_settings()
{ QString zeile,feld1,feld2;;
  QFile fh("gameparm.ini"); 
  if (sound_on) zeile="0;"; else zeile="1;";
  zeile=zeile+feld1.setNum(tileset)+';'+feld2.setNum(bgbild)+"\n";
  if (fh.open(IO_WriteOnly)) { fh.writeBlock(zeile,zeile.length()); fh.close(); }
  emit grab_focus();
}

// Spielspeicher
// --------------

void game::lade_spielstand(int nr)      
{  int bufsize=254,i,k=nr+1;
   aktiv=false;
   // speicher.ini komplett einlesen:
   QString buff,buff1,bname[MAXSPEICHER];
   QFile fh("./speicher.ini");
   if (fh.open(IO_ReadOnly))
   { fh.readLine(buff,bufsize);
     for (i=0; i<MAXSPEICHER; i++) { bname[i]=buff.section(';',i,i); }
     for (i=1; i<=k; i++) { fh.readLine(buff,bufsize); }
     buff1=buff.section(';',0,0); level_id=buff1.toInt();
     buff1=buff.section(';',1,1); aktlevel=buff1.toInt();  
     buff1=buff.section(';',2,2); punkte0=buff1.toInt();
     buff1=buff.section(';',3,3); vorrat0=buff1.toInt();
     fh.close();
   }
   // Levels einlesen zugehoerigem Levelfile:
   switch (level_id)
   { case 0: my_levelliste=new level(QString(LEVELFILE)); 
             emit levelfile0_changed(true);
	     levelfile1_changed(false);
             break; 
     case 1: my_levelliste=new level(QString(ULEVELFILE)); 
             emit levelfile0_changed(false);
	     levelfile1_changed(true);
             break;
   }
   
   if (aktlevel>=my_levelliste->anz_levels) aktlevel=my_levelliste->anz_levels-1;
   if (aktlevel<0) // falls  userlevel.csv geplaettet wurde
   { aktlevel=0; my_levelliste=new level(QString(LEVELFILE)); punkte0=0; 
     emit levelfile0_changed(true); levelfile1_changed(false);
   }
   punkte=punkte0; 
   vorrat=vorrat0; 
   QString raus;
   raus=_GAMELOADED+bname[nr];
   emit messi(raus);
   starte_level(aktlevel);
}  

void game::speichere_spielstand(int nr) 
{ int bufsize=254,i,j=aktlevel,k=nr+1,nrecs=MAXSPEICHER;
  if (j<0) j=0;
  QString buff[nrecs+1];
  QString bu0="",bu1="",bu2="";
  QString bname[MAXSPEICHER];
  QFile fh("./speicher.ini"); 
  if (fh.open(IO_ReadOnly)) 
  { fh.readLine(buff[0],bufsize);
    for (i=0; i<MAXSPEICHER; i++) { bname[i]=buff[0].section(';',i,i); }
    for (i=1; i<=nrecs; i++) fh.readLine(buff[i],bufsize); fh.close(); }  
    buff[k]=bu0.setNum(level_id)+';'+bu1.setNum((j))+';'+bu2.setNum(punkte0)+';';
    if (vorrat<vorrat0) bu1=bu0.setNum(vorrat0); else bu1=bu0.setNum(vorrat);
    buff[k]=buff[k]+bu1+"\n";
    if (fh.open(IO_WriteOnly)) { for (i=0; i<=nrecs; i++) { fh.writeBlock(buff[i],buff[i].length()); } 
    fh.close();
  }
  QString raus;
  raus=_GAMESAVED+bname[nr];
  emit messi(raus);
  grab_focus(); 
}

// =============================================================================================================================
