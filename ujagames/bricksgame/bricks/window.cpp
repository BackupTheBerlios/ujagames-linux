/****************************************************************************************
    Copyright 2004 uja
    Copyright 2004 Trolltech (Qt3)

    This file is part of ujagames lib which runs under GPL.
*****************************************************************************************/

#include "window.h"
#include <qfont.h>
// ================================================================================================

window::window(QWidget *parent,char* name): QHBox(parent,name)
{ 
  //Geometrie und Aussehen: Elemente bei einer QHBox sind spaltenfoermig angelegt:
  this->setMargin(16);
  this->setSpacing(4);
  this->setPaletteBackgroundColor(BACKGR0);
  this->setPaletteBackgroundPixmap(QPixmap(BACKPIX0));
  this->setCaption(_TITEL);  
//  this->setMaximumSize(WIN_BMAX,WIN_HMAX);
  this->setMinimumSize(WIN_BMIN,WIN_HMIN);

  QLabel* p_spalte=new QLabel(_NIX,this); // nur Spacer

// erste Spalte: Canvasview (Spielfeld)
  QVBox* p_spalte0=new QVBox(this);
  p_spalte0->setSpacing(8);
  
  my_view=new viewport(p_spalte0,"playfield"); // QCanvasView-Variante
  QLabel* p_spacer01=new QLabel(_NIX,p_spalte0);
  p_spalte0->setStretchFactor(my_view,10);
  p_spalte0->setStretchFactor(p_spacer01,90);
    
// zweite Spalte: Titel, Anzeigentafel als 2-spaltiges Gitter, mittig angeordnet:  
  // Ueberschrift

  QVBox* p_spalte1=new QVBox(this);
  p_spalte1->setSpacing(8);
  QLabel* p_spacer11=new QLabel(_NIX,p_spalte1);
  
  QFont* my_font=new QFont();
  my_font->setFamily("URW Chancery L");
  my_font->setStyleStrategy(QFont::PreferAntialias);
  my_font->setStyleStrategy(QFont::PreferOutline);
  my_font->setPointSize(28);
  my_font->setWeight(4);
		        				
  QLabel* p_ue =new QLabel(p_spalte1);
  p_ue->setText(PROGNAME); 
  p_ue->setFont(*my_font);
  p_ue->setPaletteForegroundColor(COLOR0);

  p_messi=new QLabel(_NIX,p_spalte1);
  p_messi->setMinimumSize(160,128);
  p_messi->setPaletteForegroundColor(COLOR0);
  
  QGrid* p_baelle=new QGrid(MAXBAELLE,p_spalte1,"baelle");  // Anzeigetafel
  p_baelle->setMinimumSize(160,48);
  for (int i=0; i<MAXBAELLE; i++) p_ball[i]=new QLabel(p_baelle);
	  
  QGrid* p_anzeigen=new QGrid(2,p_spalte1,"anzeigen");  // Anzeigetafel
  p_anzeigen->setPaletteBackgroundPixmap(QPixmap(BACKPIX2));
  p_anzeigen->setPaletteForegroundColor(COLOR2);
  p_anzeigen->setMinimumSize(160,96);
  p_anzeigen->setFrameShadow(Sunken);
  p_anzeigen->setFrameShape(Box);
  p_anzeigen->setMargin(4);

  // Daten einfuellen:
  QLabel* p_llevel =new QLabel(_ANZEIGE0,p_anzeigen); p_level=new QLabel(p_anzeigen);
  QLabel* p_lrest  =new QLabel("<b>Bricks:</b>",p_anzeigen); p_rest=new QLabel(p_anzeigen);
  QLabel* p_lpunkte=new QLabel(_ANZEIGE1,p_anzeigen); p_punkte=new QLabel(p_anzeigen);

  QVBox* knoeppe=new QVBox(p_spalte1);
  QFont* font_b=new QFont();
  font_b->setBold(true);
  knoeppe->setFont(*font_b);
  p_neu=new QPushButton(_NEU,knoeppe);
  p_pause=new QPushButton(_PAUSECONT,knoeppe);
  p_tilt=new QPushButton(_TILT,knoeppe);
  p_neu->setPaletteBackgroundPixmap(QPixmap(BACKPIX2));
  p_pause->setPaletteBackgroundPixmap(QPixmap(BACKPIX2));
  p_tilt->setPaletteBackgroundPixmap(QPixmap(BACKPIX2));

  QLabel* p_spacer12=new QLabel(_NIX,p_spalte1); 

  p_spalte1->setStretchFactor(p_spacer11,2); 
  p_spalte1->setStretchFactor(p_messi,1); 
  p_spalte1->setStretchFactor(p_anzeigen,2); 
  p_spalte1->setStretchFactor(p_spacer12,99); 

  QLabel* p_spalte2=new QLabel(_NIX,this); // nur Spacer
  setStretchFactor(p_spalte,99);  
  setStretchFactor(p_spalte0,3);
  setStretchFactor(p_spalte1,1);
  setStretchFactor(p_spalte2,99);
  
  set_messi("Messages");
  set_level("Levelnames");
  set_leben(MAXBAELLE);
  set_rest(0);
  set_punkte(0);
  keyup=-1;
  keydn=-1;
}

// Slots: - nicht benoetigte Slots stoeren erstmal nicht
void window::set_punkte(int p)    { p_punkte->setNum(p); }
void window::set_level(QString p) { p_level->setText(p); }
void window::set_rest(int p)      { p_rest->setNum(p); }
void window::set_messi(QString p) { p_messi->setText(p); }

void window::set_leben(int p)     
{ for (int i=0; i<MAXBAELLE; i++) 
  if (p>i) p_ball[i]->setPixmap(QPixmap("./images/ball.png"));
  else p_ball[i]->setPixmap(QPixmap("./images/leer.png"));
}

void window::keyPressEvent(QKeyEvent* e) { keydn=e->key(); }
void window::keyReleaseEvent(QKeyEvent* e) { keydn=-1; }






// ==================================================================================================================
