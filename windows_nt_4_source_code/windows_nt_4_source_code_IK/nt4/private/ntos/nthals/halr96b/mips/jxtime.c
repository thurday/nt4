// #pragma comment(exestr, "@(#) jxtime.c 1.1 95/09/28 15:41:57 nec")
/*++

Copyright (c) 1991  Microsoft Corporation

Module Name:

    jxtime.c

Abstract:

    This module implements the HAL set/query realtime clock routines for
    a MIPS R3000 or R4000 Jazz system.

Author:

    David N. Cutler (davec) 5-May-1991

Environment:

    Kernel mode

Revision History:

    M0001	1994.9.9	kbnes!A.Kuriyama
    
    Modify for R94A

    HalpReadClockRegister()  -  R94A use RTC Index register except EISA NmiEnable 
                                register.
    HalpWriteClockRegister() -  R94A use RTC Index register except EISA NmiEnable 
                                register.

    M0002	1994.10.8	kbnes!kuriyama(A)

                HalpReadClockRegister()
		      -add specify Read Data register
                HalpWritelockRegister()
		      -add specify Write Data register

    M0003	1994.10.14	kbnes!kuriyama(A)

                define error clear

    M0004       1994.12.19      kbnes!kuriyama(A)

                define miss fix
--*/

#include "halp.h"
#include "jazzrtc.h"
#include "eisa.h"


//
// Define forward referenced procedure prototypes.
//

UCHAR
HalpReadClockRegister (
    UCHAR Register
    );

VOID
HalpWriteClockRegister (
    UCHAR Register,
    UCHAR Value
    );

BOOLEAN
HalQueryRealTimeClock (
    OUT PTIME_FIELDS TimeFields
    )

/*++

Routine Description:

    This routine queries the realtime clock.

    N.B. This routine is required to provide any synchronization necessary
         to query the realtime clock information.

Arguments:

    TimeFields - Supplies a pointer to a time structure that receives
        the realtime clock information.

Return Value:

    If the power to the realtime clock has not failed, then the time
    values are read from the realtime clock and a value of TRUE is
    returned. Otherwise, a value of FALSE is returned.

--*/

{

    UCHAR DataByte;
    KIRQL OldIrql;

    //
    // If the realtime clock battery is still functioning, then read
    // the realtime clock values, and return a function value of TRUE.
    // Otherwise, return a function value of FALSE.
    //

    KeRaiseIrql(HIGH_LEVEL, &OldIrql);
    DataByte = HalpReadClockRegister(RTC_CONTROL_REGISTERD);
    if (((PRTC_CONTROL_REGISTER_D)(&DataByte))->ValidTime == 1) {

        //
        // Wait until the realtime clock is not being updated.
        //

        do {
            DataByte = HalpReadClockRegister(RTC_CONTROL_REGISTERA);
        } while (((PRTC_CONTROL_REGISTER_A)(&DataByte))->UpdateInProgress == 1);

        //
        // Read the realtime clock values.
        //

        TimeFields->Year = 1980 + (CSHORT)HalpReadClockRegister(RTC_YEAR);
        TimeFields->Month = (CSHORT)HalpReadClockRegister(RTC_MONTH);
        TimeFields->Day = (CSHORT)HalpReadClockRegister(RTC_DAY_OF_MONTH);
        TimeFields->Weekday  = (CSHORT)HalpReadClockRegister(RTC_DAY_OF_WEEK) - 1;
        TimeFields->Hour = (CSHORT)HalpReadClockRegister(RTC_HOUR);
        TimeFields->Minute = (CSHORT)HalpReadClockRegister(RTC_MINUTE);
        TimeFields->Second = (CSHORT)HalpReadClockRegister(RTC_SECOND);
        TimeFields->Milliseconds = 0;
        KeLowerIrql(OldIrql);
        return TRUE;

    } else {
        KeLowerIrql(OldIrql);
        return FALSE;
    }
}

BOOLEAN
HalSetRealTimeClock (
    IN PTIME_FIELDS TimeFields
    )

/*++

Routine Description:

    This routine sets the realtime clock.

    N.B. This routine is required to provide any synchronization necessary
         to set the realtime clock information.

Arguments:

    TimeFields - Supplies a pointer to a time structure that specifies the
        realtime clock information.

Return Value:

    If the power to the realtime clock has not failed, then the time
    values are written to the realtime clock and a value of TRUE is
    returned. Otherwise, a value of FALSE is returned.

--*/

