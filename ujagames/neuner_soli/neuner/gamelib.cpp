/****************************************************************************************
    Copyright 2004 uja - Dr. Ulrike Jahnke-Soltau - www.gamecraft.de, www.ujaswelt.de
    Copyright 2004 Trolltech (Qt3)
    This file is part of ujagames_qt - card variant which run under GPL.
*****************************************************************************************/

#include "gamelib.h"

// Spielfelddarstellung - klick- und hoversensitiver View
// ----------------------------------------------------------------------------------------------------------------------------
viewport::viewport(QWidget* parent, char* name):QCanvasView(parent,name)
{ 
  int xmax=600,ymax=480;             // Daten aus config.h des jeweiligen Programms
  setMinimumSize(xmax+4,ymax+4);             // feste Feldgroesse setzen, Rand mit einrechnen
  setMaximumWidth(XMAX*DX);
  setHScrollBarMode(QScrollView::AlwaysOff);        // Scrollbalken abschalten
  //  QScrollView::viewport()->setMouseTracking(true);  // Hover-Effekte ermoeglichen

  // schicker Rand:
  // Die Randfarben koennen nicht einzeln veraendert werden, deshalb wurde die aktuelle Palette geklont und in ihr
  // die neuen Randfarben eingestellt. Die neue Palette wurde dem Widget zugewiesen.
  QPalette my_pal=QPalette(this->palette());        
  my_pal.setColor(QColorGroup::Light,RANDHCOLOR);
  my_pal.setColor(QColorGroup::Dark,RANDDCOLOR);
  this->setPalette(my_pal);
  setFrameShape(Box);
  setFrameStyle(Panel);
  setFrameShadow(Sunken);
}
  
void viewport::contentsMousePressEvent(QMouseEvent *e) { int xx=e->x(),yy=e->y(); emit geklickt(xx,yy); }
// void viewport::contentsMouseMoveEvent(QMouseEvent* e)  { int xx=e->x(),yy=e->y(); emit hover(xx,yy); }
// void viewport::contentsWheelEvent(QWheelEvent* e)      { if (e) setContentsPos(OFX,OFY); } // auf Startwerte zuruecksetzen

// ====================================================================================================================

// Container zu Anzeigen eines RTF-Textes:
textviewer::textviewer(QWidget* parent,char* name): QVBox(parent,name)
{ 
  // Textfeld, Reispapier als Hintergrund:
  my_text=new QTextEdit(this); 
  my_text->setReadOnly(true);       // nur Textanzeige
  my_text->setTextFormat(RichText);
  QBrush* my_brush=new QBrush(COLOR1,QPixmap(PAPERBILD));
  my_text->setPaper(*my_brush);
     
  // mittiger Schliessen-Button:
  QHBox* p_mittig=new QHBox(this);
  QLabel* p_leer1=new QLabel(p_mittig);
  QPushButton* p_wech=new QPushButton(_CLOSE,p_mittig);
  QLabel* p_leer2=new QLabel(p_mittig);
  p_mittig->setStretchFactor(p_leer1,10); 
  p_mittig->setStretchFactor(p_leer2,10); 
  p_mittig->setStretchFactor(p_wech,10);
  connect(p_wech,SIGNAL(clicked()),this,SLOT(hide()));            
}

// Slot: Einlesen und Darstellen eines der 3 Texte Hilfe, Info, Lizenz:
// ---------------------------------------------------------------------
void textviewer::show_info(const int flag)
{ 
  QString buff,infile;
  buff=PROGNAME;
  if (flag==0)      { infile=INFOFILE; resize(INFOBREITE,320); setCaption((buff+" - "+_INFO)); }
  else if (flag==1) { infile=HELPFILE; resize(HELPBREITE,320); setCaption((buff+" - "+_HELP)); }
  else { infile="unknown file"; resize(240,96); }
  
  QString buffout="Couldn't find "+infile;
  QFile file(infile); // Text als Stream einlesen
  if (file.open(IO_ReadOnly)) { QTextStream stream(&file); my_text->setText(stream.read()); file.close(); }

  polish();
  show();
  raise();
}

// ===============================================================================================================
