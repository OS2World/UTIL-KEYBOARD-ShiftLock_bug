/* ShiftLock bug fix v0.5 (950329) */

What's That?
------------

This package takes care of OS/2 Warp's buggy ShiftLock handling.  It also
allows you to modify the initial keyboard state (Capslock, Numlock and
Scrolllock).

What's New?
-----------

o An INSTALL.CMD script as been added.

Contents?
---------

This package contains the following files:

   build.cmd   - a batch file used to build sl and slhook;
   install.cmd - an install script;
   keybset.c   - keybset source code;
   keybset.exe - keybset executable;
   readme      - this file;
   sl.c        - sl source code;
   sl.def      - sl definition file;
   sl.exe      - sl executable;
   slhook.c    - slhook source code;
   slhook.def  - slhook definition file;
   slhook.dll  - slhook dynamic load library.
 
How to Install?
---------------

You can use the INSTALL.CMD script, or you can install it manualy:

Unzip SHIFTBUG.ZIP in a directory, and start SL.EXE (by issuing the
"START SL" command in an OS/2 windowed shell).

If you want to use it automatically, you can create a program object for
SL.EXE, and put it (or, a shadow of it) in your STARTUP folder.

When SL.EXE is running, there's a "MLShiftLock" entry in the Tasks list.

If you want to modify keyboard initial state, you can insert the
desired KEYBSEY.EXE command in your STARTUP.CMD file.  For example,

   KEYBSET /NUMLOCK:ON

If you're not interested by source code, you can remove it by issuing:

   DEL /F *.C *.DEF BUILD.CMD

How to Uninstall?
-----------------

If SL.EXE is running, close it (that is, open the Tasks List, select the
"MLShiftLock" entry, open its context menu, and select Close).

You can then remove the ShiftLock object from your startup folder, SL.EXE
and SLHOOK.DLL...


Hope you find it useful...


Martin Lafaix
--
lafaix@mimosa.unice.fr
Team OS/2
