/***************************************************************************\
* keybset.c - this is the VIO part of the ShiftLock project   950320
*
* Copyright (c) 1995 Martin Lafaix. All Rights Reserved.
\***************************************************************************/

#define INCL_DOSFILEMGR
#define INCL_DOSDEVICES
#define INCL_DOSDEVIOCTL
#define INCL_WININPUT

#include <os2.h>
#include <stdio.h>

HFILE hf;
SHIFTSTATE ss;
BYTE KeyState[257];
BOOL bFlag;
APIRET rc;

static VOID _error(const char *pch)
/* 
 * This function prints an error message on stderr, and halts program
 * execution.
 *
 * Input:  pch - the offending command-line parameter.
 * Returns:  1.
 */
{
  fprintf(stderr, "KEYBSET: error - Invalid switch : %s\n", pch);
  exit(1);
}

static VOID _verbose(VOID)
/* 
 * This function prints program informations on stdout.
 *
 * Returns:  nothing.
 */
{
  puts("Operating System/2  Keyboard Flags Modifier");
  puts("Version 0.0.001 Mar 20 1995");
  puts("(C) Copyright Martin Lafaix 1995");
  puts("All rights reserved.\n");

  puts("Usage:  keybset [option[:[ON|OFF]]] ...");
  puts("        /Capslock");
  puts("        /Numlock");
  puts("        /Scrolllock");
}

static VOID _setflag(BOOL bSet, USHORT usvk, USHORT usflag)
/* 
 * This function sets or resets specified flag.
 *
 * We use the KeyState and ss global structures.
 *
 * Input:  bSet - new flag state.
 *         usvk - flag's corresponding virtual key code.
 *         usflag - flag's corresponding value.
 * Returns:  nothing.
 */
{
  if(bSet)
    {
    KeyState[usvk] |= 0x01;
    ss.fsState |= usflag;
    }
  else
    {
    KeyState[usvk] &= ~0x01;
    ss.fsState &= ~usflag;
    }
}

static VOID _readstate(VOID)
/* 
 * This function reads the current keyboard state, and stores the results
 * in the KeyState and ss global structures.
 *
 * We use the hf global variable, as well as the KeyState and ss global
 * structures.
 *
 * Returns:  nothing.
 */
{
  ULONG ulAction, ulLength;
 
  rc = DosOpen("KBD$",
               &hf,
               &ulAction,
               0L,
               0,
               FILE_OPEN,
               OPEN_ACCESS_READONLY|OPEN_SHARE_DENYNONE,
               0);
 
  ulAction = 0;
  ulLength = sizeof(ss);
  rc = DosDevIOCtl(hf,
                   IOCTL_KEYBOARD,
                   KBD_GETSHIFTSTATE,
                   0,
                   0,
                   &ulAction,
                   &ss,
                   sizeof(ss),
                   &ulLength);

  WinSetKeyboardStateTable(HWND_DESKTOP, &KeyState[1], FALSE);
}

static VOID _writestate(VOID)
/* 
 * This function sets the keyboard state as described by the KeyState
 * and ss global structures.
 *
 * We use the hf global variable as well as the KeyState and ss global
 * structures.
 *
 * Returns:  nothing.
 */
{
  ULONG ulAction, ulLength;
  unsigned short w;
 
  WinSetKeyboardStateTable(HWND_DESKTOP, &KeyState[1], TRUE);

  ulAction = sizeof(ss);
  ulLength = 0;
  rc = DosDevIOCtl(hf,
                   IOCTL_KEYBOARD,
                   KBD_SETSHIFTSTATE,
                   &ss,
                   sizeof(ss),
                   &ulAction,
                   0,
                   0,
                   &ulLength);

  w  = (ss.fsState & (CAPSLOCK_ON|NUMLOCK_ON|SCROLLLOCK_ON)) >> 4;
 
  ulAction = sizeof(w);
  rc = DosDevIOCtl(hf,
                   IOCTL_KEYBOARD,
                   0x5A, /* KBD_ALTERKEYBOARDLED */
                   &w,
                   sizeof(w),
                   &ulAction,
                   0,
                   0,
                   &ulLength);

  rc = DosClose(hf);
}

static BOOL _is(const char *pchOpt, const char *achName)
/* 
 * This function compares pchOpt with achName.  If pchOpt is an abbreviation
 * of achName, it returns TRUE.
 *
 * Side effect:  bFlag is set if pchOpt ends with "", ":" or ":on".  It is
 *               cleared otherwise.
 *
 * Input:  pchOpt - the parameter to examine.
 *         achName - the reference value.
 * Returns:  TRUE if pchOpt is an abbreviation of achName, FALSE otherwise.
 */
{
  BOOL bRet = TRUE;
 
  bFlag = TRUE;

  if((pchOpt[0] == '-' || pchOpt[0] == '/') && pchOpt[1])
    {
    while(*++pchOpt && *achName && (*pchOpt|32) == *achName++);
 
    if(*pchOpt)
      if(*pchOpt == ':')
        bFlag = (pchOpt[1] == 0) || (((pchOpt[1]|32) == 'o') && ((pchOpt[2]|32) == 'n'));
      else
        bRet = FALSE;
    else
      bRet = TRUE;
    }
  else
    bRet = FALSE;

  return bRet;
}

int main(int argc, char *argv[], char *envp[])
{
  USHORT w;

  if(argc == 1 || (argc == 2 && argv[1][0] == '/' && argv[1][1] == '?'))
    {
    _verbose();
    return 0;
    }

  _readstate();

  for(w = 1; w < argc; w++)
    if(_is(argv[w], "numlock"))
      _setflag(bFlag, VK_NUMLOCK+1, NUMLOCK_ON);
    else
    if(_is(argv[w], "capslock"))
      _setflag(bFlag, VK_CAPSLOCK+1, CAPSLOCK_ON);
    else
    if(_is(argv[w], "scrolllock"))
      _setflag(bFlag, VK_SCRLLOCK+1, SCROLLLOCK_ON);
    else
      _error(argv[w]);

  _writestate();
 
  return 0;
}
