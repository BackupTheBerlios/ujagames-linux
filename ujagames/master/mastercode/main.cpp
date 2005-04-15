/****************************************************************************************
    Copyright 2004.2005 uja
    Copyright 2004 Trolltech (Qt3)

    This file is part of mastercode.
    mastercode is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later vezu Fuss an die QT-Libs III: schafe rsion.
    mastercode is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Edelsteine; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*****************************************************************************************
 Changelog 1.0 -> 1.1
 1. Loesung wird in Steinen angezeigt
 2. 2 Steinsets waehlbar
 3. diverse Hintergruende waehlbar
------------------------------------------

*/
// Hauptprogramm masercode
// Module erzeugen, Parameter lesen, Menu erstellen, Canvas und Sprites generieren, mit Appli verbinden
// --------------------------------------------------------------------------------------------------------

#include <qapplication.h>      // Fensterhandling-Event-Routinen
#include <qmenubar.h>          // Menuleiste
#include <qmenudata.h>         // Menu-Items
#include <qaccel.h>            // Shortcuts
#include <qspinbox.h>          // Zahlen-Auswahl 
#include <qradiobutton.h>
#include <qgrid.h>             // Layout
#include <qfile.h>             // INI-Dateien auswerten
#include <stdio.h>             // debugging

#include "lang.h"     // laenderspezifische Texte
#include "config.h"   // Paraneter Darstellung, Spielgroesse, Dateien
#include "gamelib.h"  // ujas gamelib: viewport, hiscore, info-textmodule
#include "window.h"   // Hauptfenster Darstellung
#include "game.h"    // Gameparameter und -routinen 
// ===============================================================================

// -------------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{ 
  QApplication my_app(argc,argv);                  // die Fenster/Appli-Steuerung  
  window*      my_window    =new window();         // eine Instanz des zu zeigenden Spiel-Hauptfensters
  textviewer*  my_textviewer=new textviewer();     // Fenster fuer Hilfstexte
  game*        my_game      =new game(my_window,"game"); // spezifisch fuer Spiel: Wiese und Bewohner
  viewport*    my_view      =my_window->my_view;   // Darstellungsbereich des Spiels  
  int i,j,k,k1,x,y;

// ini-File auswerten: ------------------------------------------------------------------------------------------------
  int numf=4,numt=6,numg=0,nump=0,nstein=0,nbg=0;
  QString buff,bu;
  QFile fh(QString("./gameparm.ini"));
  if (fh.open(IO_ReadOnly)) { fh.readLine(buff,254); fh.close(); }
  bu=buff.section(';',0,0); numg=bu.toInt();
  bu=buff.section(';',1,1); nump=bu.toInt();
  bu=buff.section(';',2,2); numf=bu.toInt();
  bu=buff.section(';',3,3); numt=bu.toInt();
  bu=buff.section(';',4,4); nstein=bu.toInt();
  bu=buff.section(';',5,5); nbg=bu.toInt();

// --- allgemeines Spielemenu: ----------------------------------------------------------------------------------------
  QMenuBar* my_menubar=new QMenuBar(my_window);  // Menubar mit Window verbinden    

// Menu game: neu, restart level, pause/weiter, Bestenliste/Reset, Quit  
  QPopupMenu* my_menu1=new QPopupMenu;
  my_menubar->insertItem(_GAME,my_menu1);     
  my_menu1->insertItem(_NEU,      my_game,SLOT(neues_spiel()),QKeySequence("CTRL+N"));
  my_menu1->insertItem(_RESETSTAT,my_game,SLOT(reset_stat()), QKeySequence("CTRL+R"));
  my_menu1->insertSeparator();
  my_menu1->insertItem(_QUIT,&my_app,SLOT(quit()),QKeySequence("CTRL+Q"));  

// ----------------------------------------------------------------------

