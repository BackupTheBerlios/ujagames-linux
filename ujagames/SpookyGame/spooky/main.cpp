/****************************************************************************************
    Copyright 2005 uja
    Copyright 2004 Trolltech (Qt3)

    This file is part of SpookyShooter.
    
    SpookyShooter is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later vezu Fuss an die QT-Libs III: schafe rsion.
	    
    SpookyShootere is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
	    
    You should have received a copy of the GNU General Public License
    along with Edelsteine; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*****************************************************************************************/
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
#include "wiese.h"    // Gameparameter und -routinen 

#include "lala_arts.h" // Mucke

// ===============================================================================

// -------------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{ int i;
  QApplication my_app(argc,argv);                  // die Fenster/Appli-Steuerung  
  window*      my_window    =new window();         // eine Instanz des zu zeigenden Spiel-Hauptfensters
  hiscore*     my_hiscore   =new hiscore();        // Bestenlisten-Hauptfenster
  textviewer*  my_textviewer=new textviewer();     // Fenster fuer Hilfstexte
  wiese*       my_wiese     =new wiese(my_window); // spezifisch fuer Spiel: Wiese und Bewohner
  viewport*    my_view      =my_window->my_view;   // Darstellungsbereich des Spiels  
  lala*        my_lala      =new lala(); 


// -----------------------------------------------------

  
// Speichernamen und Nr.Bestenlisteneintragspfad:
  int hilist=0;
  QString buff;
  QFile fh1(QString("./hiscore.ini"));
  if (fh1.open(IO_ReadOnly)) { fh1.readLine(buff,254); fh1.close(); }
  hilist=buff.toInt();
  if (hilist>1) hilist=1; else if (hilist<0) hilist=0;  
  
    
// --- allgemeines Spielemenu: -----------------------------------------------------------------------------------------      
  QMenuBar*   my_menubar=new QMenuBar(my_window);  // Menubar mit Window verbinden    

// Menu game: neu, restart level, pause/weiter, Bestenliste/Reset, Quit  
  QPopupMenu* my_menu1=new QPopupMenu;
  my_menubar->insertItem(_GAME,my_menu1);     
  my_menu1->insertItem(_NEU,  my_wiese,SLOT(neues_spiel()),QKeySequence("CTRL+N"));   // _NEW kills Qt-library!      
  my_menu1->insertItem(_PAUSE,my_wiese,SLOT(pause()),      QKeySequence("CTRL+P"));
  my_menu1->insertSeparator();
  my_menu1->insertItem(_QUIT,&my_app,SLOT(quit()),QKeySequence("CTRL+Q"));  

// Optionen:
  QPopupMenu* my_menu1a=new QPopupMenu;
  my_menubar->insertItem(_OPTIONS,my_menu1a);     
  QButtonGroup* my_wig11=new QButtonGroup(1,Qt::Horizontal,_SOUND,0);
  QRadioButton* wig110=new QRadioButton(_ON,my_wig11);
  QRadioButton* wig111=new QRadioButton(_OFF,my_wig11);
  bool cpppatsch;  // irgendein Quatsch, damit der Compiler die Schnauze haelt
  cpppatsch=((wig110) && (wig111));      
  my_wig11->setExclusive(true);
  my_menu1a->insertItem(my_wig11,11); 

// Menu Hiscore: show, reset, use local, use net, save settings 
  QPopupMenu* my_menu2=new QPopupMenu;
  my_menubar->insertItem(_HISCORE,my_menu2);
  my_menu2->insertItem(_SHOWHSC, my_hiscore,SLOT(show_hsc()),QKeySequence("CTRL+B"));
  my_menu2->insertItem(_HSCRESET,my_hiscore,SLOT(reset_hsc())); 
  my_menu2->insertSeparator();
  QButtonGroup* my_wig23=new QButtonGroup(1,Qt::Horizontal,"",0);
  QRadioButton* wig230=new QRadioButton(_LOCALHI,my_wig23);
  QRadioButton* wig231=new QRadioButton(_NETHI,my_wig23);

  cpppatsch=((wig230) && (wig231));      
  my_wig23->setExclusive(true);
  my_wig23->setButton(hilist);
  my_menu2->insertItem(my_wig23,23); 
  my_app.connect(my_wig23,SIGNAL(clicked(int)),my_hiscore,SLOT(set_hscpath(int)));
  my_menu2->insertSeparator();
  my_menu2->insertItem(_SAVEPARM,my_hiscore,SLOT(save_hscpath())); 
 
