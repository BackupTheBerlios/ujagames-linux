#!/bin/sh
##########################################################
# Installer ujagames_qt: spooky english aRts-Soundserver #
##########################################################

# Bibliotheken finden: 
########################
KDEPFAD=""
KDEPFAD="$(kde-config --prefix)"

KDELIBS="$(find $KDEPFAD|grep libartsc.so$)"
KDELIBS="$(echo $KDELIBS|sed 's/libartsc.so//')"

ARTSINCLUDE="$(find $KDEPFAD|grep soundserver.h$)"
ARTSINCLUDE="$(echo $ARTSINCLUDE|sed 's/soundserver.h//')"

# enteryour own path if not foud by routine:
# KDELIBS=/kderoot kdeversion/lib
# ARTSINCLUDE=/kderoot kdeversion/include/arts

if [ ! -d $KDELIBS ]
then
echo "aRts-libpath $KDELIBS not found!Please enter in $0, or install NAS version!"
exit 1
fi

if [ ! -d $ARTSINCLUDE ]
then
echo "aRts-headerpath $ARTSINCLUDE not found!Please enter in $0, or install NAS version!"
exit 1
fi

echo
echo "Soundlibs found in $KDELIBS and $ARTSINCLUDE"


echo --------------------------------------
echo "Installation Spooky Shooter english"
echo --------------------------------------

#####################################################################
# Subroutinen:
# compiling, linking:
mach_hin()
{
echo "Compiling, Linking..."
echo "--------------------------------------"
make
ret=$?
if [ $ret -ne 0 ] 
then
 echo "Error: Could not compile/link program!"
 exit 2
fi

echo "deleting temporary files..."
echo "--------------------------------------"
for i in $( ls *.o  )
do 
  rm $i
done
for i in $( ls moc*  )
do 
  rm $i
done
rm ./arts/moc*
rm Makefile

echo "... done!"
echo "----------------------------------------"
}


# Ende Subroutinen
####################

$path
cd spooky
workdir=$PWD

echo " Step 1: Generating .pro file:"
echo "---------------------------------"

cat >spooky.pro<<ENDCAT
# spooky.pro - generiert von bash
##################################
TEMPLATE = app
CONFIG += qt
TARGET = spooky

INCLUDEPATH += . ./english ./arts ./arts_plugin $ARTSINCLUDE
LIBS        += -lsoundserver_idl -L$KDELIBS

# Input
HEADERS +=  english/lang.h config.h gamelib.h window.h wiese.h arts/lala.h arts_plugin/arts.h
SOURCES +=  main.cpp gamelib.cpp window.cpp wiese.cpp arts/lala.cpp arts_plugin/arts.cpp

ENDCAT

echo "*.pro file done"
echo "-------------------------"
echo


echo
echo "Step 2: Generating  Makefile"
echo "---------------------------------------------------"

qmake spooky.pro
ret=$?
if [ $ret -ne 0 ]
then
 echo "Error: Could not generate  Makefile! (QTDIR and aRts-Libs available?)"
 exit 1
fi
mach_hin

echo "Installation succesful"
echo "To start change to directory spooky, type ./spooky"
echo "=================================================="
exit 0

####################################################################################
