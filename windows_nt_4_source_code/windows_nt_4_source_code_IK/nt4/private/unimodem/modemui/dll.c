//---------------------------------------------------------------------------
//
// Copyright (c) Microsoft Corporation 1993-1995
//
// File: dll.c
//
//  This file contains the library entry points 
//
// History:
//  12-23-93 ScottH     Created
//   9-22-95 ScottH     Ported to NT
//
//---------------------------------------------------------------------------


#include "proj.h"         
#include <rovdbg.h>         // debug assertion code

// Global data
//
int g_cProcesses = 0;
BOOL g_bAdminUser;


#ifdef WIN32

CRITICAL_SECTION g_csDll = { 0 };

#endif  // WIN32


/*----------------------------------------------------------
Purpose: Initialize the DLL
Returns: 
Cond:    --
*/
BOOL PRIVATE Dll_Initialize(void)
    {
    BOOL bRet = TRUE;

    InitCommonControls();

    return bRet;
    }


/*----------------------------------------------------------
Purpose: Register window classes per process
Returns: 
Cond:    --
*/
BOOL PRIVATE InitWindowClasses(
    HINSTANCE hinst)
    {
    return TRUE;
    }


/*----------------------------------------------------------
Purpose: Terminate DLL
Returns: --
Cond:    --
*/
BOOL PRIVATE Dll_Terminate(
    HINSTANCE hinst)
    {
    return TRUE;
    }


/*----------------------------------------------------------
Purpose: Unregister window classes per process
Returns: 
Cond:    --
*/
void PRIVATE TermWindowClasses(
    HINSTANCE hinst)
    {
    }


/*----------------------------------------------------------
Purpose: Attach a process to this DLL
Returns: --
Cond:    --
*/
BOOL PRIVATE Dll_ProcessAttach(HINSTANCE hDll)
    {
    BOOL bSuccess = TRUE;

#ifdef WIN32

    if (g_cProcesses == 0)
        {
        InitializeCriticalSection(&g_csDll);
#ifndef WIN95
        g_bAdminUser = IsAdminUser();
#endif  // WIN95
        }

#endif  // WIN32

    if (bSuccess)
        {
            g_hinst = hDll;

#ifdef DEBUG

        // We do this simply to load the debug .ini flags
        //
        RovComm_ProcessIniFile();

        TRACE_MSG(TF_GENERAL, "Process Attach [%d] (hDll = %lx)", g_cProcesses, hDll);
        DEBUG_BREAK(BF_ONPROCESSATT);

#endif

        if (g_cProcesses++ == 0)
            {
            bSuccess = Dll_Initialize();
            }

        if (bSuccess)
            {
            // (Only do this if we succeeded above)
            //
            // Register the window classes again (they are per-process)
            //
            bSuccess = InitWindowClasses(hDll);
            }
        }

    return bSuccess;
    }


/*----------------------------------------------------------
Purpose: Detach a process from the DLL
Returns: --
Cond:    --
*/
BOOL PRIVATE Dll_ProcessDetach(HINSTANCE hDll)
    {
    BOOL bSuccess = TRUE;

    ASSERT(hDll == g_hinst);

    DEBUG_CODE( TRACE_MSG(TF_GENERAL, "Process Detach [%d] (hDll = %lx)", 
        g_cProcesses-1, hDll); )

    DEBUG_CODE( DEBUG_BREAK(BF_ONPROCESSDET); )

    if (--g_cProcesses == 0)
        {
        bSuccess = Dll_Terminate(g_hinst);
        }

#ifdef WIN32

    if (g_cProcesses == 0)
        {
        DeleteCriticalSection(&g_csDll);
        }

#endif  // WIN32

    TermWindowClasses(hDll);

    return bSuccess;
    }


//
// Per-instance data
//
#pragma data_seg(DATASEG_PERINSTANCE)

HINSTANCE g_hinst = 0;

#pragma data_seg()


// **************************************************************************
// WIN32 specific code
// **************************************************************************

#ifdef WIN32

#ifdef DEBUG
BOOL g_bExclusive=FALSE;
#endif


/*----------------------------------------------------------
Purpose: Enter an exclusive section
Returns: --
Cond:    --
*/
void PUBLIC Dll_EnterExclusive(void)
    {
    EnterCriticalSection(&g_csDll);

#ifdef DEBUG
    g_bExclusive = TRUE;
#endif
    }


/*----------------------------------------------------------
Purpose: Leave an exclusive section
Returns: --
Cond:    --
*/
void PUBLIC Dll_LeaveExclusive(void)
    {
#ifdef DEBUG
    g_bExclusive = FALSE;
#endif

    LeaveCriticalSection(&g_csDll);
    }


/*----------------------------------------------------------
Purpose: Win32 Libmain
Returns: --
Cond:    --
*/
BOOL APIENTRY LibMain(
    HANDLE hDll, 
    DWORD dwReason,  
    LPVOID lpReserved)
    {
    switch(dwReason)
        {
    case DLL_PROCESS_ATTACH:
        Dll_ProcessAttach(hDll);
        break;

    case DLL_PROCESS_DETACH:
        Dll_ProcessDetach(hDll);
        break;

    case DLL_THREAD_ATTACH:

#ifdef DEBUG

        ENTER_X()
            {
            // We do this simply to load the debug .ini flags
            //
            RovComm_ProcessIniFile();

            TRACE_MSG(TF_GENERAL, "Thread Attach [%d] (hDll = %lx)", 
                      g_cProcesses, hDll);
            }
        LEAVE_X()

        DEBUG_BREAK(BF_ONTHREADATT);

#endif

        break;

    case DLL_THREAD_DETACH:

#ifdef DEBUG

        ENTER_X()
            {
            TRACE_MSG(TF_GENERAL, "Thread Detach [%d] (hDll = %lx)", 
                g_cProcesses, hDll);
            }
        LEAVE_X()

        DEBUG_BREAK(BF_ONTHREADDET);

#endif

        break;

    default:
        break;
        } 
    
    return TRUE;
    } 


#else   // WIN32


// **************************************************************************
// WIN16 specific code
// **************************************************************************


BOOL CALLBACK LibMain(HINSTANCE hinst, UINT wDS, DWORD unused)
    {
    return Dll_ProcessAttach(hinst);
    }

BOOL CALLBACK WEP(BOOL fSystemExit)
    {
    return Dll_ProcessDetach(g_hinst);
    }

#endif  // WIN32