// Brett einstellen:
  QPopupMenu* my_menu2=new QPopupMenu;
  my_menubar->insertItem(_OPTIONS,my_menu2);
  
  QGrid* my_wig21=new QGrid(2,Qt::Horizontal,0);
  QLabel* nfrb=new QLabel(_NFARBEN,my_wig21);
  QSpinBox* wig210=new QSpinBox(4,XMAX,1,my_wig21,_NFARBEN);
  QLabel* ntoe=new QLabel(_NTOEPFE,my_wig21);
  QSpinBox* wig211=new QSpinBox(4,XMAX,1,my_wig21,_NTOEPFE);
  
  QButtonGroup* my_wig22=new QButtonGroup(1,Qt::Horizontal,_STEINE);
  QRadioButton* wig220=new QRadioButton("_SCHLIFF",my_wig22); wig220->setPixmap(QPixmap("./images/s_0000.png"));
  QRadioButton* wig221=new QRadioButton("_KUGEL",my_wig22);   wig221->setPixmap(QPixmap("./images/s_0010.png"));
  QRadioButton* wig222=new QRadioButton("_BW",my_wig22);      wig222->setPixmap(QPixmap("./images/s_0020.png"));
  my_wig22->setExclusive(true);
  my_wig22->setButton(nstein);

  QButtonGroup* my_wig23=new QButtonGroup(1,Qt::Horizontal,"Background");
  QRadioButton* wig230=new QRadioButton(_GRANIT,my_wig23);
  QRadioButton* wig231=new QRadioButton(_HOLZ,my_wig23);
  QRadioButton* wig232=new QRadioButton(_AQUA,my_wig23);
  QRadioButton* wig233=new QRadioButton(_KEINBG,my_wig23);
  my_wig23->setExclusive(true);
  my_wig23->setButton(nbg);

  my_menu2->insertItem(my_wig21);
  my_menu2->insertItem(my_wig22);
  my_menu2->insertItem(my_wig23);
  my_menu2->insertItem(_SAVESETTINGS,my_game,SLOT(save_settings()),QKeySequence("CTRL+S"));
  wig210->setValue(numf);
  wig211->setValue(numt);

// ------------------------------------------------------------------------------------------

// Info-Menuitem:I
  QPopupMenu* my_menu6=new QPopupMenu;
  my_menubar->insertItem(_INFO,my_menu6);
  my_menu6->insertItem(_HELP,my_textviewer,SLOT(show_info(int)),QKeySequence("CTRL+H"),61);
  my_menu6->insertItem("About",my_window,SLOT(about()),QKeySequence("CTRL+A"),62); 
  my_menu6->insertItem("About Qt",&my_app,SLOT(aboutQt()),QKeySequence("CTRL+T"),63); 
  my_menubar->setItemParameter(61,0);
 
// --------------------------------------------------------------------------------------------------------------------
// Canvas fuer Sprites e#define INIFILE   "./gameparm.ini"rzeugen, mit Viewport verbinden,
// Wiesen-Sprites belegen und mit Canvas verbinden:
// printf("Canvas\n");

  QCanvas* my_canvas=new QCanvas((DX+DX2)*XMAX+OFX,DY*YMAX);
  my_view->setCanvas(my_canvas);
  my_canvas->setBackgroundColor(BACKGR1);
  my_canvas->setBackgroundPixmap(QPixmap("./images/bgr4.png"));

  my_canvas->setDoubleBuffering(true);
  my_canvas->setAdvancePeriod(50);

  my_game->chiparray  =new QCanvasPixmapArray(STEINSPRITE,3*XMAX);
  my_game->bgarray    =new QCanvasPixmapArray(BGSPRITE);
  my_game->markerarray=new QCanvasPixmapArray(MARKERSPRITE,3);

  // Check auf Vollstaendigkeit der Bilder:
  QString err="";
  if (!my_game->chiparray->isValid())   err=err+"Some gems are missing!\n";
  if (!my_game->markerarray->isValid()) err=err+"Some markers are missing!\n";
  if (!my_game->bgarray->isValid())     err=err+"The placeholder is missing!\n";

