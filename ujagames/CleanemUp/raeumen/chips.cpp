/****************************************************************************************
    Copyright 2004 uja
    Copyright 2004 Trolltech (Qt3)

    This file is part of Auf raeumen.
*****************************************************************************************/

// Initialisierung des Spiels:

#include <stdio.h>
#include "chips.h"

int koords2posi(int xx,int yy) 
{ 
  int x=(xx-OFX)/DX,y=(yy-OFY)/DY; 
  if ((x<XMAX) && (y<YMAX)) return x+y*XMAX; else return -1;
}

// Listenroutinen-Auszug: (c)uja 1989
int  in_liste(int e,int l[])  { int i,k=-1; if (l[0]>0) for (i=1; i<=l[0]; i++) if (e==l[i]) k=i; return k; }
void add_liste(int e,int l[]) { if (in_liste(e,l)<1) { l[0]++; l[l[0]]=e; } }

// ===============================================================================================================
chips::chips(QWidget* parent)
{ 
  zeit=ZEIT0;
  for (int i=0; i<SMAX; i++) 
  { f[i]=i%5;
    liste[i]=0;
  }				
  my_timer  =new QTimer(this);
  my_stopper=new QTimer(this); // fuer singleShot
  tic=200;
  
  connect(this,SIGNAL(send_messi(QString)),parent,SLOT(set_messi(QString)));   // Anzeige Punkte
  connect(this,SIGNAL(punkte_changed(int)),parent,SLOT(set_punkte(int)));   // Anzeige Punkte
  connect(this,SIGNAL(zeit_changed(int)),parent,SLOT(set_zeit(int)));       // 10-sStoppuhr
  connect(this,SIGNAL(level_changed(int)),parent,SLOT(set_level(int))); // Levelanzeige
  connect(my_timer,SIGNAL(timeout()),this,SLOT(ticken()));
  my_timer->start(1000);
  ofs_px=0;
  nfarben=3;
}

// private slots abzgl. Befehlskette:

void chips::game_over()   
{ 
  aktiv=false;
  int i,sum=0;
  for (i=0; i<SMAX; i++) sum=sum+f[i];
  if (sum==0) punkte=punkte+zeit;
  p_gover->show(); 
  emit punkte_changed(punkte);  
  emit gameover(punkte);
}

void chips::reset_timer() 
{ 
  zeit=ZEIT0; 
  if (nfarben>4) zeit=zeit*4; else if (nfarben>3) zeit=zeit*2; 
  emit zeit_changed(zeit);
}

void chips::ticken()
{ 
  if (aktiv)
  { zeit--;
    emit zeit_changed(zeit);
    if (zeit<0) game_over();
  }
}

void chips::pause()  { aktiv=false; pausiert=true;  p_pause->show(); }
void chips::weiter() { aktiv=true;  pausiert=false; p_pause->hide(); }

// --------- unabhaengiger Kram: -------------

// Feldanzeige:
void chips::zeige_feld(int nr) 
{ 
  if (f[nr]<1) p_sprite[nr]->hide();
  else 
  { int k=f[nr]+ofs_px-1; 
    if (in_liste(nr,liste)>0) k=k+CMAX; 
    p_sprite[nr]->setFrame(k);
    p_sprite[nr]->show();
  }     
}


// zusammenhaengende Werte bestimmen, braucht in_liste, add_liste, liste ist "counted array" - ujas Fill-Algo 1998 
// aktiv ist abgeschaltet, um Zwischenfaelle mit Events zu vermeiden - 
void chips::get_kette(int nr) { aktiv=false; liste[0]=1; liste[1]=nr; get_nachbarn(1,1); } 
void chips::get_nachbarn(int von,int bis)
{
  int i,k,farbe=f[liste[1]];
  for (i=von; i<=bis; i++)
  { 
    k=liste[i]+1;    if (k%XMAX>0)      if (f[k]==farbe) add_liste(k,liste);
    k=liste[i]-1;    if (k%XMAX<XMAX-1) if (f[k]==farbe) add_liste(k,liste);
    k=liste[i]+XMAX; if (k<SMAX)        if (f[k]==farbe) add_liste(k,liste);
    k=liste[i]-XMAX; if (k>=0)          if (f[k]==farbe) add_liste(k,liste);
  }    
  if (liste[0]>bis) { k=bis+1; get_nachbarn(k,liste[0]); } else aktiv=true;
}

// -----------------------------------------------------------------------------------------------------------------
// Befehlszyklus Auswertung -  aktiv ist abgeschaltet
// --------------------------------------------------------------------------------------------------------
void chips::entferne_kette()
{
  aktiv=false;
  for (int i=1; i<=liste[0]; i++) { f[liste[i]]=-1; zeige_feld(liste[i]); }
  my_stopper->singleShot(tic,this,SLOT(droppen()));
} 
  
