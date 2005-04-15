/****************************************************************************************
    Copyright 2004 uja 
    Copyright 2004 Trolltech (Qt3)
    published under GPL
    This file is part of Fleur: game implementation
*****************************************************************************************/
#include <stdio.h> // for debugging purpose
#include "chips.h"
#include <math.h>

// 17.09.2004 uja: Screenaufbau, Menus, Anzeigen 
// 18.09.2004 uja; Slots fuer Klasse Chips, Klasse chips: Header, Konstruktor  
// 20.09.2004 uja: Klasse Karten, Kartenbilder konvertiert, weitere Grafiken fuer Screenaufbau
//                 Slots reihe_austeilen (Spielregeln 1), neues_spiel, spielstart und  Markierungen bei Anwahl (Spielregeln 2)
// 21.09.2004 uja: Slot klick: reihe2stack, stack2stack, stack2reihe, reihe2reihe, autoplay mit Animation 
// 22.09.2004 uja: Tests, Endeerkennung und Routine, Buttons eingefaerbt, Texte erstellt -> V.1.00
// ----------------------------------------------------------------------------------------------------------------------------


// Listenroutinen-Auszug: (c)uja 1989
int  in_liste(int e,int l[])  { int i,k=-1; if (l[0]>0) for (i=1; i<=l[0]; i++) if (e==l[i]) k=i; return k; }
void add_liste(int e,int l[]) { if (in_liste(e,l)<1) { l[0]++; l[l[0]]=e; } }

// ======================================================================================================================

karte::karte(int fa,int bi,int de,bool o) 
{ 
  farbe=fa; bild=bi; deck=de; 
  x=0; y=0;
  z=bild+13*farbe+NUMSPRITES*deck; 
  code=bild+13*farbe; 
  dpix=2*NUMSPRITES+deck;
  offen=o;
}

void karte::open_card(bool o) { offen=o; norm_card(); }
void karte::move_card(int xx,int yy,int zz) { x=xx; y=yy, z=zz; p_sprite->setZ(z); p_sprite->move(x,y); }
void karte::mark_card() { p_sprite->setFrame(code+NUMSPRITES); } 
void karte::norm_card() { if (offen) p_sprite->setFrame(code); else p_sprite->setFrame(dpix); }

bool karte::is_open()   { return offen; }
int  karte::get_farbe() { return farbe; }
int  karte::get_bild()  { return bild;  }

// ========================================================================================================================

chips::chips(QWidget* parent)
{ 
  int i,j,k,ii=0;
  for (k=0; k<NUMBG; k++) for (j=0; j<4; j++) for (i=0; i<13; i++)   
  {   
    talon[ii]=ii;
    my_card[ii]=new karte(j,i,k,true);
    ii++; 
  }
  akt_zug.posnr=-1;
  ani1=true;
  ani2=true;
  stopper=new QTimer(this); 
  tic=100; // Animationstimer
  srand(time(0)); 
  connect(stopper,SIGNAL(timeout()),this,SLOT(cycle()));                     // Endebild
  connect(this,SIGNAL(send_messi(QString)),parent,SLOT(set_messi(QString))); // Restanzeige
}

// ---------------------------------------------------------------------------------

// Check, ob Karten zueinander passen:
bool chips::passt(karte* k1,karte* k2,bool farbwechsel,bool aufsteigend, bool wrap)
{
  int diff;
  bool ok;
  if (farbwechsel) ok=(k1->get_farbe()/2 != k2->get_farbe()/2); else ok=(k1->get_farbe()==k2->get_farbe());
  if (ok) 
  { diff=k1->get_bild()-k2->get_bild(); if (aufsteigend) ok=(diff==1); else ok=(diff==-1);
    if (!ok) if (wrap)
    { if (aufsteigend) ok=((k1->get_bild()==0) && (k2->get_bild()==12));
       else ok=((k1->get_bild()==12) && (k2->get_bild()==0));
    }       
  }    
  return ok;    
}

// suchen nach oberster freier Karte in Kartenreihe, Rueckgabe: Position in Reihe
int chips::oberste_freie(int nr)
{
  int i,j,frei=r_liste[nr][0];
  bool weiter=true;
  while ((frei>1) && (weiter))  
  { 
    i=r_liste[nr][frei];
    j=r_liste[nr][frei-1];
    if (my_card[j]->is_open()) { if (passt(my_card[i],my_card[j],true,false,true)) frei--; else weiter=false; } else weiter=false;
  }
  return frei;
}


// Feststellen und markieren der zu verschiebenden Karten im Reihenbereich: Reihennr,Startkarte

int chips::markiere_reihe(int nr,int start)
{
  int i,k,erste=oberste_freie(nr);
  if (erste<start) erste=start;  
  for (i=erste; i<=r_liste[nr][0]; i++) { k=r_liste[nr][i]; my_card[k]->mark_card(); }
  return erste;
}


