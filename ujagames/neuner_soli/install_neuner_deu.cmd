#!/bin/sh
##########################################################
# Installer ujagames_qt: spooky deutsch NAS/kein -Soundserver #
##########################################################

echo --------------------------------------
echo "Installation Neuner, deutsch"
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
cd neuner
workdir=$PWD


echo
echo "Step 2: Erzeugen des Makefile "
echo "---------------------------------------------------"

qmake neuner_deu.pro
ret=$?
if [ $ret -ne 0 ]
then
 echo "Fehler: Konnte Makefile nicht generieren! (QTDIR vorhanden?)"
 exit 1
fi
mach_hin

echo "Installation abgeschlossen"
echo "Um das Programm zu starten, zum Directory neuner wechseln, ./neuner aufrufen"
echo "============================================================================"
exit 0

####################################################################################


