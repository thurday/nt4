
/*++

Copyright (c) 1995  Microsoft Corporation

Module Name:

    itlgagl.c

Abstract:

    This module contains the test functions for lineGetAgentGroupList

Author:

	 Xiao Ying Ding (XiaoD)		23-Dec-1995

Revision History:

--*/


#include "windows.h"
#include "malloc.h"
#include "string.h"
#include "tapi.h"
#include "trapper.h"
#include "tcore.h"
#include "ttest.h"
#include "doline.h"
#include "vars.h"
#include "line20.h"

#define NUMTOTALSIZES 5


//  lineGetAgentGroupList
//
//  The following tests are made:
//
//                               Tested                 Notes
//  -------------------------------------------------------------------------
// 1. Go/No-Go test
// 2. Bad hLine
// 3. Bad dwAddressID
// 4. Bad lpAgentGroupList
// 5. Bad dwTotalSize
// 6. Open with no Proxy request
//	
// * = Stand-alone test case
//
//

BOOL TestLineGetAgentGroupList(BOOL fQuietMode, BOOL fStandAlone)
{
    LPTAPILINETESTINFO  lpTapiLineTestInfo;
    INT i, n;
    BOOL fTestPassed                  = TRUE;
    LPDWORD	lpRequestBuf;
    LONG lret;
    LPTAPIMSG lpTapiMsg = NULL;
    LPTAPIMSG lpMatch;
    DWORD dwFixedSize = sizeof(LINEAGENTGROUPLIST);
    DWORD lExpected;
    DWORD dwTotalSizes[NUMTOTALSIZES] = {
                           0,
                           (DWORD) dwFixedSize - 1,
   								0x70000000,
                           0x7FFFFFFF,
                           0xFFFFFFFF
                           };
 
 	 InitTestNumber();

    OutputTAPIDebugInfo(
        DBUG_SHOW_PASS,
        "\n***************************************************************");

    OutputTAPIDebugInfo(
        DBUG_SHOW_PASS,
        ">> Test lineGetAgentGroupList");

    // ===================================================================
    // ===================================================================
    //
    // 1. Test Case: Go/No-Go test for owner.
    //
    // ===================================================================
    // ===================================================================

    TapiLineTestInit();
    lpTapiLineTestInfo = GetLineTestInfo();
    fTestPassed = TRUE;


    //
    // Initialize a line app
    //

    lpTapiLineTestInfo->lpLineInitializeExParams =
         (LPLINEINITIALIZEEXPARAMS) AllocFromTestHeap (
         sizeof(LINEINITIALIZEEXPARAMS));
    lpTapiLineTestInfo->lpLineInitializeExParams->dwTotalSize =
         sizeof(LINEINITIALIZEEXPARAMS);
    lpTapiLineTestInfo->lpLineInitializeExParams->dwOptions =
         LINEINITIALIZEEXOPTION_USEHIDDENWINDOW;
    lpTapiLineTestInfo->lpdwAPIVersion = &lpTapiLineTestInfo->dwAPIVersion;
    lpTapiLineTestInfo->dwAPIVersion = TAPI_VERSION2_0;

    if(! DoLineInitializeEx (lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }

    lpTapiLineTestInfo->dwDeviceID =
        (*(lpTapiLineTestInfo->lpdwNumDevs) == 0 ?
        0 : *(lpTapiLineTestInfo->lpdwNumDevs)-1);

    if(IsUNIMDMLineDevice(lpTapiLineTestInfo))
    {
        TapiLogDetail(
            DBUG_SHOW_DETAIL,
            "## lineGetAgentActivityList unimodem didn't support");
        return fTestPassed;
    }

    lpTapiLineTestInfo->dwAPIVersion = HIGH_APIVERSION;
    lpTapiLineTestInfo->dwExtLowVersion = LOW_APIVERSION;
    lpTapiLineTestInfo->dwExtHighVersion = HIGH_APIVERSION;
    lpTapiLineTestInfo->lpdwExtVersion = &lpTapiLineTestInfo->dwExtVersion;

    //
    // Negotiate the API Ext Version
    //

    if (! DoLineNegotiateExtVersion(lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }

    //
    // Open a line
    //

    lpTapiLineTestInfo->dwMediaModes = LINEMEDIAMODE_DATAMODEM;
    lpTapiLineTestInfo->dwPrivileges = LINECALLPRIVILEGE_OWNER |
													LINEOPENOPTION_PROXY;
    lpTapiLineTestInfo->lpCallParams = (LPLINECALLPARAMS) AllocFromTestHeap (
			sizeof(LINECALLPARAMS) + sizeof(DWORD));
    lpTapiLineTestInfo->lpCallParams->dwTotalSize =
			sizeof(LINECALLPARAMS) + sizeof(DWORD);
    lpTapiLineTestInfo->lpCallParams->dwDevSpecificSize =
						sizeof(DWORD);
    lpTapiLineTestInfo->lpCallParams->dwDevSpecificOffset =
						sizeof(LINECALLPARAMS);
    lpRequestBuf = (LPDWORD) (((LPBYTE)lpTapiLineTestInfo->lpCallParams)  +
                    lpTapiLineTestInfo->lpCallParams->dwDevSpecificOffset);
    *lpRequestBuf = LINEPROXYREQUEST_GETAGENTGROUPLIST;

		
    if (! DoLineOpen(lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }

	//
    // Get the line device capabilities
    //

    TapiLogDetail(DBUG_SHOW_PASS,
                  ">>Test Case %ld: Go/No Go Test",
                  dwTestCase+1);

    lpTapiLineTestInfo->dwAddressID = 0;

    lpTapiLineTestInfo->lpAgentGroupList = (LPLINEAGENTGROUPLIST) AllocFromTestHeap(
            sizeof(LINEAGENTGROUPLIST)
            );
    lpTapiLineTestInfo->lpAgentGroupList->dwTotalSize = sizeof(LINEAGENTGROUPLIST);

    if (! DoLineGetAgentGroupList(lpTapiLineTestInfo, TAPISUCCESS, TRUE))
    {
        TLINE_FAIL();
    }


    lpMatch = (LPTAPIMSG) AllocFromTestHeap (sizeof(TAPIMSG));

    lpMatch->dwMsg = LINE_PROXYREQUEST;
//  lpMatch->dwParam1 = (DWORD) lpProxyBuffer;
    lpMatch->dwParam2 = 0;
    lpMatch->dwParam3 = 0;
    lpMatch->dwFlags = TAPIMSG_DWMSG;

    lret = FindReceivedMsgs(&lpTapiMsg, lpMatch, FALSE);

    TapiLogDetail(DBUG_SHOW_DETAIL,"##lret = %lx", lret);
	
    if(lret == 1)
    {
        lpTapiLineTestInfo->lpProxyRequest = (LPLINEPROXYREQUEST) lpTapiMsg->dwParam1;
        lpTapiLineTestInfo->lpProxyRequest->GetAgentGroupList.GroupList.dwNeededSize =
        lpTapiLineTestInfo->lpProxyRequest->GetAgentGroupList.GroupList.dwTotalSize;
        lpTapiLineTestInfo->lpProxyRequest->GetAgentGroupList.GroupList.dwNumEntries = 2;
        lpTapiLineTestInfo->dwResult = 0;
        TapiShowProxyBuffer(lpTapiLineTestInfo->lpProxyRequest);
	
        if (! DoLineProxyResponse(lpTapiLineTestInfo, TAPISUCCESS))
        {
            TLINE_FAIL();
        }

        AddMessage (LINE_REPLY,
                    0,
                    0,
                    0,
                    lpTapiLineTestInfo->dwResult,
                    0,
                    TAPIMSG_DWMSG | TAPIMSG_DWPARAM2);
        WaitForAllMessages();
    }

    TapiLogDetail (
        DBUG_SHOW_DETAIL,
        "## dwTotalSize = %lx, dwNeededSize = %lx, dwNumEntries = %lx",
        lpTapiLineTestInfo->lpAgentGroupList->dwTotalSize,
        lpTapiLineTestInfo->lpAgentGroupList->dwNeededSize,
        lpTapiLineTestInfo->lpAgentGroupList->dwNumEntries);

    fTestPassed = ShowTestCase(fTestPassed);

    if (! DoLineClose(lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }


    //
    // Shutdown
    //

    if (! DoLineShutdown(lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }

    FreeTestHeap();

    // ===================================================================
    // ===================================================================
    //
    // 2. Test Case: Bad hLine.
    //
    // ===================================================================
    // ===================================================================

    TapiLineTestInit();
    lpTapiLineTestInfo = GetLineTestInfo();
    fTestPassed = TRUE;


    //
    // Initialize a line app
    //

    lpTapiLineTestInfo->lpLineInitializeExParams =
         (LPLINEINITIALIZEEXPARAMS) AllocFromTestHeap (
         sizeof(LINEINITIALIZEEXPARAMS));
    lpTapiLineTestInfo->lpLineInitializeExParams->dwTotalSize =
         sizeof(LINEINITIALIZEEXPARAMS);
    lpTapiLineTestInfo->lpLineInitializeExParams->dwOptions =
         LINEINITIALIZEEXOPTION_USEHIDDENWINDOW;
    lpTapiLineTestInfo->lpdwAPIVersion = &lpTapiLineTestInfo->dwAPIVersion;
    lpTapiLineTestInfo->dwAPIVersion = TAPI_VERSION2_0;

    if(! DoLineInitializeEx (lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }

    lpTapiLineTestInfo->dwDeviceID =
        (*(lpTapiLineTestInfo->lpdwNumDevs) == 0 ?
        0 : *(lpTapiLineTestInfo->lpdwNumDevs)-1);

    if(IsUNIMDMLineDevice(lpTapiLineTestInfo))
    {
        TapiLogDetail(
            DBUG_SHOW_DETAIL,
            "## lineGetAgentActivityList unimodem didn't support");
        return fTestPassed;
    }

    lpTapiLineTestInfo->dwAPIVersion = HIGH_APIVERSION;
    lpTapiLineTestInfo->dwExtLowVersion = LOW_APIVERSION;
    lpTapiLineTestInfo->dwExtHighVersion = HIGH_APIVERSION;
    lpTapiLineTestInfo->lpdwExtVersion = &lpTapiLineTestInfo->dwExtVersion;

    //
    // Negotiate the API Ext Version
    //

    if (! DoLineNegotiateExtVersion(lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }

    //
    // Open a line
    //

    lpTapiLineTestInfo->dwMediaModes = LINEMEDIAMODE_DATAMODEM;
    lpTapiLineTestInfo->dwPrivileges = LINECALLPRIVILEGE_OWNER |
													LINEOPENOPTION_PROXY;
    lpTapiLineTestInfo->lpCallParams = (LPLINECALLPARAMS) AllocFromTestHeap (
			sizeof(LINECALLPARAMS) + sizeof(DWORD));
    lpTapiLineTestInfo->lpCallParams->dwTotalSize =
			sizeof(LINECALLPARAMS) + sizeof(DWORD);
    lpTapiLineTestInfo->lpCallParams->dwDevSpecificSize =
						sizeof(DWORD);
    lpTapiLineTestInfo->lpCallParams->dwDevSpecificOffset =
						sizeof(LINECALLPARAMS);
    lpRequestBuf = (LPDWORD) (((LPBYTE)lpTapiLineTestInfo->lpCallParams)  +
                    lpTapiLineTestInfo->lpCallParams->dwDevSpecificOffset);
    *lpRequestBuf = LINEPROXYREQUEST_GETAGENTGROUPLIST;

		
    if (! DoLineOpen(lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }

	//
    // Get the line device capabilities
    //

    TapiLogDetail(DBUG_SHOW_PASS,
                  ">>Test Case %ld: Bad hLine",
                  dwTestCase+1);

    lpTapiLineTestInfo->dwAddressID = 0;

    lpTapiLineTestInfo->lpAgentGroupList = (LPLINEAGENTGROUPLIST) AllocFromTestHeap(
            sizeof(LINEAGENTGROUPLIST)
            );
    lpTapiLineTestInfo->lpAgentGroupList->dwTotalSize = sizeof(LINEAGENTGROUPLIST);

    //
    // save previous hLine
    //

    lpTapiLineTestInfo->hLine_Orig = lpTapiLineTestInfo->hLine1;

    //
    // Test invalid handles
    //

    for(i = 0;  i < NUMINVALIDHANDLES; i++)
    {
        *lpTapiLineTestInfo->lphLine = (HLINE)gdwInvalidHandles[i];
        if (! DoLineGetAgentGroupList(lpTapiLineTestInfo,
                                      LINEERR_INVALLINEHANDLE,
                                      FALSE))
        {
            TLINE_FAIL();
        }
    }

    //
    // Reset hLine
    //

    lpTapiLineTestInfo->hLine1 = lpTapiLineTestInfo->hLine_Orig;

    fTestPassed = ShowTestCase(fTestPassed);

    if (! DoLineClose(lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }


    //
    // Shutdown
    //

    if (! DoLineShutdown(lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }

    FreeTestHeap();

    // ===================================================================
    // ===================================================================
    //
    // 3. Test Case: Bad dwAddressID.
    //
    // ===================================================================
    // ===================================================================

    TapiLineTestInit();
    lpTapiLineTestInfo = GetLineTestInfo();
    fTestPassed = TRUE;


    //
    // Initialize a line app
    //

    lpTapiLineTestInfo->lpLineInitializeExParams =
         (LPLINEINITIALIZEEXPARAMS) AllocFromTestHeap (
         sizeof(LINEINITIALIZEEXPARAMS));
    lpTapiLineTestInfo->lpLineInitializeExParams->dwTotalSize =
         sizeof(LINEINITIALIZEEXPARAMS);
    lpTapiLineTestInfo->lpLineInitializeExParams->dwOptions =
         LINEINITIALIZEEXOPTION_USEHIDDENWINDOW;
    lpTapiLineTestInfo->lpdwAPIVersion = &lpTapiLineTestInfo->dwAPIVersion;
    lpTapiLineTestInfo->dwAPIVersion = TAPI_VERSION2_0;

    if(! DoLineInitializeEx (lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }

    lpTapiLineTestInfo->dwDeviceID =
        (*(lpTapiLineTestInfo->lpdwNumDevs) == 0 ?
        0 : *(lpTapiLineTestInfo->lpdwNumDevs)-1);

    if(IsUNIMDMLineDevice(lpTapiLineTestInfo))
    {
        TapiLogDetail(
            DBUG_SHOW_DETAIL,
            "## lineGetAgentActivityList unimodem didn't support");
        return fTestPassed;
    }

    lpTapiLineTestInfo->dwAPIVersion = HIGH_APIVERSION;
    lpTapiLineTestInfo->dwExtLowVersion = LOW_APIVERSION;
    lpTapiLineTestInfo->dwExtHighVersion = HIGH_APIVERSION;
    lpTapiLineTestInfo->lpdwExtVersion = &lpTapiLineTestInfo->dwExtVersion;

    //
    // Negotiate the API Ext Version
    //

    if (! DoLineNegotiateExtVersion(lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }

    //
    // Get lineDevCaps
    //

    lpTapiLineTestInfo->lpLineDevCaps = (LPLINEDEVCAPS) AllocFromTestHeap(
            sizeof(LINEDEVCAPS));
    lpTapiLineTestInfo->lpLineDevCaps->dwTotalSize = sizeof(LINEDEVCAPS);
    if (! DoLineGetDevCaps(lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }

    //
    // Open a line
    //

    lpTapiLineTestInfo->dwMediaModes = LINEMEDIAMODE_DATAMODEM;
    lpTapiLineTestInfo->dwPrivileges = LINECALLPRIVILEGE_OWNER |
													LINEOPENOPTION_PROXY;
    lpTapiLineTestInfo->lpCallParams = (LPLINECALLPARAMS) AllocFromTestHeap (
			sizeof(LINECALLPARAMS) + sizeof(DWORD));
    lpTapiLineTestInfo->lpCallParams->dwTotalSize =
			sizeof(LINECALLPARAMS) + sizeof(DWORD);
    lpTapiLineTestInfo->lpCallParams->dwDevSpecificSize =
						sizeof(DWORD);
    lpTapiLineTestInfo->lpCallParams->dwDevSpecificOffset =
						sizeof(LINECALLPARAMS);
    lpRequestBuf = (LPDWORD) (((LPBYTE)lpTapiLineTestInfo->lpCallParams)  +
                    lpTapiLineTestInfo->lpCallParams->dwDevSpecificOffset);
    *lpRequestBuf = LINEPROXYREQUEST_GETAGENTGROUPLIST;

		
    if (! DoLineOpen(lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }

	//
    // Get the line device capabilities
    //

    lpTapiLineTestInfo->lpAgentGroupList = (LPLINEAGENTGROUPLIST) AllocFromTestHeap(
            sizeof(LINEAGENTGROUPLIST)
            );
    lpTapiLineTestInfo->lpAgentGroupList->dwTotalSize = sizeof(LINEAGENTGROUPLIST);

    TapiLogDetail(DBUG_SHOW_PASS,
                  ">>Test Case %ld: Bad dwAddressID=-1",
                  dwTestCase+1);

    lpTapiLineTestInfo->dwAddressID = (DWORD)-1;

    if (! DoLineGetAgentGroupList(lpTapiLineTestInfo,
                                  LINEERR_INVALADDRESSID,
                                  FALSE))
    {
            TLINE_FAIL();
    }

    TapiLogDetail(DBUG_SHOW_PASS,
                  ">>Test Case %ld: Bad dwAddressID= dwNumAddress",
                  dwTestCase+1);

    lpTapiLineTestInfo->dwAddressID =
        lpTapiLineTestInfo->lpLineDevCaps->dwNumAddresses;

    if (! DoLineGetAgentGroupList(lpTapiLineTestInfo,
                                  LINEERR_INVALADDRESSID,
                                  FALSE))
    {
            TLINE_FAIL();
    }

    fTestPassed = ShowTestCase(fTestPassed);

    if (! DoLineClose(lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }


    //
    // Shutdown
    //

    if (! DoLineShutdown(lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }

    FreeTestHeap();

    // ===================================================================
    // ===================================================================
    //
    // 4. Test Case: Bad lpAgentGroupList.
    //
    // ===================================================================
    // ===================================================================

    TapiLineTestInit();
    lpTapiLineTestInfo = GetLineTestInfo();
    fTestPassed = TRUE;


    //
    // Initialize a line app
    //

    lpTapiLineTestInfo->lpLineInitializeExParams =
         (LPLINEINITIALIZEEXPARAMS) AllocFromTestHeap (
         sizeof(LINEINITIALIZEEXPARAMS));
    lpTapiLineTestInfo->lpLineInitializeExParams->dwTotalSize =
         sizeof(LINEINITIALIZEEXPARAMS);
    lpTapiLineTestInfo->lpLineInitializeExParams->dwOptions =
         LINEINITIALIZEEXOPTION_USEHIDDENWINDOW;
    lpTapiLineTestInfo->lpdwAPIVersion = &lpTapiLineTestInfo->dwAPIVersion;
    lpTapiLineTestInfo->dwAPIVersion = TAPI_VERSION2_0;

    if(! DoLineInitializeEx (lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }

    lpTapiLineTestInfo->dwDeviceID =
        (*(lpTapiLineTestInfo->lpdwNumDevs) == 0 ?
        0 : *(lpTapiLineTestInfo->lpdwNumDevs)-1);

    if(IsUNIMDMLineDevice(lpTapiLineTestInfo))
    {
        TapiLogDetail(
            DBUG_SHOW_DETAIL,
            "## lineGetAgentActivityList unimodem didn't support");
        return fTestPassed;
    }

    lpTapiLineTestInfo->dwAPIVersion = HIGH_APIVERSION;
    lpTapiLineTestInfo->dwExtLowVersion = LOW_APIVERSION;
    lpTapiLineTestInfo->dwExtHighVersion = HIGH_APIVERSION;
    lpTapiLineTestInfo->lpdwExtVersion = &lpTapiLineTestInfo->dwExtVersion;

    //
    // Negotiate the API Ext Version
    //

    if (! DoLineNegotiateExtVersion(lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }

    //
    // Open a line
    //

    lpTapiLineTestInfo->dwMediaModes = LINEMEDIAMODE_DATAMODEM;
    lpTapiLineTestInfo->dwPrivileges = LINECALLPRIVILEGE_OWNER |
													LINEOPENOPTION_PROXY;
    lpTapiLineTestInfo->lpCallParams = (LPLINECALLPARAMS) AllocFromTestHeap (
			sizeof(LINECALLPARAMS) + sizeof(DWORD));
    lpTapiLineTestInfo->lpCallParams->dwTotalSize =
			sizeof(LINECALLPARAMS) + sizeof(DWORD);
    lpTapiLineTestInfo->lpCallParams->dwDevSpecificSize =
						sizeof(DWORD);
    lpTapiLineTestInfo->lpCallParams->dwDevSpecificOffset =
						sizeof(LINECALLPARAMS);
    lpRequestBuf = (LPDWORD) (((LPBYTE)lpTapiLineTestInfo->lpCallParams)  +
                    lpTapiLineTestInfo->lpCallParams->dwDevSpecificOffset);
    *lpRequestBuf = LINEPROXYREQUEST_GETAGENTGROUPLIST;

		
    if (! DoLineOpen(lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }

	//
    // Get the line device capabilities
    //

    TapiLogDetail(DBUG_SHOW_PASS,
                  ">>Test Case %ld: Bad lpAgentGroupList",
                  dwTestCase+1);

    lpTapiLineTestInfo->dwAddressID = 0;

    for(i=0; i < NUMINVALIDPOINTERS; i++)
    {

        lpTapiLineTestInfo->lpAgentGroupList =
            (LPLINEAGENTGROUPLIST)gdwInvalidPointers[i];
        if (! DoLineGetAgentGroupList(lpTapiLineTestInfo,
                                 LINEERR_INVALPOINTER,
                                 FALSE))
        {
                TLINE_FAIL();
        }
    }

    fTestPassed = ShowTestCase(fTestPassed);

    if (! DoLineClose(lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }


    //
    // Shutdown
    //

    if (! DoLineShutdown(lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }

    FreeTestHeap();

    // ===================================================================
    // ===================================================================
    //
    // 5. Test Case: Bad dwTotalSize.
    //
    // ===================================================================
    // ===================================================================

    TapiLineTestInit();
    lpTapiLineTestInfo = GetLineTestInfo();
    fTestPassed = TRUE;


    //
    // Initialize a line app
    //

    lpTapiLineTestInfo->lpLineInitializeExParams =
         (LPLINEINITIALIZEEXPARAMS) AllocFromTestHeap (
         sizeof(LINEINITIALIZEEXPARAMS));
    lpTapiLineTestInfo->lpLineInitializeExParams->dwTotalSize =
         sizeof(LINEINITIALIZEEXPARAMS);
    lpTapiLineTestInfo->lpLineInitializeExParams->dwOptions =
         LINEINITIALIZEEXOPTION_USEHIDDENWINDOW;
    lpTapiLineTestInfo->lpdwAPIVersion = &lpTapiLineTestInfo->dwAPIVersion;
    lpTapiLineTestInfo->dwAPIVersion = TAPI_VERSION2_0;

    if(! DoLineInitializeEx (lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }

    lpTapiLineTestInfo->dwDeviceID =
        (*(lpTapiLineTestInfo->lpdwNumDevs) == 0 ?
        0 : *(lpTapiLineTestInfo->lpdwNumDevs)-1);

    if(IsUNIMDMLineDevice(lpTapiLineTestInfo))
    {
        TapiLogDetail(
            DBUG_SHOW_DETAIL,
            "## lineGetAgentActivityList unimodem didn't support");
        return fTestPassed;
    }

    lpTapiLineTestInfo->dwAPIVersion = HIGH_APIVERSION;
    lpTapiLineTestInfo->dwExtLowVersion = LOW_APIVERSION;
    lpTapiLineTestInfo->dwExtHighVersion = HIGH_APIVERSION;
    lpTapiLineTestInfo->lpdwExtVersion = &lpTapiLineTestInfo->dwExtVersion;

    //
    // Negotiate the API Ext Version
    //

    if (! DoLineNegotiateExtVersion(lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }

    //
    // Open a line
    //

    lpTapiLineTestInfo->dwMediaModes = LINEMEDIAMODE_DATAMODEM;
    lpTapiLineTestInfo->dwPrivileges = LINECALLPRIVILEGE_OWNER |
													LINEOPENOPTION_PROXY;
    lpTapiLineTestInfo->lpCallParams = (LPLINECALLPARAMS) AllocFromTestHeap (
			sizeof(LINECALLPARAMS) + sizeof(DWORD));
    lpTapiLineTestInfo->lpCallParams->dwTotalSize =
			sizeof(LINECALLPARAMS) + sizeof(DWORD);
    lpTapiLineTestInfo->lpCallParams->dwDevSpecificSize =
						sizeof(DWORD);
    lpTapiLineTestInfo->lpCallParams->dwDevSpecificOffset =
						sizeof(LINECALLPARAMS);
    lpRequestBuf = (LPDWORD) (((LPBYTE)lpTapiLineTestInfo->lpCallParams)  +
                    lpTapiLineTestInfo->lpCallParams->dwDevSpecificOffset);
    *lpRequestBuf = LINEPROXYREQUEST_GETAGENTGROUPLIST;

		
    if (! DoLineOpen(lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }

	//
    // Get the line device capabilities
    //

    lpTapiLineTestInfo->dwAddressID = 0;

    lpTapiLineTestInfo->lpAgentGroupList = (LPLINEAGENTGROUPLIST) AllocFromTestHeap(
            sizeof(LINEAGENTGROUPLIST)
            );
    TapiLogDetail(DBUG_SHOW_PASS,
                  ">>Test Case %ld: Bad dwTotalSize",
                  dwTestCase+1);

/*
    lpTapiLineTestInfo->lpAgentGroupList->dwTotalSize = 0;

    if (! DoLineGetAgentGroupList(lpTapiLineTestInfo,
                             LINEERR_STRUCTURETOOSMALL,
                             FALSE))
    {
            TLINE_FAIL();
    }

    TapiLogDetail(DBUG_SHOW_PASS,
                  ">>Test Case %ld: Bad dwTotalSize=fixed_size-1",
                  dwTestCase+1);
    lpTapiLineTestInfo->lpAgentGroupList->dwTotalSize =
        sizeof(LINEAGENTGROUPLIST)-1;

    if (! DoLineGetAgentGroupList(lpTapiLineTestInfo,
                             LINEERR_STRUCTURETOOSMALL,
                             FALSE))
    {
            TLINE_FAIL();
    }

    TapiLogDetail(DBUG_SHOW_PASS,
                  ">>Test Case %ld: Bad dwTotalSize=real big",
                  dwTestCase+1);
    lpTapiLineTestInfo->lpAgentGroupList->dwTotalSize = 0x7FFFFFFF;

    if (! DoLineGetAgentGroupList(lpTapiLineTestInfo,
                             LINEERR_INVALPOINTER,
                             FALSE))
    {
            TLINE_FAIL();
    }
*/

    for (n = 0; n < NUMTOTALSIZES; n++)
        {
        lpTapiLineTestInfo->lpAgentGroupList->dwTotalSize = 
                        dwTotalSizes[n];
	     if(dwTotalSizes[n] < dwFixedSize)
           lExpected = LINEERR_STRUCTURETOOSMALL;
        else
           lExpected = LINEERR_INVALPOINTER;
        TapiLogDetail(
           DBUG_SHOW_DETAIL,
           "dwTotalSize = %lx", dwTotalSizes[n]);
        if (! DoLineGetAgentGroupList(lpTapiLineTestInfo, lExpected, TRUE))
           {
              TLINE_FAIL();
           }
        }
 
 
    fTestPassed = ShowTestCase(fTestPassed);
    lpTapiLineTestInfo->lpAgentGroupList->dwTotalSize = dwFixedSize;

    if (! DoLineClose(lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }


    //
    // Shutdown
    //

    if (! DoLineShutdown(lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }

    FreeTestHeap();

    // ===================================================================
    // ===================================================================
    //
    // 6. Test Case: Open with no Proxy request.
    //
    // ===================================================================
    // ===================================================================

    TapiLineTestInit();
    lpTapiLineTestInfo = GetLineTestInfo();
    fTestPassed = TRUE;


    //
    // Initialize a line app
    //

    lpTapiLineTestInfo->lpLineInitializeExParams =
         (LPLINEINITIALIZEEXPARAMS) AllocFromTestHeap (
         sizeof(LINEINITIALIZEEXPARAMS));
    lpTapiLineTestInfo->lpLineInitializeExParams->dwTotalSize =
         sizeof(LINEINITIALIZEEXPARAMS);
    lpTapiLineTestInfo->lpLineInitializeExParams->dwOptions =
         LINEINITIALIZEEXOPTION_USEHIDDENWINDOW;
    lpTapiLineTestInfo->lpdwAPIVersion = &lpTapiLineTestInfo->dwAPIVersion;
    lpTapiLineTestInfo->dwAPIVersion = TAPI_VERSION2_0;

    if(! DoLineInitializeEx (lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }

    lpTapiLineTestInfo->dwDeviceID =
        (*(lpTapiLineTestInfo->lpdwNumDevs) == 0 ?
        0 : *(lpTapiLineTestInfo->lpdwNumDevs)-1);

    if(IsUNIMDMLineDevice(lpTapiLineTestInfo))
    {
        TapiLogDetail(
            DBUG_SHOW_DETAIL,
            "## lineGetAgentActivityList unimodem didn't support");
        return fTestPassed;
    }

    lpTapiLineTestInfo->dwAPIVersion = HIGH_APIVERSION;
    lpTapiLineTestInfo->dwExtLowVersion = LOW_APIVERSION;
    lpTapiLineTestInfo->dwExtHighVersion = HIGH_APIVERSION;
    lpTapiLineTestInfo->lpdwExtVersion = &lpTapiLineTestInfo->dwExtVersion;

    //
    // Negotiate the API Ext Version
    //

    if (! DoLineNegotiateExtVersion(lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }

    //
    // Open a line
    //

    lpTapiLineTestInfo->dwMediaModes = LINEMEDIAMODE_DATAMODEM;
    lpTapiLineTestInfo->dwPrivileges = LINECALLPRIVILEGE_OWNER ;
    lpTapiLineTestInfo->lpCallParams = (LPLINECALLPARAMS) AllocFromTestHeap (
			sizeof(LINECALLPARAMS) + sizeof(DWORD));
    lpTapiLineTestInfo->lpCallParams->dwTotalSize =
			sizeof(LINECALLPARAMS) + sizeof(DWORD);
    lpTapiLineTestInfo->lpCallParams->dwDevSpecificSize =
						sizeof(DWORD);
    lpTapiLineTestInfo->lpCallParams->dwDevSpecificOffset =
						sizeof(LINECALLPARAMS);
    lpRequestBuf = (LPDWORD) (((LPBYTE)lpTapiLineTestInfo->lpCallParams)  +
                    lpTapiLineTestInfo->lpCallParams->dwDevSpecificOffset);
    *lpRequestBuf = LINEPROXYREQUEST_GETAGENTGROUPLIST;

		
    if (! DoLineOpen(lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }

	//
    // Get the line device capabilities
    //

    TapiLogDetail(DBUG_SHOW_PASS,
                  ">>Test Case %ld: Open with no Proxy request",
                  dwTestCase+1);

    lpTapiLineTestInfo->dwAddressID = 0;

    lpTapiLineTestInfo->lpAgentGroupList = (LPLINEAGENTGROUPLIST) AllocFromTestHeap(
            sizeof(LINEAGENTGROUPLIST)
            );
    lpTapiLineTestInfo->lpAgentGroupList->dwTotalSize = sizeof(LINEAGENTGROUPLIST);

    if (! DoLineGetAgentGroupList(lpTapiLineTestInfo, LINEERR_OPERATIONUNAVAIL, TRUE))
    {
        TLINE_FAIL();
    }

    fTestPassed = ShowTestCase(fTestPassed);

    if (! DoLineClose(lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }


    //
    // Shutdown
    //

    if (! DoLineShutdown(lpTapiLineTestInfo, TAPISUCCESS))
    {
        TLINE_FAIL();
    }

    FreeTestHeap();

    //
    // +----------------------edit above this line-------------------------
    //

    TapiLogDetail(
        DBUG_SHOW_PASS,
        "@@ LineGetAgentGroupList: Total Test Case = %ld, Passed = %ld, Failed = %ld",
        dwTestCase, dwTestCasePassed, dwTestCaseFailed);
    TapiLogDetail(
        DBUG_SHOW_PASS,
        "@@ Total Test Case = %ld, Passed = %ld, Failed = %ld",
        dwglTestCase, dwglTestCasePassed, dwglTestCaseFailed);
    TapiLogDetail(
        DBUG_SHOW_PASS,
        ">>>>>>>>  End testing LineGetAgentGroupList  <<<<<<<<");

    if(dwTestCaseFailed > 0)
        fTestPassed = FALSE;

    return fTestPassed;
}



