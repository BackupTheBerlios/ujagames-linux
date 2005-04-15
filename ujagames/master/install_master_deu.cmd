#!/bin/sh
##########################################################
# Installer ujagames_qt: deutsch NAS / kein -Soundserver #
##########################################################

echo --------------------------------------
echo "Installation master deutsch,"
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

echo Installation erfolgreich
echo ----------------------------------------
}


# Ende Subroutinen
####################

$path
cd mastercode
workdir=$PWD

echo
echo "Erzeuge Makefile "
echo "---------------------------------------------------"

qmake master_deu.pro
ret=$?
if [ $ret -ne 0 ]
then
 echo "Fehler: Konnte Makefile nicht generieren! (QTDIR vorhanden?)"
 exit 1
fi
mach_hin

echo "Installation abgeschlossen"
echo "Um das Programm zu starten, zum Directory mastercode wechseln, ./master aufrufen"
echo "================================================================================"
exit 0

####################################################################################


