#!/bin/sh
##########################################################
# Installer ujagames_qt: bricks eglish aRts-Soundserver #
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
echo "aRts-Bibliothekspfad $KDELIBS not found! Please enter in $0 or install NAS-Version!"
exit 1
fi

if [ ! -d $ARTSINCLUDE ]
then
echo "aRts-Headerpath $ARTSINCLUDE not found! Please enter in $0 or install NAS-Version!"
exit 1
fi

echo
echo "Soundlibs found $KDELIBS and $ARTSINCLUDE"


###################################################################################################################

# Programkopf: 
################
echo "----------------------------------------------"
echo "Installation Bricks KDE/aRts english"
echo "----------------------------------------------"

######################################################
# Subroutinen:
# compiling, linking:
mach_hin()
{
echo "Compiling, linking"
echo "--------------------------------------"
make
ret=$?
if [ $ret -ne 0 ] 
then
 echo "Error: Could not compile/link program"
 exit 2
fi

echo "Removing temporary files"
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

echo "Installation done"
echo "----------------------------------------"
}


# Ende Subroutinen
##########################################################

$path
cd bricks
workdir=$PWD

echo "Generating bricks.pro - file"
echo "---------------------------------"

cat >bricks.pro<<ENDCAT
# bricks.pro - generiert von bash
##################################
TEMPLATE = app
TARGET = bricks

INCLUDEPATH += . ./english ./arts ./arts_plugin $ARTSINCLUDE
LIBS        += -lsoundserver_idl -L$KDELIBS

CONFIG += qt

# Input
HEADERS +=  english/lang.h config.h game.h gamelib.h levels.h window.h arts/lala.h arts_plugin/arts.h
SOURCES +=  main.cpp game.cpp gamelib.cpp window.cpp levels.cpp arts/lala.cpp arts_plugin/arts.cpp
ENDCAT

echo "bricks.pro - done"
echo "-------------------------"
echo


echo
echo "Generating Makefile "
echo "---------------------------------------------------"

qmake bricks.pro
ret=$?
if [ $ret -ne 0 ]
then
 echo "Error: Could not generate Makefile! (QTDIR and aRts-Libs available?)"
 exit 1
fi
mach_hin

echo "Installation completed"
echo "To start the game, point to bricks, call: ./bricks"
echo "============================================================================"
exit 0

####################################################################################
