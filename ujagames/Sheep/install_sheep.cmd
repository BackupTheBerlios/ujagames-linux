#!/bin/sh
###########################################
# Installer ujagames_qt: schafe, englisch #
###########################################
echo "-------------------------------------------"
echo "Installation LittleSheep 1.6" 
echo "-------------------------------------------"
$path
cd schafe
workdir=$PWD

# compiling, linking:
mach_hin()
{
echo Compiling, Linking
echo --------------------------------------
make
ret=$?
if [ $ret -ne 0 ] 
then
 echo Error: Could not compile/link program
 exit 2
fi

echo Removing temp files
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

echo Installation successful
echo ----------------------------------------
}

echo Step 1: "Generating Makefile for LittleSheep"
echo -------------------------------------------

qmake sheep.pro
ret=$?
if [ $ret -ne 0 ]
then
 echo Error: Could not generate Makefile, check if QTDIR is available!
 exit 1
fi
mach_hin

echo "Step 2: Generating Makefile for leveleditor"
echo -------------------------------------------
qmake leveleditor.pro
ret=$?
if [ $ret -ne 0 ]
then
 echo Error: Could not generate Makefile!
 exit 1
fi
mach_hin

echo "Installation complete"
echo ---------------------------------------------------------
exit 0
