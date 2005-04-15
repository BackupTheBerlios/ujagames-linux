/****************************************************************************************
    Copyright 2004 uja
    Copyright 2004 Trolltech (Qt3)
    This file is part of Fleur which runs under GPL
*****************************************************************************************/

// Fenster des Musters: Anzeige oben, Spielfeld unten 
// --------------------------------------------------------------------------------------

#ifndef WINDOW_H
#define WINDOW_H

#include "config.h"
#include "gamelib.h"

#include <qvbox.h>        // Anzeige-Unterteilung
#include <qhbox.h>        // Basis dieses Windows
#include <qlabel.h>       // Anzeigen 
#include <qapplication.h> // OBJECT
#include <qpushbutton.h>  // Neu-Button

class window:public QVBox
{
  Q_OBJECT

  public:
  window::window(QWidget* parent=0, char* name=0);
  QPushButton* p_austeilen;
  QPushButton* p_abraeumen;
  QPushButton* p_neues_spiel;
  QLabel*   p_messi;    
  viewport* my_view;

  public slots:
  void set_messi(QString);

};

#endif 
