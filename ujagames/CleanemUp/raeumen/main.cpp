/// ==================================================================================================================
// ujagames_qt: Hauptmodul-Muster des Typs KlickiBunti - (c) uja 09/04
// Programm Raeumen

// Aufbau Menu, Canvas(se), Initialisierung aller Objekte, soweit nicht im Objekt selbst moeglich

// es gibt insgesamt 4 Menus,
// Menu 1: Neues_Spiel, Level von vorne, Bestenliste ansehen, Bestenliste zuruecksetzen, Quit
// Menu 2: Spieleoptionen, Netzwerkhiscore-Schalter, Optionen speichern - wird von gameparm.ini initialisiert
// Menu 3: Spielstandsspeicher
// Menu 4: Infos
// Individualisierung geschieht durch config.h, nicht benoetigte Teile sind auskommentiert.

// Check und Einrichten der Sprites im entsprechendem Canvas, ausser den Spielsteinen und Marken
// sind dies das Pause- und Gameoverbild 
// Initialisierung der Spielparameter aus gameparm.ini und Start der 1.Runde
// -----------------------------------------------------------------------------------------------------------------

// 07.09.2004 uja: Zusammenfassung aller bisherigen main.cpp zu diesem Modul des Typs KlickiBunti
 
// =================================================================================================================

#include "config.h"         // ein paar Konstanten
#include "gamelib.h"    // konstante Module
#include "window.h"     // Hauptfenster Darstellung
#include "chips.h"      // Spielwiese
#include <qapplication.h>      // Fensterhandling-Event-Routinen
#include <qmenubar.h>          // Menuleiste
#include <qmenudata.h>         // Menu-Items
#include <qaccel.h>            // Shortcuts
#include <qradiobutton.h>      // Auswahl
#include <qbuttongroup.h>      // Auswahl
#include <qfile.h>             // INI-Datei auswerten
#include <stdio.h>             // debugging
// -------------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{ 
  QApplication my_app(argc,argv);                  // die Fenster/Appli-Steuerung
  window*      my_window    =new window();         // eine Instanz des zu zeigenden Spiel-Hauptfensters
  hiscore*     my_hiscore   =new hiscore();        // Bestenlisten-Hauptfenster
  textviewer*  my_textviewer=new textviewer();     // Fenster fuer Hilfstexte
  chips*       my_chips     =new chips(my_window); // spezifisch fuer Spiel: Chips oder  Wiese und Bewohner
  viewport*    my_view      =my_window->my_view;   // Darstellungsbereich des Spiels
//  viewport*    my_view2     =my_window->my_view2;  //  2. View fuer Vorschau - faellt hier flach
     
  int i,x=0,y=0,z=0;                               // ini-File Parameter
  QString buff,b0;
  
// ini-File auswerten: Spielmodus, Farben (falls vorhanden) , Netzwerkoption
// Daten abladen in x,y,z
// -----------------------------------------------------------------------------------
  QFile f0(INIFILE);
  buff=""; 
  if (f0.open(IO_ReadOnly)) // hier: 2-zeilig, 3 Parameter
  { 
    f0.readLine(buff,96); if (buff!="") { b0=buff.section(';',0,0); z=b0.toInt(); }
    f0.readLine(buff,96); if (buff!="") { b0=buff.section(';',0,0); x=b0.toInt(); b0=buff.section(';',1,1); y=b0.toInt(); }
    f0.close();
  }  
    
      
// allgemeines Spielemenu: 
// ---------------------------------------------------------------------------------------      
  QMenuBar*   my_menubar=new QMenuBar(my_window);  // Menubar mit Window verbinden    
 
// Menu 1:  neues Spiel, Pause, Bestenliste, Reset, Quit 

  QPopupMenu* my_menu1 = new QPopupMenu; 
  my_menubar->insertItem(_MENU1,my_menu1);   
  my_menu1->insertItem(_NEWGAME,my_chips,SLOT(neues_spiel()),QKeySequence("CTRL+N"));
//  my_menu1->insertItem(_RESTARTLEVEL,my_chips,SLOT(restart_level()),QKeySequence("CTRL+L"));
  my_menu1->insertItem(_PAUSE,my_chips,SLOT(pause()),QKeySequence("CTRL+P"));
  my_menu1->insertItem(_HISCORE,my_hiscore,SLOT(show_hsc()),QKeySequence("CTRL+B"),14);
  my_menu1->insertSeparator();
  my_menu1->insertItem(_HISCORERESET,my_hiscore,SLOT(reset_hsc())); 
  my_menu1->insertSeparator();
  my_menu1->insertItem(_QUIT,&my_app,SLOT(quit()),QKeySequence("CTRL+Q"));  // slot quit wird eingeschleppt 
  

