/****************************************************************************************
    Copyright 2004 uja
    Copyright 2004 Trolltech (Qt3)

    This file is part of bricks.
    
    bricks is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later vezu Fuss an die QT-Libs III: schafe rsion.
	    
    bricks is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
	    
    You should have received a copy of the GNU General Public License
    along with Edelsteine; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*****************************************************************************************/

// Hauptprogramm bricks
// Module erzeugen, Parameter lesen, Menu erstellen, Canvas und Sprites generieren, mit Appli verbinden
// --------------------------------------------------------------------------------------------------------

#include <qapplication.h>      // Fensterhandling-Event-Routinen
#include <qmenubar.h>          // Menuleiste
#include <qmenudata.h>         // Menu-Items
#include <qaccel.h>            // Shortcuts
#include <qradiobutton.h>      // Auswahl
#include <qbuttongroup.h>      // Auswahl
#include <qfile.h>             // INI-Dateien auswerten
#include <stdio.h>             // debugging

#include "lang.h"     // laenderspezifische Texte
#include "config.h"   // Paraneter Darstellung, Spielgroesse, Dateien
#include "gamelib.h"  // ujas gamelib: viewport, hiscore, info-textmodule
#include "window.h"   // Hauptfenster Darstellung
#include "game.h"    // Gameparameter und -routinen 
#include "lala.h"    // Sound
// ===============================================================================

// -------------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{ 
  QApplication my_app(argc,argv);                  // die Fenster/Appli-Steuerung  
  window*      my_window    =new window();         // eine Instanz des zu zeigenden Spiel-Hauptfensters
  hiscore*     my_hiscore   =new hiscore();        // Bestenlisten-Hauptfenster
  textviewer*  my_textviewer=new textviewer();     // Fenster fuer Hilfstexte
  game*        my_game      =new game(my_window,"game"); // spezifisch fuer Spiel: Wiese und Bewohner
  viewport*    my_view      =my_window->my_view;   // Darstellungsbereich des Spiels  
  lala*        my_lala      =new lala();
  int i,k;

// Speichernamen und Nr.Bestenlisteneintragspfad:
  int hilist=0;
  QString buff;
  QString speicher[MAXSPEICHER];
  QFile fh(QString("./speicher.ini"));
  if (fh.open(IO_ReadOnly)) { fh.readLine(buff,254); fh.close(); } 
  for (i=0; i<MAXSPEICHER; i++) { speicher[i]=buff.section(';',i,i); }

  QFile fh1(QString("./hiscore.ini"));
  if (fh1.open(IO_ReadOnly)) { fh1.readLine(buff,254); fh1.close(); }
  hilist=buff.toInt();
  if (hilist>1) hilist=1; else if (hilist<0) hilist=0;  

  QFile fh2(QString("./gameparm.ini"));
  if (fh2.open(IO_ReadOnly)) { fh2.readLine(buff,254); fh2.close(); }
  QString buff2;
  buff2=buff.section(';',0,0); int soundon=buff2.toInt();
  buff2=buff.section(';',1,1); int tileset=buff2.toInt();
  buff2=buff.section(';',2,2); int bgbild=buff2.toInt();

// --- allgemeines Spielemenu: -----------------------------------------------------------------------------------------      
  QMenuBar*   my_menubar=new QMenuBar(my_window);  // Menubar mit Window verbinden    

// Menu game: neu, restart level, pause/weiter, Bestenliste/Reset, Quit  
  QPopupMenu* my_menu1=new QPopupMenu;
  my_menubar->insertItem(_GAME,my_menu1);     
  my_menu1->insertItem(_NEU,    my_game,SLOT(neues_spiel()),QKeySequence("CTRL+N"));   // _NEW kills Qt-library!      
  my_menu1->insertItem(_RESTART,my_game,SLOT(restart()),    QKeySequence("CTRL+L"));
  my_menu1->insertItem(_PAUSE,  my_game,SLOT(pause()),      QKeySequence("CTRL+P"));
  my_menu1->insertItem(_TILT,   my_game,SLOT(tilten()),     QKeySequence("CTRL+T"));
  my_menu1->insertSeparator();
  my_menu1->insertItem(_QUIT,&my_app,SLOT(quit()),QKeySequence("CTRL+Q"));  

