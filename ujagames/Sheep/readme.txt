------------------------------------------------
Installation ujagames_qt: schafe/sheep V.1.6
------------------------------------------------

New in 1.6:
------------
different speeds
better immunity for sheep (cyclams) when moskitoflight covers whole meadow 
more than 12 transporters available

New in V.1.5:
-----------------
1. Leveleditor for userlevels, 5 levels provided
2. Sheep are now immune to moskito when wet
3. Switch between supplied levels and userlevels
   (speicher.ini is no longer downwards compatible 
    due to a further entry, see Updating)
4. immediate score update after loss of points
5. more space available for messages with bigger fonts
6. bash files for installation


Updating from V.1.3 to 1.5
--------------------------
1. In schafe backup your *.ini files
2. Install as described
3. Move your ini-files back
4. Open speicher.ini with a text editor,
   insert 0; (zero semicolon) at the start of every storage entry
   (1st line: storage names, 2nd and following lines: storage entries)
5. save and exit


Installation:
--------------------
1. Unpack the archive into a directory of your choice.
2. Open a command line interface (console), 
   change to this directory, type
   ./install_sheep.cmd  (english version)
   ./install_schafe.cmd (german version)
   wait until compilation and linking has finished.
   You may ignore warnings.
   
   Note! The french version mouton only installs the game
   because my French is not good enough for a further language file.

3. Start game from the CLI by changing to the directory schafe, type ./sheep,
   Start leveleditor by changing to the directory schafe, type ./leveleditor,
   or start programs from a directory manager by (double-) clicking,
   or make a link/icon on the desktop pointing to these programs,
   work directory: the directory the programs are is in.

   
Configuration:
----------------------------
If you want to play only on your local machine, you may skip steps 1 and 2.

1. Open the file hiscore.ini

2. Set the path in the second path entry to your network path.
   You need Read/Write permission for this path.
   You may switch the actual List via Menu2.
   
3. After starting the game reset every hiscore list via Menu.
----------------------------------------------------------------------------

Userlevels:
--------------------
Interna:
The userlevels are stored in userlevel.csv which is a simple textfile representing a data table.

You may add userlevels from your friends by copying the appropriate lines to your file userlevel.csv.
The lines must follow immediately, i.e. without an empty line between them. It does not harm since V.1.6,
but it's a waste of ressources.
(Any line that has more than 192 charactest is treated as a levelcode)
-----------------------------------------------------------------------------------------

 