/****************************************************************************************
    Copyright 2004 uja
    Game constants of Lämmerlinge/LittleSheep which runs under GPL
*****************************************************************************************/

// files:
#define PROGICON      "./images/b_48x24_0001.png"     // icon
#define BACKPIX0      "./images/bgr0.png"             // background window
#define BACKPIX1      "./images/bgr_640x480.png"      // background canvas
#define BACKPIX2      "./images/bgr1.png"             // background signpost
#define PAPERBILD     "./images/bgr1.png"             // background textview
#define HSCPOTT       "./images/pott.png"             // Medaillen
#define BLOCKSPRITE   "./images/b_48x24_%1.png"       // sprites
#define PADDELSPRITE  "./images/paddel_%1.png"
#define GOODIESPRITE  "./images/goodie_%1.png"
#define BALLSPRITE    "./images/ball.png"             // sprites
#define FLOATERSPRITE  "./images/floater_%1.png"
#define LASERSPRITE    "./images/shot.png"


#define GAMEOVERFILE  "./images/gover.png"
#define PAUSEFILE     "./images/pause.png"
#define LEVELFILE     "./levels.csv"                  // Levelcode
#define ULEVELFILE    "./userlevels.csv"              // Levelcode Usereingabe
#define HSCFILE       "./hiscore.csv"                 // fallback highscore list (local)

// colors: 
#define BACKGR0   "#000077"   // Aussenfenster Hintergrund
#define COLOR0    "#cccc99"   // Aussenfenster Textfarbe
#define BACKGR1   "#334455"   // Spielfeld Hintergrund
#define COLOR1    "#cccc99"   // Spielfeld Textfarbe
#define BACKGR2   "#cccccc"   // Anzeigen Hintergrund
#define COLOR2    "#330000"   // Anzeigen Textfarbe 
#define PAPCOLOR0  "#cccccc"  // Rahmenfarbe HSCListe
#define HICOLOR    "#990011"  // Hiscore-Top-Farbe
#define LOCOLOR    "#003355"  // Hiscore-Textfarbe
#define RANDHCOLOR "#ccddee"  // Randfarben Spielfeld
#define RANDDCOLOR "#334455"

// pics:
#define BACKIMA0 ""
#define BACKIMA1 "./images/schiefer.png"
#define BACKIMA2 "./images/sky.png"
#define BACKIMA3 "./images/deepspace.png"

#define TILESET0 "./images/s_%1.png"
#define TILESET1 "./images/b_48x24_%1.png"
#define TILESET2 "./images/h_%1.png"



// sizes:
#define WIN_BMAX 800 // window sizes, optimized 800x600
#define WIN_HMAX 600
#define WIN_BMIN 720
#define WIN_HMIN 480 // min.width HSCList
#define HSCBREITE 540 
#define HELPBREITE 480
#define INFOBREITE 320

#define MAXSPEICHER  8 // Number games to store
#define MAXEINTRAG  20 // max. entries Hiscore list
#define HSCGRENZE    5 // nr of best players to hilite in Hiscore list 

#define MAXLEVELS   99 // maximum levels to store, not the actual num of levels!


#define DX 48     // sprite size, playfield offsets    
#define DY 24
#define OFX 32
#define OFY 32
#define XMAX 12   // field dimensions in sprite units
#define YMAX 16
#define SMAX 192  // max. num fields (for events)  
// ---------------------------------------------------------------------------------
#define MAXBAELLE 8
