#!/bin/sh
##########################################################
# Installer ujagames_qt: spooky english NAS-Soundserver #
##########################################################

echo --------------------------------------
echo "Installation SNeuner english"
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
rm Makefile

echo "... done!"
echo "----------------------------------------"
}


# Ende Subroutinen
####################

$path
cd neuner
workdir=$PWD


echo
echo "Step 2: Generating  Makefile"
echo "---------------------------------------------------"

qmake neuner_engl.pro
ret=$?
if [ $ret -ne 0 ]
then
 echo "Error: Could not generate  Makefile! (QTDIR available?)"
 exit 1
fi
mach_hin

echo "Installation succesful"
echo "To start change to directory neuner, type ./neuner"
echo "=================================================="
exit 0

####################################################################################