// Zeichnen einer Spalte samt Cleanup (letzte oeffnen), kann Anzeige bereinigen:
void chips::zeige_reihe(int nr)
{
  int i,k,x;
  if (r_liste[nr][0]>0) for (i=1; i<=r_liste[nr][0]; i++)
  { 
    k=r_liste[nr][i];
    x=OFX+nr*DX;
    my_card[k]->move_card(x,OFY+(i-1)*DDY,i);
    my_card[k]->norm_card();
  }
  k=r_liste[nr][r_liste[nr][0]];
  my_card[k]->open_card(true);
}

void chips::loesche_marken()
{ 
  if (akt_zug.posnr>=0)
  { if (akt_zug.im_stack) my_card[s_liste[akt_zug.posnr][akt_zug.posi]]->norm_card();
    else  for (int i=1; i<=r_liste[akt_zug.posnr][0]; i++) my_card[r_liste[akt_zug.posnr][i]]->norm_card();
  }    
}

// gebraucht von autoplay:
void chips::letzte_oeffnen(int n) { if (r_liste[n][0]>0) my_card[r_liste[n][r_liste[n][0]]]->open_card(true); }
void chips::talon2reihe(int n,bool o) { if (t_ind>=0) { int i=talon[t_ind]; add_liste(i,r_liste[n]); my_card[i]->open_card(o); t_ind--; } }

bool chips::reihe2stack(int n1,int n2)
{
  bool moveit=false;
  int i=r_liste[n1][0],j=s_liste[n2][0];
  if (i>0) 
  { i=r_liste[n1][i];
    if (j<1) { if (my_card[i]->get_farbe()==n2%4) if (my_card[i]->get_bild()==0) moveit=true; }
    else     { j=s_liste[n2][j]; moveit=passt(my_card[i],my_card[j],false,true,false); }     
  }
  if (moveit)
  {
    r_liste[n1][0]--; add_liste(i,s_liste[n2]);
    my_card[i]->move_card(OFXA+n2*DX,OFYA,s_liste[n2][0]);
    letzte_oeffnen(n1);
  }
  return moveit;
}

bool chips::stack2stack(int n1,int n2)
{
  bool moveit=false;
  int i=s_liste[n1][0],j=s_liste[n2][0];
  if (i>0) 
  { i=s_liste[n1][i];
    if (j<1) { if (my_card[i]->get_farbe()==n2%4) if (my_card[i]->get_bild()==0) moveit=true; }
    else     { j=s_liste[n2][j]; moveit=passt(my_card[i],my_card[j],false,true,false); }     
  }
  if (moveit)
  {
    s_liste[n1][0]--; add_liste(i,s_liste[n2]);
    my_card[i]->move_card(OFXA+n2*DX,OFYA,s_liste[n2][0]);
  }
  return moveit;
}


bool chips::stack2reihe(int n1,int n2)
{
  bool moveit=false;
  int i=s_liste[n1][0],j=r_liste[n2][0];
  if (i>0) 
  { i=s_liste[n1][i];
    if (j<1) moveit=true; else moveit=passt(my_card[i],my_card[r_liste[n2][j]],true,false,true);
  }  
  if (moveit)
  {
    s_liste[n1][0]--; add_liste(i,r_liste[n2]);
    my_card[i]->move_card(OFX+n2*DX,OFY+(r_liste[n2][0]-1)*DDY,r_liste[n2][0]);
  }
  return moveit;
}

bool chips::reihe2reihe(int n1,int n2)
{
  bool moveit=false;
  int i=r_liste[n1][akt_zug.posi],j=r_liste[n2][0];
  if (j<1) moveit=true; else moveit=passt(my_card[i],my_card[r_liste[n2][j]],true,false,true);  
  if (n1==n2) moveit=false;
  if (moveit)
  {
    for (i=akt_zug.posi; i<=r_liste[n1][0]; i++) add_liste(r_liste[n1][i],r_liste[n2]);    
    r_liste[n1][0]=akt_zug.posi-1; // Achtng bei n1=n2! Abfangen!
    letzte_oeffnen(n1);
    zeige_reihe(n2);
  }
  return moveit;
}



// Slots:

// mischen, weiterreichen nach spielstart:
void chips::neues_spiel()
{ 
  int i,j,k;
  for (i=0; i<2*NUMSPRITES; i++) talon[i]=i; 
  for (i=0; i<2*NUMSPRITES; i++) { k=rand()%(2*NUMSPRITES); j=talon[i]; talon[i]=talon[k]; talon[k]=j; } 
  spielstart();
}

void chips::spielstart()
{
  stopper->stop();
  anim=0;
  int i,k,dx,dy,nmax=2*NUMSPRITES;
  t_ind=nmax-1;
  r_ind=0;
  for (i=0; i<SMAX; i++) s_liste[i][0]=0;
  for (i=0; i<RMAX; i++) r_liste[i][0]=0;
  for (i=0; i<nmax; i++) 
  { k=talon[i]; 
    my_card[k]->open_card(false); 
    dx=OFXA+DX*RMAX+i%8-4;
    dy=OFYA+4-i%8;
    my_card[k]->move_card(dx,dy,i+1);
    my_card[k]->p_sprite->show();
  }
  reihe_austeilen();
  aktiv=true;
}

