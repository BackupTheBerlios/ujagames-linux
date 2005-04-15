/****************************************************************************************
    Copyright 2004 uja
    Copyright 2004 Trolltech (Qt3)

    This file is part of master which runs under GPL
*****************************************************************************************/

#ifndef WINDOW_H
#define WINDOW_H
#include "lang.h"
#include "config.h"
#include "gamelib.h"
#include <qvbox.h>        // Anzeige-Unterteilung
#include <qhbox.h>        // Basis dieses Windows
#include <qgrid.h>        // Unterteilungsgitter
#include <qlabel.h>       // Anzeigen 
#include <qbuttongroup.h>
#include <qmessagebox.h>

class window:public QHBox
{
  Q_OBJECT

// V.1.1:
  private:
  int fset,bset,nfarben;
  
  QHBox* centerwig;
  QVBox* spalte2;

  QLabel* p_ue;
  QLabel*   p_messi;
  QLabel*   p_numgames;
  QLabel*   p_sumpunkte;
  QLabel*   p_ratio;
  QLabel*   p_punkte;
  QLabel* p_code[XMAX];

  QGrid* p_codes;
  QGrid* p_anzeigen;
  QGrid* p_steuer;

  void polish(QWidget*);

  public:
  window::window(QWidget* parent=0, char* name=0);

  QButtonGroup* p_auswahl;
  QPushButton* p_bchip[XMAX];
  QPushButton* p_auswerten;
  QPushButton* p_neue_runde;

  viewport* my_view;
    
  public slots:
  void set_punkte(int);
  void set_messi(QString);
  void set_numgames(int);
  void set_numfarben(int);
  void set_sumpunkte(int);
  void set_ratio(int,int);
  // V.1.1:
  void show_code(QString);
  void loesche_code();
  void farbset(int);
  void bgset(int);
  void about();

  signals:
  void auswerten();
  void neue_runde();
  // V.1.1:
  void farbset_changed(int);
  void bgset_changed(int);

};

#endif 
