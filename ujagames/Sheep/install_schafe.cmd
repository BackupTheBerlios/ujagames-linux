#!/bin/sh
###########################################
# Installer ujagames_qt: schafe, englisch #
###########################################
echo --------------------------------------
echo "Installation Lämmerlinge 1.6"
echo --------------------------------------
$path
cd schafe
workdir=$PWD

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

echo Step 1: "Erzeugen des Makefile für Lämmerlinge"
echo -------------------------------------------

qmake schafe.pro
ret=$?
if [ $ret -ne 0 ]
then
 echo "Fehler: Konnte Makefile nicht generieren! (QTDIR vorhanden?)"
 exit 1
fi
mach_hin

echo "Step 2: Erzeugen des  Makefile für den Editor"
echo -------------------------------------------
qmake editor.pro
ret=$?
if [ $ret -ne 0 ]
then
 echo "Fehler: Konnte Makefile nicht generieren!"
 exit 1
fi
mach_hin

echo "Installation abgeschlossen"
echo ---------------------------------------------------------
exit 0
