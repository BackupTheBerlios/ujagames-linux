#!/bin/sh
##########################################################
# Installer ujagames_qt: brcks deutsch aRts-Soundserver #
##########################################################

# Bibliotheken finden: 
########################
KDEPFAD=""
KDEPFAD="$(kde-config --prefix)"

KDELIBS="$(find $KDEPFAD|grep libartsc.so$)"
KDELIBS="$(echo $KDELIBS|sed 's/libartsc.so//')"

ARTSINCLUDE="$(find $KDEPFAD|grep soundserver.h$)"
ARTSINCLUDE="$(echo $ARTSINCLUDE|sed 's/soundserver.h//')"

# hier eigenen Pfad eintragen:
# KDELIBS=/kderoot kdeversion/lib
# ARTSINCLUDE=/kderoot kdeversion/include/arts

if [ ! -d $KDELIBS ]
then
echo "aRts-Bibliothekspfad $KDELIBS nicht gefunden! Bitte haendisch eintragen in $0 oder NAS-Version installieren!"
exit 1
fi

if [ ! -d $ARTSINCLUDE ]
then
echo "aRts-Headerpfad $ARTSINCLUDE nicht gefunden! Bitte haendisch eintragen in $0 oder NAS-Version installieren!"
exit 1
fi

echo
echo "Soundbibliotheken gefunden in $KDELIBS und $ARTSINCLUDE"


###################################################################################################################

# Programkopf: 
################
echo "----------------------------------------------"
echo "Installation Bricks KDE/aRts deutsch"
echo "----------------------------------------------"

######################################################
# Subroutinen:
# compiling, linking:
mach_hin()
{
echo "Compilieren, Linken"
echo "--------------------------------------"
make
ret=$?
if [ $ret -ne 0 ] 
then
 echo "Fehler: konnre das Programm nicht compilieren/linken"
 exit 2
fi

echo "lösche temporäre Dateien"
echo "--------------------------------------"
for i in $( ls *.o  )
do 
  rm $i
done
for i in $( ls moc*  )
do 
  rm $i
done
rm Makefile
rm arts/moc*
rm arts_plugin/moc*

echo Installation erfolgreich
echo ----------------------------------------
}


# Ende Subroutinen
##########################################################

$path
cd bricks
workdir=$PWD

echo "Erzeuge bricks.pro - file"
echo "---------------------------------"

cat >bricks.pro<<ENDCAT
# bricks.pro - generiert von bash
##################################
TEMPLATE = app
TARGET = bricks

INCLUDEPATH += . ./german ./arts ./arts_plugin $ARTSINCLUDE
LIBS        += -lsoundserver_idl -L$KDELIBS

CONFIG += qt

# Input
HEADERS +=  german/lang.h config.h game.h gamelib.h levels.h window.h arts/lala.h arts_plugin/arts.h
SOURCES +=  main.cpp game.cpp gamelib.cpp window.cpp levels.cpp arts/lala.cpp arts_plugin/arts.cpp
ENDCAT

echo "bricks.pro wurde erstellt"
echo "-------------------------"
echo


echo
echo "Erzeuge Makefile "
echo "---------------------------------------------------"

qmake bricks.pro
ret=$?
if [ $ret -ne 0 ]
then
 echo "Fehler: Konnte Makefile nicht generieren! (QTDIR und aRts-Libs vorhanden?)"
 exit 1
fi
mach_hin

echo "Installation abgeschlossen"
echo "Um das Programm zu starten, zum Directory bricks wechseln, ./bricks aufrufen"
echo "============================================================================"
exit 0

####################################################################################
