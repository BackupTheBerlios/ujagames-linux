#!/bin/sh
##########################################################
# Installer ujagames_qt: bricks english nass-Soundserver #
##########################################################


###################################################################################################################

# Programkopf: 
################
echo "----------------------------------------------"
echo "Installation Bricks NAS english"
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
rm nas/moc*

echo "Installation done"
echo "----------------------------------------"
}


# Ende Subroutinen
##########################################################

$path
cd bricks
workdir=$PWD

echo
echo "Generating Makefile "
echo "---------------------------------------------------"

qmake bricks_nas_engl.pro
ret=$?
if [ $ret -ne 0 ]
then
 echo "Error: Could not generate Makefile! (QTDIR available?)"
 exit 1
fi
mach_hin

echo "Installation completed"
echo "To start the game, point to bricks, call: ./bricks"
echo "============================================================================"
exit 0

####################################################################################
