/****************************************************************************************
    Copyright 2004 uja
    Copyright 2004 Trolltech (Qt3)

    This file is part of Fleur: game header
*****************************************************************************************/

// Game-Headerdatei Fleur

#ifndef CHIPS_H
#define CHIPS_H

#include "config.h"
#include <qcanvas.h>
#include <qtimer.h>
#include <qapplication.h>

struct wahl { bool im_stack; int  posnr, posi; };

// -------------------------------------------

class karte
{
  public:
  karte(int,int,int,bool); // Farbe,Bild,Deck
  QCanvasSprite* p_sprite;
  
  void move_card(int,int,int);
  void open_card(bool);
  void mark_card();
  void norm_card();
  
  bool is_open();
  int get_farbe();
  int get_bild();
  void abmarsch();
  
  private:
  int x,y,z,bild,farbe,deck,dpix,code;
  int vx,vy;
  bool offen;
};



class chips:public QCanvas
{
  Q_OBJECT
  
  public:
  chips(QWidget* parent=0);
  QCanvasPixmapArray* my_pixarray;
  QCanvasPixmapArray* my_hipixarray;
  QCanvasPixmapArray* my_cardbgarray;
  QCanvasPixmapArray* my_stackarray;
  karte* my_card[2*NUMSPRITES];
  QCanvasSprite* p_stack[SMAX];
  QCanvasSprite* p_reihe[RMAX];
  
  private:
  bool aktiv,ani1,ani2;
  int anim,tic,t_ind,r_ind,talon[2*NUMSPRITES+1];
  int s_liste[SMAX][14];              // Ablagen
  int r_liste[RMAX][2*NUMSPRITES+1];  // Spalten
  wahl akt_zug;
  QTimer* stopper;

  bool passt(karte*,karte*,bool,bool,bool); // Regelwerk Anlegen
  int  oberste_freie(int);        // oberste freie Karte in Reihen suchen
  int  markiere_reihe(int,int);   // Reihe ab gewuenschter oder letzter freier Karte markieren 
  void loesche_marken();
  void letzte_oeffnen(int);
  void zeige_reihe(int);          // Reihe bereinigt anzeigen
  void talon2reihe(int,bool);     // Karte aus Talon an Reihe anlegen 
  bool reihe2stack(int,int);      // Karte von Reihen auf Ablagen
  bool stack2reihe(int,int);      // Karte von Ablage auf Reihe
  bool stack2stack(int,int);      // Karte von Ablage auf Ablage, passiert halt beim Rumklicken
  bool reihe2reihe(int,int);      // Stapel innerhalb Reihen verschieben
  bool is_ende();

  private slots: // Animationen
  void cycle();
  void enderoutine();
  
  public slots:
  void neues_spiel();
  void spielstart();
  void klick(int,int);
  void reihe_austeilen();
  void autoplay();  

  signals:
  void send_messi(QString);

};

#endif
