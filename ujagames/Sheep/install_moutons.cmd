#!/bin/sh
###########################################
# Installer ujagames_qt: schafe, englisch #
###########################################
echo --------------------------------------
echo Installation PetitsMoutons
echo --------------------------------------
echo Step 1: Generater Makefile 
$path
cd schafe
workdir=$PWD

qmake moutons.pro
ret=$?
if [ $ret -ne 0 ]
then
 echo Error: Could not generate Makefile, check if QTDIR is available!
 exit 1
fi

echo Step 2: Compiling, Linking
echo --------------------------------------
make
ret=$?
if [ $ret -ne 0 ] 
then
 echo Error: Could not compile/link moutons
 exit 2
fi

echo Step 3: Removing temp files
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

echo "Installation successful"
echo ---------------------------------------------------------
exit 0
