/****************************************************************************************
    Copyright 2004 uja
    This file is part of Raeumen which runs under GPL
*****************************************************************************************/

//language specific text:
// ----------------------

#define _CLOSE         "schliessen"
#define _HELP          "Wie spielt man..."
#define _HISCORE       "Bestenliste"
#define _HISCORERESET  "Bestenliste löschen"
#define _HSCMESS0      "Du hast Dir mit "        
#define _HSCMESS1      "Punkten einen Platz in der Bestenliste erobert!\nBitte gib Deinen Namen ein:"
#define _CONFRESET     "Soll die Bestenliste tatsächlich zurückgesetzt werden?"
#define _YES           "Ja, klar doch!"
#define _NO            "Nee, lieber doch nicht!"
#define _INFO          "Info"

#define _LLEVEL        "<b>Level:</b>"      
#define _LSCORE        "<b>Punkte:</b>"
#define _LTIME         "<b>Zeit</b>:"
#define _LICENSE       "Lizenzbestimmungen"
#define _LOADLEVEL     "Spiel laden von"
#define _LOCAL         "lokal"
#define _MENU1         "Spiel                     "
#define _MENU2         "Spielparameter"
#define _MENU3         "Spielstand laden/speichern"
#define _MENU4         "Info"
#define _NET           "Netzwerk"
#define _NUMPATT       "Anzahl verschiedener Steine"

#define _NEWGAME       "&neues Spiel"
#define _PAUSE         "&Pause"
#define _PATT0         "Buntmetall"
#define _PATT1         "Perlen"
#define _PATT2         "Mexiko"
#define _PATTNUM       "Steineauswahl"
#define _QUIT          "beenden"
#define _RESTARTLEVEL  "&Level von vorne"
#define _SAVELEVEL     "Spiel speichern auf"
#define _SAVEPARMS     "Parameter speichern"

// -------------------------------------------------------------

// layout and game parameters:
#define TITEL     "Aufräumen!"  // Programmtitel
#define TITELBILD "./images/ue.png"        // Titel-Schmuckbild
#define PROGICON  "./images/progicon.png"  // icon
#define INIFILE   "./gameparm.ini"         // some game parameters
#define SPEICHER  "./speicher.ini"         // stores for gamers 
#define PROGNAME  "Raeumen"

// module window
#define WIN_BMAX 800
#define WIN_HMAX 600
#define WIN_BMIN 600
#define WIN_HMIN 480
#define BACKGR0   "#334455"                     // Aussenfenster Hintergrund
#define COLOR0    "#ccddee"                     // Aussenfenster Textfarbe
#define BACKPIX0  "./images/bgr0.png"           // Aussenfenster Textur
#define BACKGR1   "#001122"                     // Spielfeld Hintergrund
#define COLOR1    "#ccddee"                     // Spielfeld Textfarbe
#define BACKPIX1  "./images/bgr0.png"       
#define BACKGR2   "#ccbb99"                     // Anzeigen Hintergrund
#define BACKPIX2 "./images/birke.png"
#define COLOR2    "#000033"                     // Anzeigen Textfarbe 

#define RANDHCOLOR "#ccddee"
#define RANDDCOLOR "#445566"

// module hiscore:
#define HSCBREITE  480
#define MAXEINTRAG 20                              // max. number entries
#define HSCFILE    "./texte/hiscore.csv"           // hiscore file when hiscore.ini is missing
#define TIMEFORM   "dd.MM.yyyy hh:mm:ss"           // time formatting
#define HSCGRENZE  5                               // top player size
#define PAPERBILD  "./images/reispapier_weiss.png" // background image hiscore
#define PAPCOLOR0  "#ddcc99"                       // frame color hiscore
#define HICOLOR    "#990011"                       // Hiscore top color
#define LOCOLOR    "#003355"                       // Hiscore textcolor
#define HSCPOTT    "./images/pott.png"             // Trophy

// module Textviewer:
#define HELPFILE   "./texte/hilfe.rtf"
#define INFOFILE   "./texte/info.rtf"
#define GPLFILE    "./texte/gpl.rtf"
#define HELPBREITE 400
#define INFOBREITE 284

// Sprites and other game parameters:
#define NUMBG       2  // 2 background modi - hier nicht gebraucht
#define NUMSPRITES 30  // 30 Sprite states
#define NUMVORRAT   5  // previwe size

#define SPRITEPICS  "./images/k_%1.png"
#define BGPICS      "./images/b_%1.png" // hier nicht gebraucht
#define GAMEOVERPIC "./images/gover.png"
#define PAUSEPIC    "./images/pause.png"

#define ZEIT0 500   // max. time for a level
#define DX     36   // tilesize
#define DY     36   // tilesize
#define OFX    16   // border
#define OFY    16   // border
#define XMAX   20   // board size in tiles
#define YMAX   10   // board size in tiles
#define SMAX  200   // max. tiles used
#define CMAX    5   // max. patterns
// ------------------------------------------------------------------------------------
