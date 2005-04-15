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
  setBackgroundColor(QColor(BACKGR0));
  QPalette my_pal=QPalette(this->palette());
  my_pal.setColor(QColorGroup::Light,RANDHCOLOR);
  my_pal.setColor(QColorGroup::Dark,RANDDCOLOR);
  this->setPalette(my_pal);

  centerwig=new QHBox(this);
  centerwig->setBackgroundColor(QColor(BACKGR0));
  centerwig->setBackgroundOrigin(WindowOrigin);
  centerwig->setMaximumSize(800,600);

// 1.Spalte: Spielbrett:
  my_view=new viewport(centerwig,"playfield"); // QCanvasView/ScrollView, Hover, Klick

// 2.Spalte: Überschrift, Messagearea, Anzeigen und Wahlmöglichkeiten:
  spalte2=new QVBox(centerwig);
  spalte2->setBackgroundOrigin(WindowOrigin);
  spalte2->setBackgroundColor(QColor(BACKGR0));
// Ueberschrift:
  QFont* my_font=new QFont();
  my_font->setFamily("URW Chancery L");
  my_font->setStyleStrategy(QFont::PreferAntialias);
  my_font->setStyleStrategy(QFont::PreferOutline);
  my_font->setPointSize(32); my_font->setWeight(4);
  p_ue =new QLabel(spalte2); p_ue->setText(PROGNAME); 
  p_ue->setFont(*my_font); p_ue->setPaletteForegroundColor(COLOR0);
  p_ue->setBackgroundOrigin(WindowOrigin);
  p_ue->setPaletteBackgroundColor(QColor(BACKGR0));
//  p_ue->setBackgroundPixmap(QPixmap(BACKPIX0));

  p_messi=new QLabel(spalte2);
  p_messi->setPaletteForegroundColor(COLOR0);
  p_messi->setBackgroundOrigin(WindowOrigin);
  p_messi->setPaletteBackgroundColor(QColor(BACKGR0));
//  p_messi->setBackgroundPixmap(QPixmap(BACKPIX0));

 // V.1.1: Code 5-spaltiges Gitter:
  p_codes=new QGrid(5,spalte2,"");
  p_codes->setBackgroundOrigin(WindowOrigin);
  p_codes->setBackgroundColor(QColor(BACKGR1));
  for (int i=0; i<XMAX; i++) 
  { p_code[i]=new QLabel(p_codes);
    p_code[i]->setFixedSize(DX,DY);
    p_code[i]->setBackgroundColor(QColor(BACKGR1));
  }

// Anzeigentafel als 2-spaltiges Gitter
  p_anzeigen=new QGrid(2,spalte2,"anzeigen");
  p_anzeigen->setPaletteBackgroundColor(QColor(BACKGR2));
  p_anzeigen->setPaletteForegroundColor(COLOR2);
  p_anzeigen->setMinimumSize(160,96);
  p_anzeigen->setFrameShadow(Sunken);
  p_anzeigen->setFrameShape(Box);
  p_anzeigen->setMargin(4);

  // Daten einfuellen:
  QLabel* p_lgames  =new QLabel(_NUMGAMES, p_anzeigen); p_numgames=new QLabel(p_anzeigen);
  QLabel* p_lspunkte=new QLabel(_SUMPUNKTE,p_anzeigen); p_sumpunkte=new QLabel(p_anzeigen);
  QLabel* p_lratio  =new QLabel(_RATIO,    p_anzeigen); p_ratio=new QLabel(p_anzeigen);
  QLabel* p_lpunkte =new QLabel(_PUNKTE,   p_anzeigen); p_punkte=new QLabel(p_anzeigen);


  // Auswahlen als 5-spaltiges Gitter:
  p_auswahl=new QButtonGroup(5,Qt::Horizontal,"",spalte2);
  p_auswahl->setBackgroundOrigin(WindowOrigin);
  p_auswahl->setBackgroundColor(QColor(BACKGR0));
  for (int i=0; i<XMAX; i++) 
  { p_bchip[i]=new QPushButton(p_auswahl);
    p_bchip[i]->setFixedSize(DX,DY);
    p_bchip[i]->setPaletteBackgroundColor(QColor("#000000"));
  }

  // Steuerung:
  p_steuer=new QGrid(2,spalte2,"anzeigen");
  p_steuer->setBackgroundOrigin(WindowOrigin);
  p_steuer->setPaletteBackgroundColor(QColor(BACKGR0));
  p_auswerten =new QPushButton(_AUSWERTEN,p_steuer);
  p_neue_runde=new QPushButton(_NEU,p_steuer);
  p_auswerten->setPaletteBackgroundColor(QColor(BACKGR2));
  p_neue_runde->setPaletteBackgroundColor(QColor(BACKGR2));

  set_messi("Messages");
  set_numgames(0);
  set_sumpunkte(0);
  set_ratio(0,1);
  set_punkte(0);
  set_numfarben(6);
}

