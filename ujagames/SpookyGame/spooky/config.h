/****************************************************************************************
    Copyright 2004 uja
    Game constants of Lämmerlinge/LittleSheep which runs under GPL
*****************************************************************************************/

// files:
#define PROGICON      "./images/progicon.png"         // icon
#define BACKPIX0      "./images/bgr.png"              // background window
#define BACKPIX1      "./images/hintergrund_long.png"      // background canvas
#define BACKPIX2      "./images/bgr2.png"            // background signpost
#define PAPERBILD     "./images/bgr2.png" // background textview
#define HSCPOTT       "./images/pott.png"             // Medaillen
#define MOVINGSPRITE  "./images/moving_%1.png"        // flying sprites (Mond,hexen,geister,kuerbisse,spinnen,katze
                                                      // Punkte: -100, +5,+10,+20,+50,+100
#define KUGELSPRITE   "./images/kugel.png"            // 6 bullets
#define ZIELSPRITE    "./images/zielen.png"           // Zielkreuz
#define STATICSPRITE  "./images/static_%1.png"        // fries,mauer,turm hoch,turm niedrig,baumkl,mittel,groß

#define GAMEOVERFILE  "./images/gover.png"
#define PAUSEFILE     "./images/pause.png"
#define HSCFILE       "./hiscore.csv"                 // fallback highscore list (local)

// colors: 
#define BACKGR0   "#220033"   // Aussenfenster Hintergrund
#define COLOR0    "#ccee99"   // Aussenfenster Textfarbe
#define BACKGR1   "#334455"   // Spielfeld Hintergrund
#define COLOR1    "#cccc99"   // Spielfeld Textfarbe
#define BACKGR2   "#bbcc99"   // Anzeigen Hintergrund
#define COLOR2    "#330000"   // Anzeigen Textfarbe 
#define PAPCOLOR0  "#ccdd99"  // Rahmenfarbe HSCListe
#define HICOLOR    "#990011"  // Hiscore-Top-Farbe
#define LOCOLOR    "#003355"  // Hiscore-Textfarbe
#define RANDHCOLOR "#ccee99"  // Randfarben Spielfeld
#define RANDDCOLOR "#110022"

// sizes:
#define WIN_BMAX 800 // window sizes, optimized 800x600
#define WIN_HMAX 600
#define WIN_BMIN 720
#define WIN_HMIN 480 // min.width HSCList
#define HSCBREITE 540 
#define HELPBREITE 480
#define INFOBREITE 320

#define MAXEINTRAG  20 // max. entries Hiscore list
#define HSCGRENZE    5 // nr of best players to hilite in Hiscore list 

#define NUMMOVING   26 // num sprite states 6 Hexen, 3 Geister, 3 Kürbisse, 2 Spinnen, 1 Mond, 1 Katze, 6 Wertungen
#define NUMSTATIC    8 // Fried, Mauer, Turm l, Turm 2(niedriger), 3 Bäume, Vordergrund 
#define NUMBULLETS   6 // 6 Kugeln im Magazin

#define DX 32     // sprite size, playfield offsets    
#define DY 32
#define OFX 80
#define OFY 32
#define XMAX 1920   // field dimensions
#define YMAX 400 
// ---------------------------------------------------------------------------------
