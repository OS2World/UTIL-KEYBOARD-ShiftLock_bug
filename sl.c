/***************************************************************************\
* sl.c - this is the pm part of the ShiftLock project         950319
*
* Copyright (c) 1995 Martin Lafaix. All Rights Reserved.
\***************************************************************************/

#define INCL_WIN

#include <os2.h>

BOOL EXPENTRY SLINIT(HAB habCaller);
BOOL EXPENTRY SLTERM(VOID);

int main(int argc, char *argv[], char *envp[])
/*
 * This is the main function.  It initializes the input hook, and then
 * waits for a WM_CLOSE message to remove it.
 *
 * Returns:  0.
 */
{
  HAB hab = WinInitialize(0);
  HMQ hmq = WinCreateMsgQueue(hab, 0);
  QMSG qmsg;
  ULONG flCreate = FCF_TITLEBAR | FCF_TASKLIST;
  HWND hwndFrame;
  HWND hwnd;

  WinRegisterClass(hab, "MLC_SHIFTLOCK", (PFNWP)WinDefWindowProc, 0, 0);
 
  hwndFrame = WinCreateStdWindow(HWND_DESKTOP, 0,
                                 &flCreate, "MLC_SHIFTLOCK",
                                 "MLShiftLock", 0,
                                 (HMODULE)0, 4321,
                                 &hwnd);

  SLINIT(hab);
   
  while(WinGetMsg(hab, &qmsg, 0, 0, 0))
    WinDispatchMsg(hab, &qmsg);
 
  SLTERM();
 
  WinDestroyWindow(hwndFrame);

  WinDestroyMsgQueue(hmq);
  WinTerminate(hab);
 
  return 0;
}
