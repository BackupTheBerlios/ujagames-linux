// mastercode by uja - The game (c) uja 1978 (TR440, ForTran), 1984(C64), 1988 (TP3), 1993(TP6), 1998(Javascript), 2005(C++/Qt3)
// ==============================================================================================================================
#include "game.h"
#include <math.h>

game::game(QWidget* parent,char* name) { oberer=parent; nfarben=6; ntoepfe=4; summe=0; lauf=0; farbset=0; bgset=0; }

void game::bilanzen()
{ lauf++;
  if (geschafft)
  { punkte=(nfarben-3)*ntoepfe*(maxrunde-runde+1);
    emit punkte_changed(punkte);
    emit messi(_CODE_ERRATEN);
  }
  else 
  { QString out,bu;
    punkte=0; 
    messi(_NICHT_GESCHAFFT);
    out=out.setNum(c[0]); for (int i=1; i<ntoepfe; i++) { bu=bu.setNum(c[i]); out=out+','+bu; }
    emit loesung(out);
  }
  summe=summe+punkte;
  emit lauf_changed(lauf);
  emit punkte_changed(punkte);
  emit summe_changed(summe);
  emit ratio_changed(summe,lauf);
}

// Bedienung view-Objekt:
void game::hover(int ix,int iy) { p_cursor->move(ix-DX2,iy-DX2); }

void game::geklickt(int ix,int iy) // iy hier uninteressant
{ int posi=int(floor(ix/DX));
  if (gewaehlt>=0) if (posi<ntoepfe) 
  { int nr=runde*XMAX+posi; 
    e[posi]=gewaehlt; 
    p_stein[nr]->setFrame(gewaehlt+farbset*XMAX); p_stein[nr]->show();
  }
}

// restliche Slots:
void game::neues_spiel() 
{ int i,j,k;
  runde=0; gameover=false; geschafft=false;
  for (i=0; i<SMAX; i++) { p_stein[i]->hide(); p_marker[i]->hide(); p_bgsprite[i]->hide(); }
  maxrunde=2*ntoepfe-1;  if (maxrunde>YMAX) maxrunde=YMAX;
  for (j=0; j<maxrunde; j++) for (i=0; i<ntoepfe; i++) { k=j*XMAX+i; p_bgsprite[k]->show(); }
  srand(time(NULL) + getpid()); for (i=0; i<ntoepfe; i++) { c[i]=rand()%nfarben; e[i]=-1; }
  stein_gewaehlt(XMAX);
  emit loe_loeschen();
  emit lauf_changed(lauf);
  emit summe_changed(summe);
  emit ratio_changed(summe,lauf);
  emit messi(_START);
}

void game::stein_gewaehlt(int nr) 
{ if (nr<nfarben) { gewaehlt=nr; p_cursor->setFrame(nr+farbset*XMAX); p_cursor->show(); } 
  else { gewaehlt=-1; p_cursor->hide(); } 
}

void game::auswerten() 
{ int i,k;
  bool do_it=true; for (i=0; i<ntoepfe; i++) if (e[i]<0) do_it=false;
  if (do_it)
  { toepfe_ok=0;
    for (i=0; i<XMAX; i++) { g[i]=0; h[i]=0; }
    for (i=0; i<ntoepfe; i++) if (e[i]>=0) { g[c[i]]++; h[e[i]]++; if (c[i]==e[i]) toepfe_ok++; }
    farben_ok=0; for (i=0; i<nfarben; i++) if (g[i]<h[i]) farben_ok=farben_ok+g[i]; else farben_ok=farben_ok+h[i];
    k=XMAX*runde; 
    for (i=0; i<ntoepfe; i++) { p_marker[k+i]->setFrame(0); p_marker[k+i]->show(); }
    if (farben_ok>0) for (i=0; i<farben_ok; i++) p_marker[k+i]->setFrame(1);
    if (toepfe_ok>0) for (i=0; i<toepfe_ok; i++) p_marker[k+i]->setFrame(2);
    for (i=0; i<XMAX; i++) e[i]=-1;
    runde++;
    geschafft=(toepfe_ok==ntoepfe);
    gameover=(runde>=maxrunde);
    if (geschafft || gameover) bilanzen();
  }
}

void game::reset_stat()       { lauf=0; summe=0; emit lauf_changed(lauf); emit summe_changed(summe); emit ratio_changed(-1,1); }
void game::set_farben(int nr) { nfarben=nr; neues_spiel(); emit updat_farben(nr); }
void game::set_toepfe(int nr) { ntoepfe=nr; neues_spiel(); }
void game::set_lauf(int nr)   { lauf=nr; emit lauf_changed(nr); }
void game::set_summe(int nr)  { summe=nr; emit summe_changed(nr); }

void game::save_settings() 
{ QString buff,bu;
  bu=bu.setNum(lauf);    buff=bu+';';
  bu=bu.setNum(summe);   buff=buff+bu+';';
  bu=bu.setNum(nfarben); buff=buff+bu+';';
  bu=bu.setNum(ntoepfe); buff=buff+bu+';';
// V.1.1:
  bu=bu.setNum(farbset); buff=buff+bu+';';
  bu=bu.setNum(bgset);   buff=buff+bu+"\n";

  QFile fh("./gameparm.ini"); 
  if (fh.open(IO_WriteOnly)) { fh.writeBlock(buff,buff.length()); fh.close(); }
  emit messi(_SETTINGSSAVED);
}

// Version 1.1, signal von window
void game::farbset_changed(int nr)
{ farbset=nr;
  if (p_cursor->isVisible()) p_cursor->setFrame(gewaehlt+farbset*XMAX);
  for (int i=0; i<SMAX; i++) if (p_stein[i]->isVisible()) p_stein[i]->setFrame(p_stein[i]->frame()%XMAX+farbset*XMAX);
}

void game::bgset_changed(int nr) { bgset=nr; } // fuer settings

// =============================================================================================================================