// ----------------------------------------------------------------------
  QPopupMenu* my_menu7=new QPopupMenu;

  my_menubar->insertItem(_OPTIONS,my_menu7); 
  QButtonGroup* my_wig70=new QButtonGroup(1,Qt::Horizontal,_SOUND,0);
  QRadioButton* wig700=new QRadioButton(_ON,my_wig70);
  QRadioButton* wig701=new QRadioButton(_OFF,my_wig70);
  bool cpppatsch; cpppatsch=((wig700) && (wig701)); // irgendein Quatsch, damit der Compiler die Schnauze haelt
  my_wig70->setExclusive(true);
  my_wig70->setButton(soundon);
  my_menu7->insertItem(my_wig70,70); 
  my_app.connect(my_wig70,SIGNAL(clicked(int)),my_game,SLOT(set_sound(int)));

  QButtonGroup* my_wig71=new QButtonGroup(1,Qt::Horizontal,"Tileset",0);
  QRadioButton* wig710=new QRadioButton("Simpel",my_wig71); wig710->setPixmap(QPixmap("./images/s_0001.png"));
  QRadioButton* wig711=new QRadioButton("Metall",my_wig71); wig711->setPixmap(QPixmap("./images/b_48x24_0001.png"));
  QRadioButton* wig712=new QRadioButton("Glas",my_wig71); wig712->setPixmap(QPixmap("./images/h_0001.png"));
  my_wig71->setExclusive(true);
  my_wig71->setButton(tileset);
  my_menu7->insertItem(my_wig71,71); 
  my_app.connect(my_wig71,SIGNAL(clicked(int)),my_game,SLOT(set_tileset(int)));

  QButtonGroup* my_wig72=new QButtonGroup(1,Qt::Horizontal,"Background",0);
  QRadioButton* wig720=new QRadioButton(_BILD0,my_wig72);
  QRadioButton* wig721=new QRadioButton(_BILD1,my_wig72);
  QRadioButton* wig722=new QRadioButton(_BILD4,my_wig72);
  QRadioButton* wig723=new QRadioButton(_BILD5,my_wig72);
  cpppatsch=((wig720) && (wig721));
  cpppatsch=((wig722) && (wig723));
  my_wig72->setExclusive(true);
  my_wig72->setButton(bgbild);
  my_menu7->insertItem(my_wig72,72); 
  my_app.connect(my_wig72,SIGNAL(clicked(int)),my_game,SLOT(set_background(int)));
  my_menu7->insertItem("Parameter speichern",my_game,SLOT(save_settings()));

// ------------------------------------------------------------------------------------------------
// Levelfile einstellen:
/*
  QPopupMenu* my_menu2=new QPopupMenu;
  my_menubar->insertItem(_LEVELFILE,my_menu2);     
  QButtonGroup* my_wig21=new QButtonGroup(1,Qt::Horizontal,_NIX,0);
  QRadioButton* wig210=new QRadioButton(_DEFAULT,my_wig21);
  QRadioButton* wig211=new QRadioButton(_USERLEVEL,my_wig21);
  cpppatsch=((wig210) && (wig211));
  my_wig21->setExclusive(true);
  my_wig21->setButton(0);
  my_menu2->insertItem(my_wig21,21); 
*/

// ----------------------------------------------------------------------------------------------
// Menu Hiscore: show, reset, use local, use net, save settings 
  QPopupMenu* my_menu3=new QPopupMenu;
  my_menubar->insertItem(_HISCORE,my_menu3);
  my_menu3->insertItem(_SHOWHSC, my_hiscore,SLOT(show_hsc()),QKeySequence("CTRL+B"));
  my_menu3->insertItem(_HSCRESET,my_hiscore,SLOT(reset_hsc())); 
  my_menu3->insertSeparator();
  QButtonGroup* my_wig33=new QButtonGroup(1,Qt::Horizontal,"",0);
  QRadioButton* wig330=new QRadioButton(_LOCALHI,my_wig33);
  QRadioButton* wig331=new QRadioButton(_NETHI,my_wig33);
  cpppatsch=((wig330) && (wig331));
  my_wig33->setExclusive(true);
  my_wig33->setButton(hilist);
  my_menu3->insertItem(my_wig33,33); 
  my_app.connect(my_wig33,SIGNAL(clicked(int)),my_hiscore,SLOT(set_hscpath(int)));
  my_menu3->insertSeparator();
  my_menu3->insertItem(_SAVEPARM,my_hiscore,SLOT(save_hscpath())); 

