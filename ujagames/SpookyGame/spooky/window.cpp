/****************************************************************************************
    Copyright 2004 uja
    Copyright 2004 Trolltech (Qt3)

    This file is part of SpookyShooter which runs under GPL.
*****************************************************************************************/

#include "window.h"
// ================================================================================================

window::window(QWidget *parent,char* name): QHBox(parent,name)
{ 
  //Geometrie und Aussehen: Elemente bei einer QHBox sind spaltenfoermig angelegt:
  this->setMargin(8);
  this->setSpacing(4);
  this->setPaletteBackgroundColor(BACKGR0);
  this->setPaletteBackgroundPixmap(QPixmap(BACKPIX0));
  this->setCaption(_TITEL);  
//  this->setMaximumSize(WIN_BMAX,WIN_HMAX);
  this->setMinimumSize(WIN_BMIN,WIN_HMIN);

  QLabel* p_spalte=new QLabel(_NIX,this); // nur Spacer

// erste Spalte:  der Canvasview (Spielfeld)
  QVBox* p_spalte0=new QVBox(this);
  p_spalte0->setSpacing(8);
  QLabel* p_spacer000=new QLabel(_NIX,p_spalte0);
  p_spacer000->setMinimumSize(240,24);
  
  my_view=new viewport(p_spalte0,"playfield"); // QCanvasView-Variante
  
  QHBox* p_magazin=new QHBox(p_spalte0);  // Kugelvorrat
  p_magazin->setMargin(8);
  p_messi=new QLabel("Shoot first, ask later! Or better not?" ,p_magazin);
  p_messi->setPaletteForegroundColor(COLOR1);
  p_messi->setMinimumWidth(240);
  for (int i=0; i<NUMBULLETS; i++) p_bullet[i]=new QLabel(_NIX,p_magazin); 
  p_nachladen=new QPushButton(p_magazin);
  p_nachladen->setText(_NACHLADEN); 
  p_nachladen->setPaletteBackgroundColor("#99ccaa");
  QLabel* p_spacer001=new QLabel(_NIX,p_magazin);
  
  QHBox* p_anzeigen=new QHBox(p_magazin);
  p_anzeigen->setPaletteBackgroundColor(BACKGR2);
  p_anzeigen->setPaletteBackgroundPixmap(QPixmap(BACKPIX2));
  p_anzeigen->setPaletteForegroundColor(COLOR2);
  p_anzeigen->setMinimumSize(144,16);
  p_anzeigen->setFrameShadow(Sunken);
  p_anzeigen->setFrameShape(Box);
  p_anzeigen->setMargin(4);
  QLabel* p_lpunkte=new QLabel("<b>"_SCORE"</b>",p_anzeigen); 
  p_punkte =new QLabel(p_anzeigen); p_punkte->setNum(12345);

  QLabel* p_spacer0001=new QLabel(_NIX,p_spalte0);
  p_spacer0001->setMinimumSize(240,16);
  p_spalte0->setStretchFactor(p_spacer001,99);
    
  QLabel* p_spalte2=new QLabel(_NIX,this); // nur Spacer
  setStretchFactor(p_spalte,9);  
  setStretchFactor(p_spalte0,1);
  setStretchFactor(p_spalte2,9);
  
}

// Slots: 
void window::set_punkte(int p) { QString temp; temp=temp.setNum(p); temp="<b>"+temp+"</b>"; p_punkte->setText(temp);  }
void window::set_messi(QString p) {  p_messi->setText(p);  }

void window::set_bullets(int p)   
{ for (int i=0; i<NUMBULLETS; i++) 
  { if (i<p) p_bullet[i]->setPixmap(QPixmap("./images/kugel.png")); else p_bullet[i]->setText(_NIX); }
}

void window::setze_view(int x,int y)     { my_view->ensureVisible(x,y); }
void window::keyPressEvent(QKeyEvent* e) { emit key_pressed(e->key()); }
void window::grab_focus()                { setFocus(); }

void window::scroll_it(int x)            
{ int vorher,nachher;
  vorher=my_view->contentsX(); 
  my_view->scrollBy(x,0); 
  nachher=my_view->contentsX();
  emit gescrollt(nachher-vorher);
}
	

// ==================================================================================================================
