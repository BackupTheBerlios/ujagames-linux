// bricks header - uja
// -----------------------------------------

#ifndef GAME_H
#define GAME_H

#include <qfile.h>
#include <qcanvas.h>
#include <qtimer.h>
#include "levels.h"
#include "config.h"
#include "lang.h"
#include "window.h"

class game;

class flieger
{
  public:
  QCanvasSprite* p_sprite;

  friend class game;
  int id,phase,breite,hoehe;
  double x,y,z,vx,vy;

  flieger(int);  
  void set_koords(double,double);
  void set_koords(int,int);
  void set_tiefe(double);
  void set_tiefe(int);

  void set_phase(int);
  void move_it();
  void set_vx(double);
  void set_vy(double);
  void set_speed(double,double);
  void sprite_off();
  void sprite_on();
} ;


class game:public window
{
  Q_OBJECT

  private:
  static const int startbaelle=4,xmax=XMAX*DX,ymax=(YMAX+5)*DY; // Spielfeldgrenzen
  static const int tic_lo=30,tic_no=20,tic_hi=10;

  bool aktiv,pausiert,sound_on,follow_paddel;
  int level_id,aktlevel,punkte,punkte0,vorrat,vorrat0,tileset,bgbild,paddelball,reststeine;
  int f[SMAX];
  double paddeldelta;
  level* my_levelliste;         // Gelaendetypenvorrat
  window* oberer;
  QTimer* timrec1;
  QTimer* timrec2;

  void grab_focus();
  void keyhandle(int);
  void init_ball();
  void ball_weg(int);
  void goodie_auf_paddel(int);
  void starte_ball_von_paddel(int);
  void level_up();
  bool check_levelende(); 
  void starte_goodie(double,double);

  public:
  game(window* parent,char* name=0);

  static const int NUM_LASERS=64;
  static const int NUM_BALLS=3;
  static const int NUM_GOODIES=8;
  static const int NUM_BLOCKS=SMAX;
  static const int NUM_FLOATERS=3;

  QCanvasPixmapArray* ballarray;    // max 3 fliegende Baelle
  QCanvasPixmapArray* paddelarray;  // normal,breit,schmal,kleber,shooter
  QCanvasPixmapArray* blockarray;   // normal:0-7, doppler,dreier,vierer, fest,explo 
  QCanvasPixmapArray* blockarray0;   // fest:0, normal:1-7, doppler,dreier,vierer, normal 
  QCanvasPixmapArray* blockarray1;   
  QCanvasPixmapArray* blockarray2;   
  QCanvasPixmapArray* blockarray3;   
  QCanvasPixmapArray* goodiearray;  // +50,-50,dreiball,levelup,+ball,-ball,normalp,breitp,schmalp,kleberp,shooterp,speed0,speedp,speedm
  QCanvasPixmapArray* floaterarray; // 8Phasen sich langsam drehende Sprites
  QCanvasPixmapArray* laserarray;   // Laserschuesse: 16 oder mehr?

  QCanvasPixmapArray* pausearray;
  QCanvasPixmapArray* goverarray;
  QCanvasPixmapArray* tuerarray;
  
  QCanvasSprite* p_gover;
  QCanvasSprite* p_pause;
  QCanvasSprite* tuer;
  QCanvasSprite* block[NUM_BLOCKS];

  flieger* paddel;
  flieger* ball[NUM_BALLS];
  flieger* floater[NUM_FLOATERS];
  flieger* goodie[NUM_GOODIES];
  flieger* laser[NUM_LASERS];

  public slots:
  void neues_spiel();
  void starte_level(int);
  void hover(int,int);
  void geklickt(int,int);
  void game_over();
  void pause();
  void toggle_pause();
  void weiter();
  void tilten();
  void restart(); 

  void lade_spielstand(int);
  void speichere_spielstand(int);
  void set_background(int); // Spielhintergrund
  void set_tileset(int);
  void set_sound(int);
  void save_settings();
  
  private slots:
  void move_all();
  void poll_it();	
  void unfreeze_paddel();
  void normal_speed();

  signals:
  void punkte_changed(int);
  void levelname_changed(QString);
  void rest_changed(int);
  void gameover(int);
  void messi(QString);
  void levelfile0_changed(bool);
  void levelfile1_changed(bool);
  void sound_setting0(bool);
  void sound_setting1(bool);
  void vorrat_changed(int);
  void play_lala(bool,int);
} ;


#endif