// ---------------------------------------------------------------------------------------------
// laden und speichern vom Spielstandspeicher:
  QPopupMenu* my_menu4=new QPopupMenu;
  my_menubar->insertItem(_LOAD,my_menu4);
  for (i=0; i<MAXSPEICHER; i++) 
  { k=i+40;
    my_menu4->insertItem(speicher[i],my_game,SLOT(lade_spielstand(int)),0,k);
    my_menubar->setItemParameter(k,i);
  }
  
  QPopupMenu* my_menu5=new QPopupMenu;
  my_menubar->insertItem(_SAVE,my_menu5);
  for (i=0; i<MAXSPEICHER; i++) 
  { k=i+50;
    my_menu5->insertItem(speicher[i],my_game,SLOT(speichere_spielstand(int)),0,k);
    my_menubar->setItemParameter(k,i);
  }
// ------------------------------------------------------------------------------------------

// Info-Menuitem:I
  QPopupMenu* my_menu6=new QPopupMenu;
  my_menubar->insertItem(_INFO,my_menu6);
  my_menu6->insertItem(_HELP,my_textviewer,SLOT(show_info(int)),QKeySequence("CTRL+H"),61);
  my_menu6->insertItem(_ABOUT,my_textviewer,SLOT(show_info(int)),QKeySequence("CTRL+A"),62); 
  my_menubar->setItemParameter(61,0);
  my_menubar->setItemParameter(62,1);
 
// --------------------------------------------------------------------------------------------------------------------
// Canvas fuer Sprites e#define INIFILE   "./gameparm.ini"rzeugen, mit Viewport verbinden,
// Wiesen-Sprites belegen und mit Canvas verbinden:

  QCanvas* my_canvas=new QCanvas(DX*XMAX,DY*(YMAX+5));
  my_view->setCanvas(my_canvas);
  my_canvas->setBackgroundColor(BACKGR1);
  my_canvas->setDoubleBuffering(true);
  my_canvas->setAdvancePeriod(10);

  my_game->goverarray=new QCanvasPixmapArray(GAMEOVERFILE);
  my_game->pausearray=new QCanvasPixmapArray(PAUSEFILE);
  my_game->tuerarray =new QCanvasPixmapArray("./images/tuer.png");
 
  my_game->paddelarray =new QCanvasPixmapArray(PADDELSPRITE,6);
  my_game->ballarray   =new QCanvasPixmapArray(BALLSPRITE);
  my_game->blockarray  =new QCanvasPixmapArray(BLOCKSPRITE,12);
  my_game->blockarray0 =new QCanvasPixmapArray(TILESET0,12);
  my_game->blockarray1 =new QCanvasPixmapArray(TILESET1,12);
  my_game->blockarray2 =new QCanvasPixmapArray(TILESET2,12);

  my_game->goodiearray =new QCanvasPixmapArray(GOODIESPRITE,16);
  my_game->floaterarray=new QCanvasPixmapArray(FLOATERSPRITE,24);
  my_game->laserarray  =new QCanvasPixmapArray(LASERSPRITE);

  // Check auf Vollstaendigkeit der Bilder:
  QString err="";
  if (!my_game->goverarray->isValid())     err=err+"Gameover Pic is missing!\n";
  if (!my_game->pausearray->isValid())     err=err+"Pause Pic is missing!\n";
  if (!my_game->paddelarray->isValid())    err=err+"Some Racket Pics are missing!\n";
  if (!my_game->ballarray->isValid())      err=err+"The Ball Pic is missing!\n";
  if (!my_game->blockarray->isValid())     err=err+"Some Block Pics are missing!\n";
  if (!my_game->goodiearray->isValid())    err=err+"Some Goodie Pics are missing!\n";
  if (!my_game->floaterarray->isValid())   err=err+"Floater Pic is missing!\n";
  if (!my_game->laserarray->isValid())     err=err+"Lasershot Pic is missing!\n";
  if (!my_game->tuerarray->isValid())      err=err+"Door Pic is missing!\n";
  
  if (err=="")  // Spritebelegung und Initialisierung:
  { my_game->p_gover=new QCanvasSprite(my_game->goverarray,my_canvas);
    my_game->p_pause=new QCanvasSprite(my_game->pausearray,my_canvas);
    my_game->p_pause->move(3*DX,5*DY); my_game->p_pause->setZ(98); my_game->p_pause->setVelocity(0,0);
    my_game->p_gover->move(3*DX,5*DY); my_game->p_gover->setZ(99); my_game->p_gover->setVelocity(0,0);

    my_game->paddel->p_sprite=new QCanvasSprite(my_game->paddelarray,my_canvas); 
    my_game->paddel->p_sprite->setVelocity(0,0);
    my_game->paddel->p_sprite->move(XMAX/2*DX,(YMAX+4)*DY,0); 
    my_game->paddel->p_sprite->setZ(1);
    my_game->paddel->p_sprite->show();

    my_game->tuer=new QCanvasSprite(my_game->tuerarray,my_canvas);
    my_game->tuer->move(XMAX*DX-my_game->tuer->width(),my_game->paddel->p_sprite->y());
    my_game->tuer->setZ(0); 
    my_game->tuer->setVelocity(0,0);

    for (i=0; i<my_game->NUM_LASERS; i++)
    { my_game->laser[i]->p_sprite=new QCanvasSprite(my_game->laserarray,my_canvas); 
      my_game->laser[i]->p_sprite->setVelocity(0,0);
      my_game->laser[i]->p_sprite->setZ(0.2); 
      my_game->laser[i]->p_sprite->move(0,i*32+320,0);
    }
    for (i=0; i<my_game->NUM_BALLS; i++) 
    { my_game->ball[i]->p_sprite=new QCanvasSprite(my_game->ballarray,my_canvas);
      my_game->ball[i]->p_sprite->hide(); my_game->ball[i]->p_sprite->setVelocity(0,0);
    }
    int x=0,y=0;
    for (i=0; i<SMAX; i++) 
    { x=i%XMAX; y=(i-x)/XMAX;
      x=x*DX; y=y*DY;
      my_game->block[i]=new QCanvasSprite(my_game->blockarray,my_canvas);
      my_game->block[i]->setZ(2.0+0.001*i); 
      my_game->block[i]->move(x,y,i%12);
      my_game->block[i]->show();
      my_game->block[i]->setVelocity(0,0);
    }
    for (i=0; i<my_game->NUM_GOODIES; i++)
    { my_game->goodie[i]->p_sprite=new QCanvasSprite(my_game->goodiearray,my_canvas);
      my_game->goodie[i]->p_sprite->setVelocity(0,0);
      my_game->goodie[i]->p_sprite->hide();
    }
    for (i=0; i<my_game->NUM_FLOATERS; i++)
    {  my_game->floater[i]->p_sprite=new QCanvasSprite(my_game->floaterarray,my_canvas);
       my_game->floater[i]->p_sprite->setVelocity(0,0);
       my_game->floater[i]->p_sprite->hide();
    }
    my_hiscore->set_hscpath(hilist);
    my_game->set_tileset(tileset);
    my_game->set_background(bgbild);
    my_view->setContentsPos(0,0);
    my_view->polish();
    my_view->show();
  }	    
  else printf(err); // Bilder fehlen
