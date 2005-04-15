#!/bin/sh
##########################################################
# Installer ujagames_qt: english nas / no -  Soundserver #
##########################################################


###################################################################################################################

# Programkopf: 
################
echo "----------------------------------------------"
echo "Installation master english"
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

echo "Installation done"
echo "----------------------------------------"
}


# Ende Subroutinen
##########################################################

$path
cd mastercode
workdir=$PWD

echo
echo "Generating Makefile "
echo "---------------------------------------------------"

qmake master_engl.pro
ret=$?
if [ $ret -ne 0 ]
then
 echo "Error: Could not generate Makefile! (QTDIR available?)"
 exit 1
fi
mach_hin

echo "Installation completed"
echo "To start the game, point to mastercode, call: ./master"
echo "============================================================================"
exit 0

####################################################################################
