/****************************************************************************************
    Copyright 2004 uja
    Copyright 2004 Trolltech (Qt3)

    This file is part of ujagames_qt  which runs under GPL.
*****************************************************************************************/

#include "window.h"

// einspaltiges  Layout fuer langgestreckte Spielfelder -  Titel oben links, Anzeigen oben rechts
// ----------------------------------------------------------------------------------------------

window::window(QWidget *parent,char* name):QVBox(parent,name)
{ 
  // allgemeines Aussehen:
  this->setMargin(16);
  this->setSpacing(4);
  this->setPaletteBackgroundColor(BACKGR0);
  this->setBackgroundPixmap(QPixmap(BACKPIX0));
  this->setCaption(TITEL);  
//  this->setMaximumSize(WIN_BMAX,WIN_HMAX);
  this->setMinimumSize(WIN_BMIN,WIN_HMIN);

  QPalette my_pal=QPalette(this->palette());  
  my_pal.setColor(QColorGroup::Light,RANDHCOLOR);
  my_pal.setColor(QColorGroup::Dark,RANDDCOLOR);
  this->setPalette(my_pal);

// 1.Zeile: Ueberschrift, Anzeigen:
  QHBox* p_oben=new QHBox(this);  
  QLabel* p_ue =new QLabel(p_oben);
  p_ue->setBackgroundPixmap(QPixmap(BACKPIX0));
  p_ue->setPixmap(QPixmap(TITELBILD)); 
   
  QHBox* p_anzeigen=new QHBox(p_oben,"anzeigen");  // Anzeigetafel
  p_anzeigen->setSpacing(8);
  p_anzeigen->setPaletteBackgroundColor(BACKGR2);
  p_anzeigen->setPaletteForegroundColor(COLOR2);
  p_anzeigen->setMinimumSize(480,24);
  p_anzeigen->setMaximumSize(480,32);
  p_anzeigen->setFrameShadow(Raised);
  p_anzeigen->setFrameShape(Panel);
  p_anzeigen->setMargin(8);
  
  QLabel* p_llevel =new QLabel(_LLEVEL,p_anzeigen); p_level  =new QLabel(p_anzeigen); 
  p_messi=new QLabel(p_anzeigen);
  QLabel* p_lzeit  =new QLabel(_LTIME,p_anzeigen); p_zeit   =new QLabel(p_anzeigen);
  QLabel* p_lpunkte=new QLabel(_LSCORE,p_anzeigen); p_punkte =new QLabel(p_anzeigen);
  
  // CPP-Patch:
  bool cpppatsch;
  cpppatsch=((p_llevel) && (p_lzeit) && (p_lpunkte));
  
// 2.Zeile: Spielbrett:
  my_view=new viewport(this,"playfield"); // QCanvasView/ScrollView-Variante 
  
// 3.Zeile: Spacer:
  QLabel* p_unten=new QLabel(this);  
  p_unten->setText("");
  p_unten->setBackgroundPixmap(QPixmap(BACKPIX0));
  
  setStretchFactor(p_oben,1);
  setStretchFactor(p_unten,99);
}

// Slots:
void window::set_punkte(int p)    { p_punkte->setNum(p); }
void window::set_zeit(int p)      { p_zeit->setNum(p);   }
void window::set_level(int p)     { p_level->setNum(p);  }
void window::set_messi(QString p) { p_messi->setText(p); }

// ================================================================================================================