// Info-Menuitem:Info
  QPopupMenu* my_menu5=new QPopupMenu;
  my_menubar->insertItem(_INFO,my_menu5);
  my_menu5->insertItem(_HELP,my_textviewer,SLOT(show_info(int)),QKeySequence("CTRL+H"),51);
  my_menu5->insertItem(_ABOUT,my_textviewer,SLOT(show_info(int)),QKeySequence("CTRL+A"),52); 
  my_menubar->setItemParameter(51,0);
  my_menubar->setItemParameter(52,1);
 
// --------------------------------------------------------------------------------------------------------------------

// printf("Initialisiere Canvas:\n");

// Wiesen-Sprites belegen und mit Canvas verbinden:

  QCanvas* my_canvas=new QCanvas(XMAX,YMAX);
  my_view->setCanvas(my_canvas);    
  my_canvas->setBackgroundColor(BACKGR1);
  my_canvas->setBackgroundPixmap(QPixmap(BACKPIX1));
  my_canvas->setDoubleBuffering(true);
  my_canvas->setAdvancePeriod(10);

  my_wiese->my_mondarray   =new QCanvasPixmapArray("./images/mond_%1.png",3);
  my_wiese->my_schlossarray=new QCanvasPixmapArray("./images/schloss_%1.png",4);
  my_wiese->my_tuerarray   =new QCanvasPixmapArray("./images/tuer_%1.png",2);
  my_wiese->my_baumarray   =new QCanvasPixmapArray("./images/baum_%1.png",3);
  my_wiese->my_katzenarray =new QCanvasPixmapArray("./images/katze.png");
  my_wiese->my_spinnenarray=new QCanvasPixmapArray("./images/spinne0.png");
  my_wiese->my_vgarray     =new QCanvasPixmapArray("./images/vordergrund.png");
  my_wiese->my_titelarray  =new QCanvasPixmapArray("./images/titel.png");
  my_wiese->my_topfarray   =new QCanvasPixmapArray("./images/topf.png");
  my_wiese->my_waberarray  =new QCanvasPixmapArray("./images/wabern_%1.png",5);
  my_wiese->my_clownarray  =new QCanvasPixmapArray("./images/clown_%1.png",4);
  my_wiese->my_kuerbisarray=new QCanvasPixmapArray("./images/kuerbis_%1.png",24);
  my_wiese->my_hexenarray  =new QCanvasPixmapArray("./images/hexe_%1.png",6);
  my_wiese->my_batarray    =new QCanvasPixmapArray("./images/bat_%1.png",3);
  my_wiese->my_geistarray  =new QCanvasPixmapArray("./images/geist_%1.png",3);
  
  my_wiese->my_baumarray   =new QCanvasPixmapArray("./images/baum_%1.png",2);
  my_wiese->my_shooterarray=new QCanvasPixmapArray("./images/shooter_0000.png");
  my_wiese->my_hotspotarray=new QCanvasPixmapArray("./images/hotspot.png");
  my_wiese->my_punktearray =new QCanvasPixmapArray("./images/punkte_%1.png",8);

  my_wiese->my_goverarray =new QCanvasPixmapArray(GAMEOVERFILE);
  my_wiese->my_pausearray =new QCanvasPixmapArray(PAUSEFILE);
  
  my_wiese->my_mauerarray =new QCanvasPixmapArray("./images/gartenmauer.png");
  my_wiese->my_buscharray =new QCanvasPixmapArray("./images/busch_%1.png",2);
  my_wiese->my_skorparray =new QCanvasPixmapArray("./images/skorpion_%1.png",8);
  my_wiese->my_floharray  =new QCanvasPixmapArray("./images/floh_%1.png",2);
  my_wiese->my_pilzarray  =new QCanvasPixmapArray("./images/pilz.png");
  
  my_wiese->my_scheunenarray=new QCanvasPixmapArray("./images/scheune_%1.png",3);
  my_wiese->my_monsterarray =new QCanvasPixmapArray("./images/monster_%1.png",8);
 
   // Check auf Vollstaendigkeit der Bilder:
  QString err="";
  if (!my_wiese->my_mondarray->isValid())    err=err+"The moon is missing!\n";
  if (!my_wiese->my_schlossarray->isValid()) err=err+"The castle is missing!\n";
  if (!my_wiese->my_tuerarray->isValid())    err=err+"The door is missing!\n";
  if (!my_wiese->my_baumarray->isValid())    err=err+"The wood is missing!\n";
  if (!my_wiese->my_katzenarray->isValid())  err=err+"The cat is missing!\n";
  if (!my_wiese->my_spinnenarray->isValid()) err=err+"The spider is missing!\n";
  if (!my_wiese->my_topfarray->isValid())    err=err+"The Magic Kettle is missing!\n";
  if (!my_wiese->my_waberarray->isValid())   err=err+"The Ingredients for the Magic Soup are missing!\n";
  if (!my_wiese->my_vgarray->isValid())      err=err+"The grass is missing!\n";
  if (!my_wiese->my_clownarray->isValid())   err=err+"The clown is missing!\n";
  if (!my_wiese->my_kuerbisarray->isValid()) err=err+"The pumpkin is missing!\n";
  if (!my_wiese->my_hexenarray->isValid())   err=err+"The witches are missing!\n";
  if (!my_wiese->my_batarray->isValid())     err=err+"The vampires are missing!\n";
  if (!my_wiese->my_geistarray->isValid())   err=err+"The ghosts are missing!\n";
  if (!my_wiese->my_punktearray->isValid())  err=err+"The scoreflags are missing!\n";

  if (!my_wiese->my_shooterarray->isValid()) err=err+"Your shooter is missing!\n";
  if (!my_wiese->my_titelarray->isValid()) err=err+"The title is missing!\n";
  if (!my_wiese->my_goverarray->isValid()) err=err+"Gameover screen is missing!\n";
  if (!my_wiese->my_pausearray->isValid()) err=err+"Pause screen is missing!\n";

  if (!my_wiese->my_titelarray->isValid()) err=err+"The title is missing!\n";
  if (!my_wiese->my_goverarray->isValid()) err=err+"Gameover screen is missing!\n";
  if (!my_wiese->my_pausearray->isValid()) err=err+"Pause screen is missing!\n";

  // Level 2:
  if (!my_wiese->my_buscharray->isValid()) err=err+"Some bush are missing!\n";
  if (!my_wiese->my_mauerarray->isValid()) err=err+"The fence is missing!\n";
  if (!my_wiese->my_floharray->isValid())  err=err+"The flea is missing!\n";
  if (!my_wiese->my_skorparray->isValid()) err=err+"The scorpions are missing!\n";
  if (!my_wiese->my_pilzarray->isValid())  err=err+"The mushroom is missing!\n";

  // Level 3: zerfallene Scheune: Ratten, B.subtilis, Tonnenmonster, Flugbesen
  if (!my_wiese->my_scheunenarray->isValid())  err=err+"The barn is missing!\n";
  if (!my_wiese->my_scheunenarray->isValid())  err=err+"The monster is missing!\n";
  
  
    
  if (err=="")  // Spritebelegung und Initialisierung:
  {    
//  printf("Initialisiere Sprites:\n");

    // statische Sprites:
    my_wiese->p_gover=new QCanvasSprite(my_wiese->my_goverarray,my_canvas);
    my_wiese->p_pause=new QCanvasSprite(my_wiese->my_pausearray,my_canvas);
    my_wiese->p_pause->setZ(35); my_wiese->p_pause->move(0,128);     // pause zweitoberste Prioritaet
    my_wiese->p_gover->setZ(36); my_wiese->p_gover->move(0,128);     // gameover oberste Prioritaet    
    my_wiese->titel=new QCanvasSprite(my_wiese->my_titelarray,my_canvas);
    my_wiese->titel->setZ(1); my_wiese->titel->move(300,8); 
    my_wiese->titel->show();   
    for (i=0; i<4; i++) my_wiese->baum[i]=new QCanvasSprite(my_wiese->my_baumarray,my_canvas);
    for (i=0; i<2; i++)
    { my_wiese->baum[i]->setZ(10); 
      my_wiese->baum[i+2]->setZ(30); 
    }     
    my_wiese->baum[0]->move(-32,80,1);
    my_wiese->baum[1]->move(480,80,1);
    my_wiese->baum[2]->move(400,72,0);
    my_wiese->baum[3]->move(-128,72,0);
    for (i=0; i<4; i++) my_wiese->baum[i]->show(); 

    for (i=0; i<4; i++)  my_wiese->schloss[i]=new QCanvasSprite(my_wiese->my_schlossarray,my_canvas);
    my_wiese->schloss[0]->setZ(10);
    for (i=1; i<4; i++) my_wiese->schloss[i]->setZ(20);
    my_wiese->schloss[0]->move(272,160,0);
    my_wiese->schloss[1]->move(176,208,1);
    my_wiese->schloss[2]->move(112,144,2);
    my_wiese->schloss[3]->move(416,176,3);
    for (i=0; i<4; i++) my_wiese->schloss[i]->show();
    my_wiese->tuer=new QCanvasSprite(my_wiese->my_tuerarray,my_canvas);
    my_wiese->tuer->setZ(21); my_wiese->tuer->move(268,282);
    my_wiese->tuer->show();
    
    my_wiese->topf=new QCanvasSprite(my_wiese->my_topfarray,my_canvas);
    my_wiese->topf->setZ(20); my_wiese->topf->move(36,288);
    my_wiese->topf->show();
    
    my_wiese->p_vordergrund=new QCanvasSprite(my_wiese->my_vgarray,my_canvas);
    my_wiese->p_vordergrund->setZ(32); my_wiese->p_vordergrund->move(0,368);
    my_wiese->p_vordergrund->show();
 
    my_wiese->mauer=new QCanvasSprite(my_wiese->my_mauerarray,my_canvas);
    my_wiese->mauer->setZ(30.5); my_wiese->mauer->move(680,288);
    my_wiese->mauer->show();
 
//    printf("Level 2 ab x=640:\n");
    for (i=0; i<5; i++) my_wiese->busch[i]=new QCanvasSprite(my_wiese->my_buscharray,my_canvas);
    my_wiese->busch[0]->move(1040,144,1); my_wiese->busch[0]->setZ(29.2);
    my_wiese->busch[1]->move(752,256,0); my_wiese->busch[1]->setZ(29.2);
    my_wiese->busch[2]->move(976,256,0); my_wiese->busch[2]->setZ(29.2);
    my_wiese->busch[3]->move(800,188,0); my_wiese->busch[3]->setZ(10); 
    my_wiese->busch[4]->move(640,320,0); my_wiese->busch[4]->setZ(29.7); 
    for (i=0; i<5; i++) my_wiese->busch[i]->show();
    
//    printf("Level 3 ab 1280:\n");
    for (i=0; i<3; i++) my_wiese->scheune[i]=new QCanvasSprite(my_wiese->my_scheunenarray,my_canvas);
    my_wiese->scheune[0]->move(1380,96,0); my_wiese->scheune[0]->setZ(20);
    my_wiese->scheune[1]->move(1420,96,1); my_wiese->scheune[1]->setZ(25.1);
    my_wiese->scheune[2]->move(1380,32,2); my_wiese->scheune[2]->setZ(30.7);
    for (i=0; i<3; i++) my_wiese->scheune[i]->show();
    
    
 // --------------------------------------------------------------------------------------
 // printf("Initialisiere dynamische Sprites:\n");
      
    // dynamische Sprites:
    my_wiese->mond->p_sprite=new QCanvasSprite(my_wiese->my_mondarray,my_canvas);
    my_wiese->mond->p_sprite->setZ(0); my_wiese->mond->p_sprite->move(-OFX,8,0);
    my_wiese->mond->p_sprite->setVelocity(0,0); // stört sonst die Collision-Routine 
    my_wiese->mond->p_sprite->show();

    my_wiese->spinne->p_sprite=new QCanvasSprite(my_wiese->my_spinnenarray,my_canvas);
    my_wiese->spinne->p_sprite->setZ(31); my_wiese->spinne->p_sprite->hide();
    my_wiese->spinne->p_sprite->setVelocity(0,0);
         
    my_wiese->katze->p_sprite=new QCanvasSprite(my_wiese->my_katzenarray,my_canvas);
    my_wiese->katze->p_sprite->setZ(11); my_wiese->katze->p_sprite->hide();
    my_wiese->katze->p_sprite->setVelocity(0,0);
       
    my_wiese->clown->p_sprite=new QCanvasSprite(my_wiese->my_clownarray,my_canvas);
    my_wiese->clown->p_sprite->setZ(21);
    my_wiese->clown->p_sprite->hide();
    my_wiese->clown->p_sprite->setVelocity(0,0);

    my_wiese->waber->p_sprite=new QCanvasSprite(my_wiese->my_waberarray,my_canvas);
    my_wiese->waber->p_sprite->setZ(21); my_wiese->waber->p_sprite->move(36,272);
    my_wiese->waber->p_sprite->hide();
    my_wiese->waber->p_sprite->setVelocity(0,0);    
      
    for (i=0; i<4; i++) 
    { my_wiese->kuerbis[i]->p_sprite=new QCanvasSprite(my_wiese->my_kuerbisarray,my_canvas);
      my_wiese->kuerbis[i]->phase=-1; my_wiese->kuerbis[i]->p_sprite->hide();
      my_wiese->kuerbis[i]->p_sprite->setVelocity(0,0);
    }
    for (i=0; i<4; i++) 
    { my_wiese->hexe[i]->p_sprite=new QCanvasSprite(my_wiese->my_hexenarray,my_canvas);
      my_wiese->hexe[i]->phase=-1; my_wiese->hexe[i]->p_sprite->hide();
      my_wiese->hexe[i]->p_sprite->setVelocity(0,0);
    }    
    for (i=0; i<8; i++) 
    { my_wiese->bat[i]->p_sprite=new QCanvasSprite(my_wiese->my_batarray,my_canvas);
      my_wiese->bat[i]->phase=-1; my_wiese->bat[i]->p_sprite->hide();
      my_wiese->bat[i]->p_sprite->setVelocity(0,0);
    }
    for (i=0; i<8; i++) 
    { my_wiese->geist[i]->p_sprite=new QCanvasSprite(my_wiese->my_geistarray,my_canvas);
      my_wiese->geist[i]->phase=-1; my_wiese->geist[i]->p_sprite->hide();
      my_wiese->geist[i]->p_sprite->setVelocity(0,0);
    }
    for (i=0; i<40; i++) 
    { my_wiese->psprite[i]->p_sprite=new QCanvasSprite(my_wiese->my_punktearray,my_canvas);
      my_wiese->psprite[i]->p_sprite->hide(); my_wiese->psprite[i]->p_sprite->setVelocity(0,0);
    }

    my_wiese->shooter=new QCanvasSprite(my_wiese->my_shooterarray,my_canvas);
    my_wiese->shooter->setZ(33); my_wiese->shooter->move(540,360); 
    my_wiese->shooter->show();
    my_wiese->hotspot=new QCanvasSprite(my_wiese->my_hotspotarray,my_canvas);
    my_wiese->hotspot->setZ(34); my_wiese->hotspot->move(588,384); 
    my_wiese->hotspot->show();
    
//  printf("Level 2: neue Sprites: Pilz, Floh, Skorpione:\n");
    my_wiese->pilz->p_sprite=new QCanvasSprite(my_wiese->my_pilzarray,my_canvas);
    my_wiese->pilz->p_sprite->setZ(9.9); my_wiese->pilz->p_sprite->move(860,256); 
    
    my_wiese->floh->p_sprite=new QCanvasSprite(my_wiese->my_floharray,my_canvas);
    my_wiese->floh->p_sprite->setZ(29.6); my_wiese->floh->p_sprite->hide();
    my_wiese->floh->phase=-1;
    
    for (i=0; i<3; i++) 
    { my_wiese->skorpion[i]->p_sprite=new QCanvasSprite(my_wiese->my_skorparray,my_canvas);
      my_wiese->skorpion[i]->phase=-1; my_wiese->skorpion[i]->p_sprite->hide();
      my_wiese->skorpion[i]->p_sprite->setVelocity(0,0);
    }
          
    
 // printf("Level 3: neue Sprites: Tonne, Monster, Ratten, Subtilis, Besen:\n");
    my_wiese->monster->p_sprite=new QCanvasSprite(my_wiese->my_monsterarray,my_canvas);
    my_wiese->monster->p_sprite->setZ(29.6); my_wiese->monster->p_sprite->hide();
    my_wiese->monster->phase=-1;
    

       
   // --- Ende Spritegenerierung -------------------------------------------

    // Game starten:
    my_hiscore->set_hscpath(hilist);
    my_view->setContentsPos(0,0);
    my_view->polish();
    my_view->show();
  }	    
  else printf(err); // Bilder fehlen


