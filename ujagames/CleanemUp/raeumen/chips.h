/****************************************************************************************
    Copyright 2004 uja
    Copyright 2004 Trolltech (Qt3)

    This file is part of Auf raeumen.
*****************************************************************************************/

#ifndef CHIPS_H
#define CHIPS_H

#include "config.h"
#include <qcanvas.h>
#include <qtimer.h>
#include <qfile.h>

class chips:public QCanvas
{
  Q_OBJECT
  
  public:
  chips(QWidget* parent=0);
  QCanvasPixmapArray* my_pixarray;
  QCanvasPixmapArray* my_goverarray;
  QCanvasPixmapArray* my_pausearray;
  QCanvasSprite* p_sprite[SMAX]; // anders ging es leider nicht
  QCanvasSprite* p_pause;
  QCanvasSprite* p_gover;
  
  bool aktiv,pausiert;
  int punkte,level,zeit,tic,nfarben,ofs_px,oldpos;

  int f[SMAX],liste[SMAX];
  QTimer* my_timer;        // Stoppuhr
  QTimer* my_stopper;      // Animation  

  private:
  void reset_timer();
  void zeige_feld(int);
  void updat_level();
  void get_kette(int);
  void get_nachbarn(int,int);
  void entferne_kette();
  void updat_punkte(int);
  void weiter();

  bool check_zug(int);
  int  zug_moeglich();

  public slots:
  void neues_spiel();
  void klick(int,int);
  void show_kette(int,int);
  void set_modus(int);
  void set_chipsatz(int);
  void pause();
  void save_settings(int);
  
  
  
  private slots:
  void ticken();
  void droppen();
  void nach_links();
  void game_over();

  signals:
  void punkte_changed(int);
  void zeit_changed(int);
  void level_changed(int);
  void gameover(int); // Punkte weiterreichen und bei Bedarf HSC-Eintrag oder Gameover-Bild (Sprite?)
  void send_messi(QString);
};

#endif