// Slots: - nicht benoetigte Slots stoeren erstmal nicht
void window::set_punkte(int p)        { p_punkte->setNum(p); }
void window::set_messi(QString p)     { p_messi->setText(p); }
void window::set_ratio(int n1,int n2) { if (n2>0) p_ratio->setNum(float(n1)/n2); else p_ratio->setNum(-1); }
void window::set_numgames(int nr)     { p_numgames->setNum(nr); }
void window::set_sumpunkte(int nr)    { p_sumpunkte->setNum(nr); }

void window::set_numfarben(int nr) 
{ nfarben=nr;
  QString bu="";
  int k=fset*XMAX;
  for (int i=0; i<XMAX; i++) if (i<nr) 
  { bu=bu.setNum(i+k);
    if ((k+i)<10) bu="./images/s_000"+bu+".png"; else bu="./images/s_00"+bu+".png";
    p_bchip[i]->setPixmap(QPixmap(bu));
  }
  else p_bchip[i]->setText("");
}

// V.1.1:

void window::show_code(QString lsg)
{ QString bu="";
  int i,j,k=fset*XMAX;
  for (i=0; i<XMAX; i++)
  { bu=lsg.section(',',i,i); 
    if (bu!="") 
    { j=bu.toInt();
      j=j+k;
      bu=bu.setNum(j);
      if (j<10) bu="./images/s_000"+bu+".png"; else bu="./images/s_00"+bu+".png";
      p_code[i]->setPixmap(QPixmap(bu));
    }
  }
}

void window::loesche_code() { for (int i=0; i<XMAX; i++) p_code[i]->setText(""); }

void window::farbset(int nr)
{ fset=nr;
  set_numfarben(nfarben);
  emit farbset_changed(fset);
}

