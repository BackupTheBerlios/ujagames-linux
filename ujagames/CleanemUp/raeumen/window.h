/****************************************************************************************
    Copyright 2004 uja
    Copyright 2004 Trolltech (Qt3)

    This file is part of ujagames_qt which run under GPL
*****************************************************************************************/

#ifndef WINDOW_H
#define WINDOW_H

#include "config.h"
#include "gamelib.h"

#include <qvbox.h>        // Anzeige-Unterteilung
#include <qhbox.h>        // Basis dieses Windows
#include <qgrid.h>        // Unterteilungsgitter
#include <qlabel.h>       // Anzeigen 
#include <qcanvas.h>      // Spielwiese

class window:public QVBox
{
  Q_OBJECT

  public:
  window::window(QWidget* parent=0, char* name=0);
  viewport* my_view;

  private:
  QLabel* p_punkte;
  QLabel* p_level;
  QLabel* p_zeit;
  QLabel* p_messi;
  
  public slots:
  void set_punkte(int);
  void set_zeit(int);
  void set_level(int);
  void set_messi(QString);
};

#endif 
