// bricks header - uja
// -----------------------------------------

#ifndef GAME_H
#define GAME_H

#include <qfile.h>
#include <qcanvas.h>
#include "config.h"
#include "lang.h"

class game:public QWidget
{
  Q_OBJECT
  private:
  bool gameover,geschafft;
  int gewaehlt,lauf,punkte,summe,nfarben,ntoepfe,runde,maxrunde,farben_ok,toepfe_ok;
  int c[XMAX],e[XMAX],g[XMAX],h[XMAX];   // Code, Eingabe, Haeufigkeit Farbe in Code, in Eingabe
  // V.1.1:
  int farbset,bgset;

  void bilanzen();

  public:
  game(QWidget* parent,char* name=0);
  QWidget* oberer;
  QCanvasPixmapArray* bgarray;
  QCanvasPixmapArray* chiparray;
  QCanvasPixmapArray* markerarray;
  QCanvasSprite* p_cursor;
  QCanvasSprite* p_bgsprite[SMAX];
  QCanvasSprite* p_stein[SMAX];
  QCanvasSprite* p_marker[SMAX];

  public slots:
  void neues_spiel();
  void reset_stat();
  void geklickt(int,int);
  void hover(int,int);
  void set_farben(int);
  void set_toepfe(int);
  void set_lauf(int);
  void set_summe(int);
  void save_settings();
  void auswerten();
  void stein_gewaehlt(int);
  // V. 1.1:
  void farbset_changed(int);
  void bgset_changed(int);

  signals:
  void punkte_changed(int);
  void messi(QString);
  void lauf_changed(int);
  void summe_changed(int);
  void updat_farben(int);
  void ratio_changed(int,int);
  void loesung(QString);
  void loe_loeschen();
  
} ;

#endif
