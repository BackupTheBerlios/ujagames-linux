#!/bin/sh
##########################################################
# Installer ujagames_qt: bricks deutsch NAS/kein -Soundserver #
##########################################################

echo --------------------------------------
echo "Installation Bricks deutsch, NAS ode kein Sound"
echo --------------------------------------

#####################################################################
# Subroutinen:
# compiling, linking:
mach_hin()
{
echo Compilieren, Linken
echo --------------------------------------
make
ret=$?
if [ $ret -ne 0 ] 
then
 echo Fehler: konnre das Programm nicht compilieren/linken
 exit 2
fi

echo "lösche temporäre Dateien"
echo --------------------------------------
for i in $( ls *.o  )
do 
  rm $i
done
for i in $( ls moc*  )
do 
  rm $i
done
rm Makefile
rm nas/moc*

echo Installation erfolgreich
echo ----------------------------------------
}


# Ende Subroutinen
####################

$path
cd bricks
workdir=$PWD

echo
echo "Erzeuge Makefile "
echo "---------------------------------------------------"

qmake bricks_nas_deu.pro
ret=$?
if [ $ret -ne 0 ]
then
 echo "Fehler: Konnte Makefile nicht generieren! (QTDIR vorhanden?)"
 exit 1
fi
mach_hin

echo "Installation abgeschlossen"
echo "Um das Programm zu starten, zum Directory bricks wechseln, ./bricks aufrufen"
echo "============================================================================"
exit 0

####################################################################################