// printf("Sprites\n");

  if (err=="")  // Spritebelegung und Initialisierung:
  { my_game->p_cursor=new QCanvasSprite(my_game->chiparray,my_canvas);
    my_game->p_cursor->setZ(3);
    k=0;
    k1=0;
    y=0;
    for (j=0; j<YMAX; j++)
    { x=0;
      for (i=0; i<XMAX; i++)
      { my_game->p_bgsprite[k]=new QCanvasSprite(my_game->bgarray,my_canvas); 
        my_game->p_bgsprite[k]->setVelocity(0,0);
        my_game->p_bgsprite[k]->setZ(1); 
        my_game->p_bgsprite[k]->move(x,y,0);
        my_game->p_bgsprite[k]->show();

        my_game->p_stein[k]=new QCanvasSprite(my_game->chiparray,my_canvas); 
        my_game->p_stein[k]->setVelocity(0,0);
        my_game->p_stein[k]->setZ(2); 
        my_game->p_stein[k]->move(x,y,k%9);
        if (k<42) my_game->p_stein[k]->show();
        k++; 
        x=x+DX;
      }
      for (i=0; i<XMAX; i++)
      { my_game->p_marker[k1]=new QCanvasSprite(my_game->markerarray,my_canvas); 
        my_game->p_marker[k1]->setVelocity(0,0);
        my_game->p_marker[k1]->setZ(2); 
        my_game->p_marker[k1]->move(x,y+4,0);
        my_game->p_marker[k1]->show();
        k1++; 
        x=x+DX2;
      }
      y=y+DY;
   }
    my_view->setContentsPos(0,0);
    my_view->polish();
    my_view->show();
  }	    
  else printf(err); // Bilder fehlen
// ---------------------------------------------------------------------------------------------------------------------
// printf("Connections\n");

  my_app.setMainWidget(my_window);
  my_app.connect(my_view,SIGNAL(geklickt(int,int)),my_game,SLOT(geklickt(int,int)));   // Klick ins Spielfeld,  Parameter: Quadrant 
  my_app.connect(my_view,SIGNAL(hover(int,int)),my_game,SLOT(hover(int,int)));      // Zielfernrohr 
  my_app.connect(my_game,SIGNAL(punkte_changed(int)),my_window,SLOT(set_punkte(int)));   // Punktestandsanzeige
  my_app.connect(my_game,SIGNAL(messi(QString)),my_window,SLOT(set_messi(QString)));
  my_app.connect(my_game,SIGNAL(updat_farben(int)),my_window,SLOT(set_numfarben(int)));
  my_app.connect(my_game,SIGNAL(lauf_changed(int)),my_window,SLOT(set_numgames(int)));
  my_app.connect(my_game,SIGNAL(summe_changed(int)),my_window,SLOT(set_sumpunkte(int)));
  my_app.connect(my_game,SIGNAL(ratio_changed(int,int)),my_window,SLOT(set_ratio(int,int)));
  my_app.connect(my_window->p_auswahl,SIGNAL(clicked(int)),my_game,SLOT(stein_gewaehlt(int)));
  my_app.connect(my_window->p_auswerten,SIGNAL(clicked()),my_game,SLOT(auswerten()));
  my_app.connect(my_window->p_neue_runde,SIGNAL(clicked()),my_game,SLOT(neues_spiel()));
  my_app.connect(wig210,SIGNAL(valueChanged(int)),my_game,SLOT(set_farben(int)));
  my_app.connect(wig211,SIGNAL(valueChanged(int)),my_game,SLOT(set_toepfe(int)));
// V.1.1:
  my_app.connect(my_window,SIGNAL(farbset_changed(int)),my_game,SLOT(farbset_changed(int)));
  my_app.connect(my_window,SIGNAL(bgset_changed(int)),my_game,SLOT(bgset_changed(int)));
  my_app.connect(my_game,SIGNAL(loesung(QString)),my_window,SLOT(show_code(QString)));
  my_app.connect(my_game,SIGNAL(loe_loeschen()),my_window,SLOT(loesche_code()));
  my_app.connect(my_wig22,SIGNAL(clicked(int)),my_window,SLOT(farbset(int)));
  my_app.connect(my_wig23,SIGNAL(clicked(int)),my_window,SLOT(bgset(int)));

  QPixmap my_pixmap=QPixmap(PROGICON); 
  my_window->setIcon(my_pixmap);
  my_window->show();
  my_game->set_lauf(numg);
  my_game->set_summe(nump);
  my_game->set_farben(numf);
  my_game->set_toepfe(numt);
  my_window->farbset(nstein);
  my_window->bgset(nbg);

  
  
// printf("Breite x Hoehe %i x %i\n",my_window->width(),my_window->height());    // Debug-Info
  return my_app.exec();
}
