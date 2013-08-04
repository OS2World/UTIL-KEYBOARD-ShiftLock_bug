/***************************************************************************\
* slhook.c - this is the DLL part of the ShiftLock project    950319
*
* Copyright (c) 1995 Martin Lafaix. All Rights Reserved.
\***************************************************************************/

#define INCL_WINHOOKS
#define INCL_WININPUT
#define INCL_WINMESSAGEMGR
#define INCL_DOSMODULEMGR

#include <os2.h>

HAB hab;
HMODULE hm;
BYTE KeyState[257];

#define DLL_NAME  "SLHOOK"

static VOID _setcaps(BOOL bSet)
/*
 * This function sets the CAPSLOCK state.
 *
 * Input:  bSet - new CAPSLOCK state.
 * Returns:  nothing.
 */
{
  WinSetKeyboardStateTable(HWND_DESKTOP, &KeyState[1], FALSE);

  if(bSet)
    KeyState[VK_CAPSLOCK+1] |= 0x01;
  else
    KeyState[VK_CAPSLOCK+1] &= ~0x01;

  WinSetKeyboardStateTable(HWND_DESKTOP, &KeyState[1], TRUE);
}

BOOL EXPENTRY SlHook(HAB hab, PQMSG pQmsg, ULONG fs)
/*
 * This is the input hook that is used to detect VK_CAPSLOCK and VK_SHIFT
 * usage.
 *
 * We only have to handle the up-to-down transition.
 *
 * Input:  hab - handle of the anchor block of the application to
 *               receive this message
 *         pQmsg - pointer to the QMSG structure to be passed to the
 *                 application via WinGetMsg/WinPeekMsg
 *         fs - specifies whether or not the message will be removed
 *              from the queue.
 * Returns:  FALSE allways.
 */
{
  if(pQmsg->msg == WM_CHAR && (SHORT1FROMMP(pQmsg->mp1) & (KC_KEYUP|KC_PREVDOWN)) == 0)
    {
    USHORT usvk = SHORT2FROMMP(pQmsg->mp2);
    USHORT ussc = HIBYTE(SHORT2FROMMP(pQmsg->mp1));
 
    if((SHORT1FROMMP(pQmsg->mp1) & KC_SCANCODE) == KC_SCANCODE &&
       (ussc == 0x2A /* L-shift */ || ussc == 0x36 /* R-shift*/ || ussc == 0x3A /* CapsLock */))
      _setcaps(ussc == 0x3A);
    else
    if((SHORT1FROMMP(pQmsg->mp1) & KC_VIRTUALKEY) == KC_VIRTUALKEY &&
       (usvk == VK_SHIFT || usvk == VK_CAPSLOCK))
      _setcaps(usvk == VK_CAPSLOCK);
    }

  return FALSE;
}

BOOL EXPENTRY SLINIT(HAB habCaller)
/*
 * This function initializes the DLL and sets the input hook.
 *
 * Input:  habCaller - handle of the caller anchor block
 * Returns:  TRUE if successful, FALSE otherwise
 */
{
  hab = habCaller;
 
  if(DosQueryModuleHandle(DLL_NAME,&hm))
    return FALSE;
 
  WinSetHook(hab, NULLHANDLE, HK_INPUT, (PFN)SlHook, hm);
 
  return TRUE;
}

BOOL EXPENTRY SLTERM(VOID)
/*
 * This function releases the input hook and broadcasts a WM_NULL message
 * to all top level windows so that they will release the DLL.  If we don't
 * do this, the DLL will remain locked and we'll have to reboot in order to
 * recompile.
 *
 * Returns:  TRUE always
 */
{
  WinReleaseHook(hab, NULLHANDLE, HK_INPUT, (PFN)SlHook, hm);

  WinBroadcastMsg(HWND_DESKTOP,WM_NULL,0,0,BMSG_FRAMEONLY|BMSG_POST);

  return TRUE;
}
