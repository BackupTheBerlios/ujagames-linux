/// ==================================================================================================================
// ujagames_qt: Hauptmodul-Muster des Typs Patience - (c) uja 09/04
// Aufbau Menu, Canvas(se), Initialisierung aller Objekte, soweit nicht im Objekt selbst moeglich

// Menu 1: Neues_Spiel, Level von vorne, Bestenliste ansehen, Bestenliste zuruecksetzen, Quit
// Menu 4: Infos
// Individualisierung geschieht durch config.h, nicht benoetigte Teile sind auskommentiert.

// Check und Einrichten der Sprites im entsprechendem Canvas, ausser den Spielsteinen und Marken
// sind dies das Pause- und Gameoverbild 
// Initialisierung der Spielparameter aus gameparm.ini und Start der 1.Runde
// -----------------------------------------------------------------------------------------------------------------
 
// =================================================================================================================
// Variante Neuner

#include "config.h"     // ein paar Konstanten
#include "lang.h"

#include "gamelib.h"    // konstante Module
#include "window.h"     // Hauptfenster Darstellung
#include "chips.h"      // Spielwiese
#include <qapplication.h>      // Fensterhandling-Event-Routinen
#include <qmenubar.h>          // Menuleiste
#include <qmenudata.h>         // Menu-Items
#include <qaccel.h>            // Shortcuts
#include <stdio.h>             // debugging
// -------------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{ 
  QApplication my_app(argc,argv);                  // die Fenster/Appli-Steuerung
  window*      my_window    =new window();         // eine Instanz des zu zeigenden Spiel-Hauptfensters
  textviewer*  my_textviewer=new textviewer();     // Fenster fuer Hilfstexte
  chips*       my_chips     =new chips(my_window); // spezifisch fuer Spiel
  viewport*    my_view      =my_window->my_view;   // Darstellungsbereich des Spiels
  
     
// allgemeines Spielemenu: 
// ---------------------------------------------------------------------------------------      
  QMenuBar*   my_menubar=new QMenuBar(my_window);  // Menubar mit Window verbinden    
 
// Menu 1:  neues Spiel, von vorne, Quit 
  QPopupMenu* my_menu1 = new QPopupMenu; 
  my_menubar->insertItem(_MENU1,my_menu1);   
  my_menu1->insertItem(_NEWGAME,my_chips,SLOT(neues_spiel()),QKeySequence("CTRL+N"));
  my_menu1->insertItem(_RESTARTGAME,my_chips,SLOT(spielstart()),QKeySequence("CTRL+L"));
  my_menu1->insertSeparator();
  my_menu1->insertItem(_QUIT,&my_app,SLOT(quit()),QKeySequence("CTRL+Q"));
  
// Menu 2:  austeilen, abraeumen: 
  QPopupMenu* my_menu2 = new QPopupMenu; 
  my_menubar->insertItem(_MENU2,my_menu2);   
  my_menu2->insertItem(_DEAL,my_chips,SLOT(reihe_austeilen()),QKeySequence("CTRL+R"));
  my_menu2->insertItem(_PLAY,my_chips,SLOT(autoplay()),QKeySequence("CTRL+A"));

    
// Menu 4: Spielanleiting, Info, Lizenz
  QPopupMenu* my_menu4 = new QPopupMenu;
  my_menubar->insertItem(_MENU4,my_menu4);
  my_menu4->insertItem(_HELP,my_textviewer,SLOT(show_info(int)),QKeySequence("CTRL+H"),41);
  my_menu4->insertItem(_INFO,my_textviewer,SLOT(show_info(int)),QKeySequence("CTRL+I"),42); 
  my_menubar->setItemParameter(42,0);
  my_menubar->setItemParameter(41,1);

// restliche Kommunikationswege festlegen:
// ----------------------------------------
  my_app.connect(my_view,SIGNAL(geklickt(int,int)),my_chips,SLOT(klick(int,int)));      // Klick ins Spielfeld, x,y 


  
// --- Ende INI-Files und Menus ---
// ===========================================================================================================================