// Menu 2: Netzwerkmodus, Parameter speichern
// Menu 2: Schwierigkeitsgrad, Steinmuster, falls mehrere vorhanden, Netzwerkmodus, Parameter speichern
  
  QPopupMenu* my_menu2 = new QPopupMenu;
  QButtonGroup* my_wig21=new QButtonGroup(1,Qt::Vertical,_NUMPATT);
  QRadioButton* wig210=new QRadioButton("3",my_wig21);
  QRadioButton* wig211=new QRadioButton("4",my_wig21);
  QRadioButton* wig212=new QRadioButton("5",my_wig21); 
  my_wig21->setExclusive(true);
  my_wig21->setButton(x);
  
  QButtonGroup* my_wig22=new QButtonGroup(1,Qt::Vertical,_PATTNUM);
  QRadioButton* wig220=new QRadioButton(_PATT0,my_wig22);
  QRadioButton* wig221=new QRadioButton(_PATT1,my_wig22);
  QRadioButton* wig222=new QRadioButton(_PATT2,my_wig22); 
  my_wig22->setExclusive(true);
  my_wig22->setButton(y);
  
  QButtonGroup* my_wig23=new QButtonGroup(1,Qt::Vertical,_HISCORE,0);
  QRadioButton* wig230=new QRadioButton(_LOCAL,my_wig23);
  QRadioButton* wig231=new QRadioButton(_NET,my_wig23);
  my_wig23->setExclusive(true);
  my_wig23->setButton(z);
     
  my_menubar->insertItem(_MENU2,my_menu2);
  my_menu2->insertItem(my_wig21,21); my_app.connect(my_wig21,SIGNAL(clicked(int)),my_chips,SLOT(set_modus(int)));
  my_menu2->insertItem(my_wig22,22); my_app.connect(my_wig22,SIGNAL(clicked(int)),my_chips,SLOT(set_chipsatz(int)));
  my_menu2->insertItem(my_wig23,23); my_app.connect(my_wig23,SIGNAL(clicked(int)),my_hiscore,SLOT(set_hscpfad(int)));
  my_menu2->insertItem(_SAVEPARMS,my_hiscore,SLOT(save_settings()));


  // CPP-Patch: irgendein Quatsch, damit der Compiler die Schnauze haelt
  bool cpppatsch=((wig210) && (wig211) && (wig212) && (wig220) && (wig221) && (wig222));
  cpppatsch=(cpppatsch && (wig230) && (wig231));
  // Ende CPP-Patch ----------------------------------------------------


/* Menu 3: Speicher für Zwischenstaende: Muster: die 7 Schwestern - faellt hier flach 
  
  QPopupMenu* my_menu3=new QPopupMenu;
  QPopupMenu* my_pop31=new QPopupMenu; // Submenu speichern
  QPopupMenu* my_pop32=new QPopupMenu; // Submenu laden
  
  my_menubar->insertItem(_MENU3,my_menu3);
  my_menu3->insertItem(_LOADLEVEL,my_pop31);
  my_menu3->insertItem(_SAVELEVEL,my_pop32);
  
  // Spielstandspeicher einlesen und in Submenus einhaengen:
  // Deren Namen stehen in der 1.Zeile von SPEICHER, die als CSV angelegt ist, (danach zeilenweise Level;Punkte;)  
  QFile f1(QString(SPEICHER));
  if (f1.open(IO_ReadOnly)) { f1.readLine(buff,96); f1.close(); }
  for (int i=0; i<7; i++) 
  { 
    my_pop31->insertItem("<-   "+buff.section(';',i,i),my_chips,SLOT(lade_spielstand(int)),0,310+i);
    my_pop32->insertItem("->   "+buff.section(';',i,i),my_chips,SLOT(speichere_spielstand(int)),0,320+i);
    my_menubar->setItemParameter(310+i,i);
    my_menubar->setItemParameter(320+i,i);
  }
*/

// Menu 4: Spielanleitung, Info, Lizenz    
  QPopupMenu* my_menu4 = new QPopupMenu;
  my_menubar->insertItem(_MENU4,my_menu4);
  my_menu4->insertItem(_HELP,my_textviewer,SLOT(show_info(int)),QKeySequence("CTRL+H"),41);
  my_menu4->insertItem(_INFO,my_textviewer,SLOT(show_info(int)),QKeySequence("CTRL+I"),42); 
  my_menu4->insertItem(_LICENSE,my_textviewer,SLOT(show_info(int)),QKeySequence("CTRL+G"),43); 
  my_menubar->setItemParameter(42,0);
  my_menubar->setItemParameter(41,1);
  my_menubar->setItemParameter(43,2);