// Steinluecken beseitigen:
// pro Spalte von unten her besetzte Felder in genullten Vektor einsammeln,
// diesen dann in der Spalte abbilden:
void chips::droppen()
{ 
  int i,j,k,pvek,vek[YMAX];
  for (i=0; i<XMAX; i++)
  { for (j=0; j<YMAX; j++) vek[j]=0;
    pvek=YMAX-1;  // Reserveboden
    for (j=YMAX-1; j>=0; j--) 
    { k=j*XMAX+i;
      if (f[k]>0) { vek[pvek]=f[k]; pvek--; } 
    }
    for (j=0; j<YMAX; j++) { k=i+j*XMAX; f[k]=vek[j]; }
  }
  for (i=0; i<SMAX; i++) zeige_feld(i);
  my_stopper->singleShot(tic,this,SLOT(nach_links()));
}  

void chips::nach_links()
{ 
  int i,j,k,i1,j1;
  for (i=XMAX-1; i>=0; i--) 
  { k=0; for (j=0; j<YMAX; j++) k=k+f[i+j*XMAX];
    if (k<1) for (j1=0; j1<YMAX; j1++) for (i1=i; i1<XMAX; i1++) { k=i1+j1*XMAX; if (i1<XMAX-1) f[k]=f[k+1]; else f[k]=0; }
  }      
  liste[0]=0; for (i=0; i<SMAX; i++) zeige_feld(i);
  if (zug_moeglich()) aktiv=true; else my_stopper->singleShot(1000,this,SLOT(game_over()));
}  

// -----------------------------------------------------------------------------------------------------------------------
void chips::updat_punkte(int nr)
{ 
  int i,k=3,n=nr-k; 
  if (n>0) for (i=1; i<=nr; i++) k=k+i; 
  if (nfarben>4) k=k*4; else if (nfarben>3) k=k*2;
  punkte=punkte+k; 
  emit punkte_changed(punkte);
}

int chips::zug_moeglich() 
{ 
  bool ok=false;
  int i,j,k; 
  for (i=1; i<XMAX; i++) for (j=1; j<YMAX; j++) 
  { 
    k=i+j*XMAX; 
    if (f[k]>0) if ((f[k]==f[k-1]) || (f[k]==f[k-XMAX])) ok=true;
  }
  // rand links noch checken
  if (!ok) for (i=1; i<YMAX; i++) { k=i*XMAX; if (f[k]>0) if (f[k]==f[k-XMAX]) ok=true; } 
  return ok;
}  

// -------------------------------------------------------------------------------------------------------------------
// --- Interaktion mit User abgesehen von dem Kleinvieh ganz oben: ----------------------------------------------------

void chips::neues_spiel()
{ 
  int i;
  punkte=0;
  my_stopper->stop();
  srand(time(0)); 
  emit punkte_changed(punkte); 
  emit level_changed(nfarben);
  p_gover->hide();
  liste[0]=0;
  oldpos=-1;
  p_gover->hide();
  do for (i=0; i<SMAX; i++) f[i]=rand()%nfarben+1; while(!zug_moeglich());
  for (i=0; i<SMAX; i++) { f[i]=rand()%nfarben+1; zeige_feld(i); }
  reset_timer();
  weiter();
}

void chips::klick(int xx,int yy)
{ 
  int posi=koords2posi(xx,yy);
  if (aktiv) 
  { if (f[posi]>0)
    {
      get_kette(posi);
      if (liste[0]>1) { updat_punkte(liste[0]); entferne_kette(); }
      oldpos=-1;
    }
  }
  else if (pausiert) weiter();
}

void chips::show_kette(int xx,int yy)
{
  int posi=koords2posi(xx,yy);
  if (aktiv) if (posi!=oldpos) 
  { 
    liste[0]=0;
    oldpos=posi;
    if ((posi>=0) && (f[posi]>0)) { get_kette(posi); if (liste[0]<2) liste[0]=0; }
    QString out; out.setNum(liste[0]); out=out+" Chips"; emit send_messi(out);
    for (int i=0; i<SMAX; i++) zeige_feld(i);
  }
}

void chips::set_modus(int nr)    { nfarben=nr+3; emit level_changed(nfarben); neues_spiel();  }
void chips::set_chipsatz(int nr) { ofs_px=2*nr*CMAX; for (int i=0; i<SMAX; i++) zeige_feld(i); }

void chips::save_settings(int hsc)
{
  QFile file(INIFILE); 
  QString buff,bu,b;
  buff.setNum((nfarben-3));
  bu.setNum(ofs_px/CMAX/2);
  b.setNum(hsc);
  buff=b+"; hscpath\n"+buff+';'+bu+";  numid,tileid\n";
  if (file.open(IO_WriteOnly)) { file.writeBlock(buff,buff.length()); file.close(); }
}