// --------------------------------------------------------------------------------------------------------------------
// leider gings nicht anders, der QCanvas und seine Belegung musste ins main:
// This patience uses 2 decks, each with a different background

  int breite=XMAX*DX+2*OFX;
  QCanvas* my_canvas=new QCanvas(breite,104*DDY+DY+OFY); // GAU: alle Karten in 1 Spalte
  my_view->setCanvas(my_canvas);    
  my_canvas->setBackgroundColor(BACKGR1);
  my_canvas->setDoubleBuffering(true);
  my_canvas->setAdvancePeriod(10);       // 10ms-Ticker

  QCanvasLine* my_dline=new QCanvasLine(my_canvas);
  my_dline->setPoints(0,OFY-9,breite,OFY-9);
  my_dline->setPen(QPen(RANDDCOLOR,1));
  my_dline->show();
  QCanvasLine* my_hline=new QCanvasLine(my_canvas);
  my_hline->setPoints(0,OFY-8,breite,OFY-8);
  my_hline->setPen(QPen(RANDHCOLOR,1));
  my_hline->show();

  
  my_chips->my_stackarray=new QCanvasPixmapArray(STACKPICS,NUMSTACKS);
  my_chips->my_pixarray  =new QCanvasPixmapArray(SPRITEPICS,NUMSPRITES);  // normal card images
  my_chips->my_hipixarray=new QCanvasPixmapArray(SPRITEPICSA,NUMSPRITES); // hilited
  my_chips->my_cardbgarray=new QCanvasPixmapArray(CARDBG,NUMBG);               // back side of decks
   
  QString err="";
  if (!my_chips->my_pixarray->isValid())   err=err+"Some cards are missing!\n";
  if (!my_chips->my_hipixarray->isValid()) err=err+"Some hi-cards are missing!\n";
  if (!my_chips->my_stackarray->isValid()) err=err+"Some stacks are missing!\n";
  if (!my_chips->my_cardbgarray->isValid()) err=err+"Some back sides oft the cards are missing!\n";

  if (err=="") 
  {  
    int i,x,y;
    for (i=0; i<NUMSPRITES; i++) my_chips->my_pixarray->setImage(i+NUMSPRITES,my_chips->my_hipixarray->image(i));
    for (i=0; i<2; i++) my_chips->my_pixarray->setImage(i+2*NUMSPRITES,my_chips->my_cardbgarray->image(i)); 

    for (i=0; i<SMAX; i++)
    { x=i*DX+OFXA; y=OFYA; 
      my_chips->p_stack[i]=new QCanvasSprite(my_chips->my_stackarray,my_canvas);
      my_chips->p_stack[i]->move(x,y,i%4);
      my_chips->p_stack[i]->setZ(0);
      my_chips->p_stack[i]->setVisible(true);
    }
    for (i=0; i<RMAX; i++)
    { x=i*DX+OFX; y=OFY; 
      my_chips->p_reihe[i]=new QCanvasSprite(my_chips->my_stackarray,my_canvas);
      my_chips->p_reihe[i]->move(x,y,4);
      my_chips->p_reihe[i]->setZ(0);
      my_chips->p_reihe[i]->setVisible(true);
    }
    
    for (i=0; i<2*NUMSPRITES; i++)        
    { x=(i%RMAX)*DX+OFX; y=(i/RMAX)*DY+OFY; 
      my_chips->my_card[i]->p_sprite=new QCanvasSprite(my_chips->my_pixarray,my_canvas);
      my_chips->my_card[i]->p_sprite->move(x,y,i%NUMSPRITES);
      my_chips->my_card[i]->p_sprite->setZ(i+1);
      my_chips->my_card[i]->p_sprite->setVisible(true);
    }
    my_chips->neues_spiel();
    my_view->polish();
    my_view->show();
  }	    
  else printf(err);



// ---------------------------------------------------------------------------------------------------------------------
  my_app.setMainWidget(my_window);        // my_window als Hauptfenster  

  my_app.connect(my_window->p_austeilen,SIGNAL(clicked()),my_chips,SLOT(reihe_austeilen())); 
  my_app.connect(my_window->p_abraeumen,SIGNAL(clicked()),my_chips,SLOT(autoplay())); 
  my_app.connect(my_window->p_neues_spiel,SIGNAL(clicked()),my_chips,SLOT(neues_spiel()));

  QPixmap my_pixmap=QPixmap(PROGICON); 
  my_window->setIcon(my_pixmap);
  my_window->show();                      // anzeigen
  printf("Breite x Hoehe %i x %i\n",my_window->width(),my_window->height());
  return my_app.exec();                   // Exec-Schleife der my_app
}