// restliche Kommunikationswege festlegen:
// ----------------------------------------
  my_app.connect(my_view,   SIGNAL(geklickt(int,int)),my_chips,  SLOT(klick(int,int)));      // Klick ins Spielfeld, x,y 
  my_app.connect(my_view,   SIGNAL(hover(int,int)),   my_chips,  SLOT(show_kette(int,int))); // Mausmove im Spielfeld, x,y 
  my_app.connect(my_chips,  SIGNAL(gameover(int)),    my_hiscore,SLOT(save_hsc(int)));       // Auswertung Spielende: Punkte
  my_app.connect(my_hiscore,SIGNAL(rewrite_ini(int)), my_chips,  SLOT(save_settings(int)));  // ini-Update Teil 2
      
// --- Ende INI-Files und Menus ---
// ===========================================================================================================================



// Canvas fuer Sprites erzeugen, mit (maussensitivem) Viewport verbinden,
// der Canvas steht um OFX bzw. OFY an jeder Seite ueber, da es sonst manchmal bunte Randstreifen gibt
// --------------------------------------------------------------------------------------------------------------------

// Chip-Sprites belegen und mit Canvas verbinden:

  QCanvas* my_canvas=new QCanvas(2*OFX+XMAX*DX,2*OFY+YMAX*DY);
  my_view->setCanvas(my_canvas);   
  my_canvas->setBackgroundColor(BACKGR1);
  my_canvas->setDoubleBuffering(true);
  my_canvas->setAdvancePeriod(10);        // soviel Luxus muss sein :-)
   
// Sprites belegen:
  my_chips->my_pixarray  =new QCanvasPixmapArray(SPRITEPICS,NUMSPRITES);
  my_chips->my_goverarray=new QCanvasPixmapArray(GAMEOVERPIC);
  my_chips->my_pausearray=new QCanvasPixmapArray(PAUSEPIC);
// Check auf Vollstaendigkeit der Bilder:
  QString err="";
  if (!my_chips->my_pixarray->isValid())   err=err+"Some stone images are missing!\n";
  if (!my_chips->my_goverarray->isValid()) err=err+"Gameover screen is missing!\n";
  if (!my_chips->my_pausearray->isValid()) err=err+"Pause screen is missing!\n";
  
  if (err=="")  // Spritebelegung und Initialisierung:
  {  
    int x,y;
    my_chips->p_gover=new QCanvasSprite(my_chips->my_goverarray,my_canvas);
    my_chips->p_gover->setZ(99); my_chips->p_gover->move(DX+OFX,2*DY+OFY);  // gameover oberste Prioritaet
    my_chips->p_pause=new QCanvasSprite(my_chips->my_pausearray,my_canvas);
    my_chips->p_pause->setZ(98); my_chips->p_pause->move(DX+OFX,2*DY+OFY);  // pause  naechstoberste Prioritaet
    
    for (i=0; i<SMAX; i++)
    { x=i%XMAX; y=(i-x)/XMAX;
      x=x*DX+OFX; y=y*DY+OFY; 
      my_chips->p_sprite[i]=new QCanvasSprite(my_chips->my_pixarray,my_canvas);
      my_chips->p_sprite[i]->move(x,y,i%NUMSPRITES);
      my_chips->p_sprite[i]->setZ(2);
      my_chips->p_sprite[i]->show();
    }     
   // --- Ende Spritegenerierung -------------------------------------------

    my_view->setContentsPos(OFX,OFY);
    my_view->polish();
    my_view->show();
  }	    
  else printf(err); // Bilder fehlen
  
// ---------------------------------------------------------------------------------------------------------------------

  my_app.setMainWidget(my_window);                                                   // my_window als Hauptfenster  
 
  // Initialisierungen und Spielstart:
  my_chips->set_modus(x);
  my_chips->set_chipsatz(y);
  my_hiscore->set_hscpfad(z);
  
  QPixmap my_pixmap=QPixmap(PROGICON);                                            // Programmicon setzen  
  my_window->setIcon(my_pixmap);
  my_window->show(); 
//  printf("Breite x Hoehe %i x %i\n",my_window->width(),my_window->height());    // Debug-Info
  return my_app.exec();                   // Exec-Schleife der my_app: Hock di hi und warte auf meine Kommandos!
}
