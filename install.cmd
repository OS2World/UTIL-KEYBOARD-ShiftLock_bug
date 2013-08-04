/***************************************************************************\
* install.cmd - this installs the ShiftLock fix               950329
*
* Copyright (c) 1995 Martin Lafaix. All Rights Reserved.
\***************************************************************************/

'@cls'; signal on halt; trace off

/* Load the RexxUtil functions */
Call RxFuncAdd 'SysLoadFuncs', 'REXXUTIL', 'SysLoadFuncs'
Call SysLoadFuncs

/* Initialisation */
exename = SysSearchPath("PATH","SL.EXE")
startupdir = SysSearchPath("PATH", "SLHOOK.DLL")

say 'This script will install a Program Object for ShiftLock in your'
say 'Startup folder.'
say
say '(Use Ctrl+C to stop the installation.)'
say
say 'Source path of ShiftLock (SL.EXE) :' exename

/* Read the SL.EXE path */
if isnotcorrect() then exename = getname("SL.EXE")
if translate(right(exename,6)) \= "SL.EXE" then
   if right(exename,1) = "\" then
      exename = exename"SL.EXE"
   else
      exename = exexname"\SL.EXE"

say
say 'Source path of the ShiftLock DLL (SLHOOK.DLL) :' startupdir

/* Read the SLHOOK path */
if isnotcorrect() then startupdir = getname("SLHOOK.DLL")
if translate(right(startupdir,10)) = "SLHOOK.DLL" then
   startupdir = left(startupdir, length(startupdir)-10)
if right(startupdir,1) = "\" then
   startupdir = left(startupdir,length(startupdir)-1)

/* Create the program object */
say
say 'Creating a program object:'
say
say '          Class = WPProgram'
say '           Name = ShiftLock'
say '    Destination = <WP_START>'
say '     Parameters = PROGTYPE=PM;EXENAME='exename';STARTUPDIR='startupdir
say '         Option = REPLACE'
say

if \isnotcorrect() & makeobj() then
   say 'Installation successful!'
else
   say 'Cannot create Program Object!  Installation failed!'
 
exit

isnotcorrect:
   call charout ,"Is this correct ? [Y/n] :"
   key = ''
   do until pos(key,"yYnN"||"0d"x) > 0
      key = SysGetKey("ECHO")
      call charout ,"082008"x
   end /* do */
   say key
   return key = "n" | key = "N"

getname:
   say "Please enter the complete path for" arg(1)
   do forever
      pull name
      if translate(right(name,length(arg(1)))) \= arg(1) then
         if right(name,1) = "\" then
            name = name||arg(1)
         else
            name = name"\"arg(1)
      if stream(name,"c","query exists") = '' then
         say "The path '"name"' is invalid!  Please enter it again."
      else
         leave
   end
   return name
 
makeobj:
   return SysCreateObject("WPProgram", ,
                          "ShiftLock2", ,
                          "<WP_START>", ,
                          "PROGTYPE=PM;EXENAME="exename";STARTUPDIR="||startupdir, ,
                          "REPLACE")
 
halt:
   say 'Installation aborted!'
   exit
