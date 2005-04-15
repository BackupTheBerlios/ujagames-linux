/****************************************************************************************
    Copyright 2004 uja
    Copyright 2004 Trolltech (Qt3)
    This file is part of Neuner which runs under GPL.
*****************************************************************************************/

#include "window.h"

window::window(QWidget *parent,char* name): QVBox(parent,name)
{ 
  this->setMargin(16);
  this->setSpacing(4);
  this->setPaletteBackgroundColor(BACKGR0);
  this->setBackgroundPixmap(QPixmap(BACKPIX0));
  this->setCaption(TITEL);  
//  this->setMaximumSize(WIN_BMAX,WIN_HMAX);
  this->setMinimumSize(WIN_BMIN,WIN_HMIN);

// Zeile oben: Name, Anzeigen, Icon
  QHBox* p_zeile0=new QHBox(this);
  p_zeile0->setSpacing(8);
  p_zeile0->setBackgroundPixmap(QPixmap(BACKPIX0)); 
// Ueberschrift als Pixmap
  QLabel* p_ue =new QLabel(p_zeile0); p_ue->setBackgroundPixmap(QPixmap(BACKPIX0)); p_ue->setPixmap(QPixmap(TITELBILD)); 
  p_messi      =new QLabel(p_zeile0); p_messi->setBackgroundPixmap(QPixmap(BACKPIX0));
  p_messi->setPaletteForegroundColor(COLOR0);
  p_messi->setText("Rest: xxx");
  
  p_austeilen  =new QPushButton(p_zeile0);
  p_austeilen->setPaletteBackgroundColor(BACKGR2);
  p_austeilen->setText(_DEAL);
  
  p_abraeumen  =new QPushButton(p_zeile0); 
  p_abraeumen->setPaletteBackgroundColor(BACKGR2);
  p_abraeumen->setText(_PLAY);
  
  p_neues_spiel=new QPushButton(p_zeile0); 
  p_neues_spiel->setPaletteBackgroundColor(BACKGR2);
  p_neues_spiel->setText(_NEWGAME);
  
    
  p_zeile0->setStretchFactor(p_ue,5);
  p_zeile0->setStretchFactor(p_austeilen,2);
  p_zeile0->setStretchFactor(p_abraeumen,1);
  p_zeile0->setStretchFactor(p_neues_spiel,1);

// Viewport - Rest es Bildschirms
  my_view=new viewport(this,"playfield"); // QCanvasView-Variante
  my_view->show(); 
}

void window::set_messi(QString t) { p_messi->setText("<b>"+t+"</b>"); }
