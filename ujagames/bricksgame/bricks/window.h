/****************************************************************************************
    Copyright 2004 uja
    Copyright 2004 Trolltech (Qt3)

    This file is part of Fleur which runs under GPL
*****************************************************************************************/

// Headerfile fuer Objekt window: window.h
// erstellt ein zweigeteiltes Fenster, links Grafikausgabe, rechts Eingabeparameter und Anzeigen
// Klamotten wie Help, Neu  Load und Save werden von QApplication in main.cpp erledigt
// ----------------------------------------------------------------------------------------------

#ifndef WINDOW_H
#define WINDOW_H
#include "lang.h"
#include "config.h"
#include "gamelib.h"
#include <qvbox.h>        // Anzeige-Unterteilung
#include <qhbox.h>        // Basis dieses Windows
#include <qgrid.h>        // Unterteilungsgitter
#include <qlabel.h>       // Anzeigen 
#include <qcanvas.h>      // Spielwiese

class window:public QHBox
{
  Q_OBJECT

  public:
  window::window(QWidget* parent=0, char* name=0);
  int keyup,keydn;
  QLabel*   p_messi;
  QLabel*   p_level;
  QLabel*   p_rest;
  QLabel*   p_zeit;
  QLabel*   p_punkte;
  QLabel*   p_ball[MAXBAELLE];
  
  QPushButton* p_neu;
  QPushButton* p_pause;  
  QPushButton* p_tilt;

  viewport* my_view;
    
  public slots:
  void set_punkte(int);
  void set_level(QString);
  void set_leben(int);
  void set_messi(QString);
  void set_rest(int);
  void keyPressEvent(QKeyEvent* e);
  void keyReleaseEvent(QKeyEvent* e);
};

#endif 