// ---------------------------------------------------------------------------------------------------------------------
  my_app.setMainWidget(my_window);
  my_app.connect(my_view,SIGNAL(geklickt(int,int)),my_game,SLOT(geklickt(int,int)));   // Klick ins Spielfeld,  Parameter: Quadrant 
  my_app.connect(my_view,SIGNAL(hover(int,int)),my_game,SLOT(hover(int,int)));      // Zielfernrohr 
  my_app.connect(my_game,SIGNAL(gameover(int)),my_hiscore,SLOT(save_hsc(int)));     // Auswertung Spielende. Parameter: Punkte
  my_app.connect(my_game,SIGNAL(vorrat_changed(int)),my_window,SLOT(set_leben(int))); // Kugelvorrat
  my_app.connect(my_game,SIGNAL(punkte_changed(int)),my_window,SLOT(set_punkte(int)));   // Punktestandsanzeige
  my_app.connect(my_game,SIGNAL(sound_setting0(bool)),wig700,SLOT(setChecked(bool)));
  my_app.connect(my_game,SIGNAL(sound_setting1(bool)),wig701,SLOT(setChecked(bool)));
  my_app.connect(my_game,SIGNAL(messi(QString)),my_window,SLOT(set_messi(QString)));
  my_app.connect(my_game,SIGNAL(levelname_changed(QString)),my_window,SLOT(set_level(QString)));
  my_app.connect(my_game,SIGNAL(play_lala(bool,int)),my_lala,SLOT(mach_lala(bool,int)));
  my_app.connect(my_game,SIGNAL(rest_changed(int)),my_window,SLOT(set_rest(int)));

  my_app.connect(my_window->p_neu,SIGNAL(clicked()),my_game,SLOT(neues_spiel()));
  my_app.connect(my_window->p_pause,SIGNAL(clicked()),my_game,SLOT(toggle_pause()));
  my_app.connect(my_window->p_tilt,SIGNAL(clicked()),my_game,SLOT(tilten()));

  QPixmap my_pixmap=QPixmap(PROGICON); 
  my_window->setIcon(my_pixmap);
  my_window->show();  
  my_game->neues_spiel();

//  printf("Breite x Hoehe %i x %i\n",my_window->width(),my_window->height());    // Debug-Info
  return my_app.exec();
}