// ---------------------------------------------------------------------------------------------------------------------
  my_app.setMainWidget(my_window);                                                // my_window als Hauptfenster  

  my_app.connect(my_view,SIGNAL(geklickt(int,int)),my_wiese,SLOT(klick(int,int)));   // Klick ins Spielfeld,  Parameter: Quadrant 
  my_app.connect(my_view,SIGNAL(hover(int,int)),my_wiese,SLOT(hover(int,int)));      // Zielfernrohr 

  my_app.connect(my_wiese,SIGNAL(gameover(int)),my_hiscore,SLOT(save_hsc(int)));     // Auswertung Spielende. Parameter: Punkte
  my_app.connect(my_wiese,SIGNAL(bullets_changed(int)),my_window,SLOT(set_bullets(int))); // Kugelvorrat
  my_app.connect(my_wiese,SIGNAL(punkte_changed(int)),my_window,SLOT(set_punkte(int)));   // Punktestandsanzeige
  my_app.connect(my_window->p_nachladen,SIGNAL(clicked()),my_wiese,SLOT(nachladen()));

  my_app.connect(my_wig11,SIGNAL(clicked(int)),my_wiese,SLOT(set_sound(int)));
  my_app.connect(my_wiese,SIGNAL(sound_setting0(bool)),wig110,SLOT(setChecked(bool)));
  my_app.connect(my_wiese,SIGNAL(sound_setting1(bool)),wig111,SLOT(setChecked(bool)));
  my_app.connect(my_wiese,SIGNAL(zeige_view(int,int)),my_window,SLOT(setze_view(int,int)));

  my_app.connect(my_window,SIGNAL(key_pressed(int)),my_wiese,SLOT(keyhandle(int)));     // Keypress im Spielfeld
  my_app.connect(my_wiese,SIGNAL(focus2playfield()),my_window,SLOT(grab_focus()));
  my_app.connect(my_wiese,SIGNAL(scrollit(int)),my_window,SLOT(scroll_it(int)));
  my_app.connect(my_window,SIGNAL(gescrollt(int)),my_wiese,SLOT(shooterupdate(int)));     
 
  my_app.connect(my_wiese,SIGNAL(play_lala(bool,int)),my_lala,SLOT(mach_lala(bool,int)));

// ---------------------------------------------------  
  
  
  
// ----------------------------------------------------  
//  printf("starte spiel\n");  
  QPixmap my_pixmap=QPixmap(PROGICON);                                            // Programmicon setzen  
  my_window->setIcon(my_pixmap);
  my_wiese->set_sound(0);
  my_wiese->neues_spiel();
  my_window->show();                      // anzeigen
//  printf("Breite x Hoehe %i x %i\n",my_window->width(),my_window->height());    // Debug-Info

  return my_app.exec();                   // Exec-Schleife der my_app
}
