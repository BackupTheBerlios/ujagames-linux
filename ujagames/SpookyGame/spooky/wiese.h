#ifndef WIESE_H

#define WIESE_H

#include "config.h"
#include <qcanvas.h>
#include <qtimer.h>

// und da die Qt-internen Spritemoveroutinen nur Ruckel und Zuckelgrafik bei langsameren Objekten erzeugen, 
// muss alles, was sich bewegt, wieder auf die konventionellen Methoden umgeschrieben werden:


class flieger
{ 
  public:
  flieger(int);
  int id,bildbase,basewav,phase,count;
  double x,y,z,vx,vy;
  QCanvasSprite* p_sprite;
   
  void set_koords(double,double);
  void set_tiefe(double);
  void move_it();
};




class wiese:public QCanvas
{
  Q_OBJECT
 
  public:
  wiese(QWidget* parent);
  QTimer* timrec1;
  QTimer* timrec2;
  
  bool sound_on;
  QWidget* oberer;
  
  QCanvasPixmapArray* my_mondarray;
  QCanvasPixmapArray* my_katzenarray;
  QCanvasPixmapArray* my_spinnenarray;
  QCanvasPixmapArray* my_topfarray;
  QCanvasPixmapArray* my_waberarray;
  QCanvasPixmapArray* my_clownarray;
  QCanvasPixmapArray* my_kuerbisarray;
  QCanvasPixmapArray* my_batarray;
  QCanvasPixmapArray* my_hexenarray;
  QCanvasPixmapArray* my_geistarray;
  
  QCanvasPixmapArray* my_baumarray;
  QCanvasPixmapArray* my_schlossarray;
  QCanvasPixmapArray* my_tuerarray;
  QCanvasPixmapArray* my_shooterarray;
  QCanvasPixmapArray* my_titelarray;  
  QCanvasPixmapArray* my_pausearray;
  QCanvasPixmapArray* my_goverarray;
  QCanvasPixmapArray* my_vgarray;
  QCanvasPixmapArray* my_hotspotarray;
  QCanvasPixmapArray* my_punktearray;
  
  // Erweiterung Bild 2:
   QCanvasPixmapArray* my_buscharray;
   QCanvasPixmapArray* my_mauerarray;
   QCanvasPixmapArray* my_floharray;
   QCanvasPixmapArray* my_skorparray;
   QCanvasPixmapArray* my_pilzarray;
   
   
   // Erweiterung Bild 3:
   QCanvasPixmapArray* my_scheunenarray;
   QCanvasPixmapArray* my_monsterarray;
   
  
   
  flieger* mond;        // E0, li->re, 3 Durchgänge, dann gameover
  flieger* spinne;      // E31, rauf/runter
  flieger* katze;       // E11, E21 statisch an versch. Stellen
  flieger* waber;       // Sequenz: Waber 0-3 oszillierend, 4: aufsteigend 
  flieger* clown;       // 2 versch. Plätze, oszillierend
  flieger* floh;        // re/li, dabei sin*sin direkt hinter 1.Mauer, schaltet einen skorpion an - E30.2
  flieger* pilz;        // nur an/aus, vgl katze, ziemlich hinten, im Wechsel mit Floh - E10.2
  flieger* monster;
  
  
  // E2-9 E12-19 E22-29:
  flieger* kuerbis[4];  // rollen re/li
  flieger* bat[8];      // random, nix weiter
  flieger* hexe[4];     // re/li
  flieger* geist[8];    // Welle?
  flieger* skorpion[3]; // re/li im Garten, Spur 0 am Teich (19.2), Spur 1 auf Mauer (30.8) re, sp, spur 2 hinter Tor 29.8

  
  QCanvasSprite* titel;          // E1
  QCanvasSprite* p_vordergrund;  // E32
  QCanvasSprite* p_pause;        // E34
  QCanvasSprite* p_gover;        // E35
  QCanvasSprite* schloss[4];     // E10, E20
  QCanvasSprite* tuer;           // E21
  QCanvasSprite* baum[5];        // E10, E30
  QCanvasSprite* topf;          // E1
  
  QCanvasSprite* shooter;        // E33
  QCanvasSprite* hotspot;        // E33
  
  flieger* psprite[40]; // pro id 1 Punktesprit
  
  // Erweiterung Level 2::
  QCanvasSprite* mauer;    // E30.5
  QCanvasSprite* busch[4]; // E30,E20,E20,E10
  
  // Erweiterung Level 3:
  QCanvasSprite* scheune[3]; // E15.5,E20,E30.7
  
  
  
  
  
  private:
  bool aktiv,pausiert;
  int tic,punkte,vorrat;
  
  int freie_bahn();
  int flughoehe(int);
  int freier_kuerbis();
  int freie_hexe();
  int freier_bat();
  int freier_geist();
  
  void starte_spinne();
  void starte_katze();
  void starte_waber();
  void starte_clown();
  void starte_kuerbis();
  void starte_hexe();
  void starte_bat();
  void starte_geist();
  void starte_floh();
  void starte_pilz();
  void starte_skorpion();
  void starte_monster();
  
  void mond_getroffen();
  void spinne_getroffen();
  void katze_getroffen();
  void pott_getroffen();
  void ghoul_getroffen();
  void clown_getroffen();
  void kuerbis_getroffen(int);
  void hexe_getroffen(int);
  void bat_getroffen(int);
  void geist_getroffen(int);
  void floh_getroffen();
  void pilz_getroffen();
  void skorpion_getroffen(int);
  void monster_getroffen();
  
  void zeige_punkte(flieger*,int);
  
  void lade_sounds();
  void mach_lala(int);
  
  private slots:
  // timergesteuert:
  void game_over();
  void move_mond();
  void move_all();
 
  void move_spinne();
  void move_katze();
  void move_waber();
  void move_clown();
  void move_floh();
  
  void move_kuerbis(int);
  void move_hexe(int);
  void move_bat(int);
  void move_geist(int);
  void move_skorpion(int);
  
  void move_monster();
  
   
  public slots:
  void neues_spiel();
  void nachladen();
  void klick(int,int);
  void pause();
  void weiter();
  void hover(int,int);
  void set_sound(int);
  void keyhandle(int);
  void shooterupdate(int);
  
  signals:
  void punkte_changed(int);
  void bullets_changed(int);
  void gameover(int);
  void sound_setting0(bool);
  void sound_setting1(bool);
  void zeige_view(int,int);
  void focus2playfield();
  void scrollit(int);
  
  void play_lala(bool,int);
} ;


#endif