// Neuner-spezifische Regeln:
void chips::reihe_austeilen()
{
  int i;
  bool o=((r_ind>1) && (r_ind!=5)); 
  loesche_marken();
  for (i=0; i<RMAX; i++) talon2reihe(i,o);  
  r_ind++; 
  if (o) for (i=0; i<RMAX; i++) zeige_reihe(i); else reihe_austeilen();
  akt_zug.posnr=-1;
  i=t_ind+1;
  QString out; out.setNum(i); out="Rest: "+out; emit send_messi(out);
}

void chips::autoplay()  // abraeumen in anderen Versionen
{
  loesche_marken();
  akt_zug.posnr=-1;
  bool geraeumt=false;
  for (int i=0; i<RMAX; i++) for (int j=0; j<SMAX; j++) if (reihe2stack(i,j)) geraeumt=true; 
  if (geraeumt) QTimer::singleShot(tic,this,SLOT(autoplay())); 
  else if (is_ende()) QTimer::singleShot(500,this,SLOT(enderoutine()));  
}

void chips::klick(int xx,int yy)
{ 
  if (aktiv) 
  { 
    if ((xx>RMAX*DX+OFXA) && (yy<OFY)) reihe_austeilen();
    else
    {
    if (akt_zug.posnr<0) // erste Karte, akt_zug belegen
    {
      akt_zug.im_stack=(yy<OFY);
      if (akt_zug.im_stack) // obere Ablagen (stack)
      { 
        akt_zug.posnr=(xx-OFXA)/DX;
	if (akt_zug.posnr>=SMAX) akt_zug.posnr=-1;
	if (akt_zug.posnr>=0) akt_zug.posi=s_liste[akt_zug.posnr][0];
	if (akt_zug.posi<1) akt_zug.posnr=-1;
        if (akt_zug.posnr>=0) my_card[s_liste[akt_zug.posnr][akt_zug.posi]]->mark_card();		
      }
      else // untere Ablage, eie der Kartenreihen
      {
        akt_zug.posnr=(xx-OFX)/DX;
	if (akt_zug.posnr>=RMAX) akt_zug.posnr=-1;          
        if (akt_zug.posnr>=0) akt_zug.posi=r_liste[akt_zug.posnr][0];
	if (akt_zug.posi<1) akt_zug.posnr=-1; // Reihe leer
        if (akt_zug.posnr>=0)
	{ 
	  akt_zug.posi=(yy-OFY)/DDY+1; // Pos in r_liste
	  if (akt_zug.posi>r_liste[akt_zug.posnr][0]) akt_zug.posi=r_liste[akt_zug.posnr][0]; // Laengenkorrektur
	  akt_zug.posi=markiere_reihe(akt_zug.posnr,akt_zug.posi);    // Reihenpruefung/korrektur und darstellung
	}  
      }    
    }
    else // Ziel-Position
    { int ziel=-1;
      loesche_marken();
      if (yy<OFY) // zum Stack
      { 
        ziel=(xx-OFXA)/DX; if (ziel>=SMAX) ziel=-1;
	if (ziel>=0) { if (akt_zug.im_stack) stack2stack(akt_zug.posnr,ziel); else reihe2stack(akt_zug.posnr,ziel); }
	if (is_ende()) QTimer::singleShot(500,this,SLOT(enderoutine()));
      }	
      else // zur Reihe
      {	
        ziel=(xx-OFX)/DX; if (ziel>=RMAX) ziel=-1;
	if (ziel>=0) { if (akt_zug.im_stack) stack2reihe(akt_zug.posnr,ziel); else reihe2reihe(akt_zug.posnr,ziel); }
      }
      akt_zug.posnr=-1;
     }
   }          
    
  } 
  else stopper->stop();
}

bool chips::is_ende()        { int i,k=0; for (i=0; i<SMAX; i++) k=k+s_liste[i][0]; return (k>=2*NUMSPRITES); }

// --- Ende-Schnickschnack: -------------------------------------------------------------------------------

void chips::enderoutine() 
{ 
  aktiv=false; 
  int i,mx=256,my=256,iradx=240,irady=96,ix,iy;
  float alpha=0.483312,xx,yy;
  
  for (i=0; i<NUMSPRITES; i++) 
  { xx=iradx*cos(alpha*i);
    yy=irady*sin(alpha*i);
    ix=round(xx+mx);
    iy=round(yy+my);
    my_card[i]->open_card(true);
    my_card[i]->p_sprite->move(ix,iy);
    my_card[i]->p_sprite->setZ(104-i);
    iradx=iradx-3;
    irady=irady-2;    
  }
  stopper->start(50);
}

void chips::cycle()
{
  int i,k; 
  for (i=0; i<NUMSPRITES; i++) { k=(my_card[i]->p_sprite->frame()+1)%NUMSPRITES; my_card[i]->p_sprite->setFrame(k); }
}

// ==================================================================================================================================