// in voller Laenge, da das Beispiel aus der Qt-Doku nicht funktionierte:
void window::bgset(int nr)
{ bset=nr;
  switch (bset)
  { case 0: this             ->setBackgroundPixmap(QPixmap(BACKPIX0)); // Hauptfenster
            centerwig        ->setBackgroundPixmap(QPixmap(BACKPIX0)); // Zentrierungsrahmen
            my_view->canvas()->setBackgroundPixmap(QPixmap(BACKPIX1));
            spalte2          ->setBackgroundPixmap(QPixmap(BACKPIX0));
            p_anzeigen       ->setBackgroundPixmap(QPixmap(BACKPIX2));
            p_steuer         ->setBackgroundPixmap(QPixmap(BACKPIX0));
            p_auswerten      ->setPaletteBackgroundPixmap(QPixmap(BACKPIX2));
            p_neue_runde     ->setPaletteBackgroundPixmap(QPixmap(BACKPIX2));
            p_ue             ->setBackgroundPixmap(QPixmap(BACKPIX0));
            p_messi          ->setBackgroundPixmap(QPixmap(BACKPIX0));
            p_auswahl        ->setBackgroundPixmap(QPixmap(BACKPIX1));
            p_codes          ->setBackgroundPixmap(QPixmap(BACKPIX1));
            for (int i=0; i<XMAX; i++) p_code[i]->setBackgroundPixmap(QPixmap(BACKPIX1));
            break;
    case 1: this             ->setBackgroundPixmap(QPixmap("./images/eiche.png"));
            centerwig        ->setBackgroundPixmap(QPixmap("./images/eiche.png")); // Zentrierungsrahmen
            my_view->canvas()->setBackgroundPixmap(QPixmap("./images/rustikal.png"));
            spalte2          ->setBackgroundPixmap(QPixmap("./images/eiche.png"));
            p_anzeigen       ->setBackgroundPixmap(QPixmap("./images/birke.png"));
            p_steuer         ->setBackgroundPixmap(QPixmap("./images/eiche.png"));
            p_auswerten      ->setPaletteBackgroundPixmap(QPixmap("./images/birke.png"));
            p_neue_runde     ->setPaletteBackgroundPixmap(QPixmap("./images/birke.png"));
            p_ue             ->setBackgroundPixmap(QPixmap("./images/eiche.png"));
            p_messi          ->setBackgroundPixmap(QPixmap("./images/eiche.png"));
            p_auswahl        ->setBackgroundPixmap(QPixmap("./images/rustikal.png"));
            p_codes          ->setBackgroundPixmap(QPixmap("./images/rustikal.png"));
            for (int i=0; i<XMAX; i++) p_code[i]->setBackgroundPixmap(QPixmap("./images/rustikal.png"));
            break;
    case 2: this             ->setBackgroundPixmap(QPixmap("./images/aqua_0.png"));
            centerwig        ->setBackgroundPixmap(QPixmap("./images/aqua_0.png")); // Zentrierungsrahmen
            my_view->canvas()->setBackgroundPixmap(QPixmap("./images/aqua_1.png"));
            spalte2          ->setBackgroundPixmap(QPixmap("./images/aqua_0.png"));
            p_anzeigen       ->setBackgroundPixmap(QPixmap("./images/birke.png"));
            p_steuer         ->setBackgroundPixmap(QPixmap("./images/aqua_0.png"));
            p_auswerten      ->setPaletteBackgroundPixmap(QPixmap("./images/birke.png"));
            p_neue_runde     ->setPaletteBackgroundPixmap(QPixmap("./images/birke.png"));
            p_ue             ->setBackgroundPixmap(QPixmap("./images/aqua_0.png"));
            p_messi          ->setBackgroundPixmap(QPixmap("./images/aqua_0.png"));
            p_auswahl        ->setBackgroundPixmap(QPixmap("./images/aqua_1.png"));
            p_codes          ->setBackgroundPixmap(QPixmap("./images/aqua_1.png"));
            for (int i=0; i<XMAX; i++) p_code[i]->setBackgroundPixmap(QPixmap("./images/aqua_1.png"));
            break;
    default: this             ->setBackgroundPixmap(QPixmap(""));
             centerwig        ->setBackgroundPixmap(QPixmap("")); // Zentrierungsrahmen
             my_view->canvas()->setBackgroundPixmap(QPixmap(""));
             spalte2          ->setBackgroundPixmap(QPixmap(""));
             p_anzeigen       ->setBackgroundPixmap(QPixmap(""));
             p_steuer         ->setBackgroundPixmap(QPixmap(""));
             p_auswerten      ->setPaletteBackgroundPixmap(QPixmap(""));
             p_neue_runde     ->setPaletteBackgroundPixmap(QPixmap(""));
             p_ue             ->setBackgroundPixmap(QPixmap(""));
             p_messi          ->setBackgroundPixmap(QPixmap(""));
             p_auswahl        ->setBackgroundPixmap(QPixmap(""));
             p_codes          ->setBackgroundPixmap(QPixmap(""));
             for (int i=0; i<XMAX; i++) p_code[i]->setBackgroundPixmap(QPixmap(""));
             break;
  }
  emit bgset_changed(bset);
}

// ==================================================================================================================

void window::about()
{ QString t="<h3>MasterCode 1.1</h3><p>A little Brain Teaser in pompous wrapping (KlickiBunti)</p>";
  t=t+"<p>Author: uja (Ulrike Jahnke-Soltau) (1980,1988,1998,2005) - License: GPL v2.</p>";
  t=t+"<p>For more KlickiBuntis please visit our website<br>http://www.gamecraft.de</p>";
  QMessageBox::about(this,"Info",t);
}