{

    UCHAR DataByte;
    KIRQL OldIrql;

    //
    // If the realtime clock battery is still functioning, then write
    // the realtime clock values, and return a function value of TRUE.
    // Otherwise, return a function value of FALSE.
    //

    KeRaiseIrql(HIGH_LEVEL, &OldIrql);
    DataByte = HalpReadClockRegister(RTC_CONTROL_REGISTERD);
    if (((PRTC_CONTROL_REGISTER_D)(&DataByte))->ValidTime == 1) {

        //
        // Set the realtime clock control to set the time.
        //

        DataByte = 0;
        ((PRTC_CONTROL_REGISTER_B)(&DataByte))->HoursFormat = 1;
        ((PRTC_CONTROL_REGISTER_B)(&DataByte))->DataMode = 1;
        ((PRTC_CONTROL_REGISTER_B)(&DataByte))->SetTime = 1;
        HalpWriteClockRegister(RTC_CONTROL_REGISTERB, DataByte);

        //
        // Write the realtime clock values.
        //

        HalpWriteClockRegister(RTC_YEAR, (UCHAR)(TimeFields->Year - 1980));
        HalpWriteClockRegister(RTC_MONTH, (UCHAR)TimeFields->Month);
        HalpWriteClockRegister(RTC_DAY_OF_MONTH, (UCHAR)TimeFields->Day);
        HalpWriteClockRegister(RTC_DAY_OF_WEEK, (UCHAR)(TimeFields->Weekday + 1));
        HalpWriteClockRegister(RTC_HOUR, (UCHAR)TimeFields->Hour);
        HalpWriteClockRegister(RTC_MINUTE, (UCHAR)TimeFields->Minute);
        HalpWriteClockRegister(RTC_SECOND, (UCHAR)TimeFields->Second);

        //
        // Set the realtime clock control to update the time.
        //

        ((PRTC_CONTROL_REGISTER_B)(&DataByte))->SetTime = 0;
        HalpWriteClockRegister(RTC_CONTROL_REGISTERB, DataByte);
        KeLowerIrql(OldIrql);
        return TRUE;

    } else {
        KeLowerIrql(OldIrql);
        return FALSE;
    }
}

UCHAR
HalpReadClockRegister (
    UCHAR Register
    )

/*++

Routine Description:

    This routine reads the specified realtime clock register.

Arguments:

    Register - Supplies the number of the register whose value is read.

Return Value:

    The value of the register is returned as the function value.

--*/

{

/* start M0001 */
#if defined(_R94A_)  /* M0003 */
    // Insert the realtime clock register number, and write the value back
    // to RTC Index register. This selects the realtime clock register
    // that is read.  
    //

    WRITE_REGISTER_UCHAR(&((PRTC_REGISTERS) HalpRealTimeClockBase)->Index,
                         Register);


#else // _R94A_

    //
    // Insert the realtime clock register number, and write the value back
    // to the EISA NMI enable register. This selects the realtime clock register
    // that is read.  Note this is a write only register and the EISA NMI
    // is always enabled.
    //

    //
    // TEMPTEMP Disable NMI's for now because this is causing machines in the
    // build lab to get NMI's during boot.
    //

    Register |= 0x80;


    WRITE_REGISTER_UCHAR(&((PEISA_CONTROL) HalpEisaControlBase)->NmiEnable,
                         Register);

#endif // _R94A_
/* end M0001 */

    //
    // Read the realtime clock register value.
    //

/* start M0002 */
#if defined(_R94A_)  // M0004

    return READ_REGISTER_UCHAR( &((PRTC_REGISTERS) HalpRealTimeClockBase)->Data);


#else // _R94A_

    return READ_REGISTER_UCHAR((PUCHAR)HalpRealTimeClockBase);

#endif // _R94A_
/* end M0002 */

}

VOID
HalpWriteClockRegister (
    UCHAR Register,
    UCHAR Value
    )

/*++

Routine Description:

    This routine writes the specified value to the specified realtime
    clock register.

Arguments:

    Register - Supplies the number of the register whose value is written.

    Value - Supplies the value that is written to the specified register.

Return Value:

    The value of the register is returned as the function value.

--*/

{

/* start M0001 */
#if defined(_R94A_)
    //
    // Insert the realtime clock register number, and write the value back
    // to RTC Index register. This selects the realtime clock
    // register that is written.
    //

    WRITE_REGISTER_UCHAR(&((PRTC_REGISTERS) HalpRealTimeClockBase)->Index,
                         Register);

#else // _R94A_

    //
    // Insert the realtime clock register number, and write the value back
    // to the EISA NMI enable register. This selects the realtime clock
    // register that is written.  Note this is a write only register and
    // the EISA NMI is always enabled.
    //

    Register |= 0x80;

    WRITE_REGISTER_UCHAR(&((PEISA_CONTROL) HalpEisaControlBase)->NmiEnable,
                         Register);

#endif // _R94A_
/* end M0001 */

    //
    // Write the realtime clock register value.
    //

/* start M0002 */
#if defined(_R94A_) // M0004

    WRITE_REGISTER_UCHAR( &((PRTC_REGISTERS) HalpRealTimeClockBase)->Data, Value);

#else // _R94A_

    WRITE_REGISTER_UCHAR((PUCHAR)HalpRealTimeClockBase, Value);

#endif // _R94A_
/* end M0002 */

    return;
}









