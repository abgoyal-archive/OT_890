/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

/****************************************************************************

  (c) SYSTEC electronic GmbH, D-07973 Greiz, August-Bebel-Str. 29
      www.systec-electronic.com

  Project:      openPOWERLINK

  Description:  source file for Epl Userspace-Timermodule NULL-Implementation

  License:

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.

    3. Neither the name of SYSTEC electronic GmbH nor the names of its
       contributors may be used to endorse or promote products derived
       from this software without prior written permission. For written
       permission, please contact info@systec-electronic.com.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
    ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.

    Severability Clause:

        If a provision of this License is or becomes illegal, invalid or
        unenforceable in any jurisdiction, that shall not affect:
        1. the validity or enforceability in that jurisdiction of any other
           provision of this License; or
        2. the validity or enforceability in other jurisdictions of that or
           any other provision of this License.

  -------------------------------------------------------------------------

                $RCSfile: EplTimeruNull.c,v $

                $Author$

                $Revision$  $Date$

                $State: Exp $

                Build Environment:
                KEIL uVision 2

  -------------------------------------------------------------------------

  Revision History:

  2006/07/06 k.t.:   start of the implementation

****************************************************************************/

#include "user/EplTimeru.h"

/***************************************************************************/
/*                                                                         */
/*                                                                         */
/*          G L O B A L   D E F I N I T I O N S                            */
/*                                                                         */
/*                                                                         */
/***************************************************************************/

//---------------------------------------------------------------------------
// const defines
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// local types
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// modul globale vars
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// local function prototypes
//---------------------------------------------------------------------------

/***************************************************************************/
/*                                                                         */
/*                                                                         */
/*          C L A S S  <Epl Userspace-Timermodule NULL-Implementation>     */
/*                                                                         */
/*                                                                         */
/***************************************************************************/
//
// Description: Epl Userspace-Timermodule NULL-Implementation
//
//
/***************************************************************************/

//=========================================================================//
//                                                                         //
//          P U B L I C   F U N C T I O N S                                //
//                                                                         //
//=========================================================================//

//---------------------------------------------------------------------------
//
// Function:    EplTimeruInit
//
// Description: function init first instance
//
//
//
// Parameters:
//
//
// Returns:     tEplKernel  = errorcode
//
//
// State:
//
//---------------------------------------------------------------------------
tEplKernel PUBLIC EplTimeruInit()
{
	tEplKernel Ret;

	Ret = EplTimeruAddInstance();

	return Ret;
}

//---------------------------------------------------------------------------
//
// Function:    EplTimeruAddInstance
//
// Description: function init additional instance
//
//
//
// Parameters:
//
//
// Returns:     tEplKernel  = errorcode
//
//
// State:
//
//---------------------------------------------------------------------------
tEplKernel PUBLIC EplTimeruAddInstance()
{
	tEplKernel Ret;

	Ret = kEplSuccessful;

	return Ret;
}

//---------------------------------------------------------------------------
//
// Function:    EplTimeruDelInstance
//
// Description: function delte instance
//              -> under Win32 nothing to do
//              -> no instnace table needed
//
//
//
// Parameters:
//
//
// Returns:     tEplKernel  = errorcode
//
//
// State:
//
//---------------------------------------------------------------------------
tEplKernel PUBLIC EplTimeruDelInstance()
{
	tEplKernel Ret;

	Ret = kEplSuccessful;

	return Ret;
}

//---------------------------------------------------------------------------
//
// Function:    EplTimeruSetTimerMs
//
// Description: function create a timer and return a handle to the pointer
//
//
//
// Parameters:  pTimerHdl_p = pointer to a buffer to fill in the handle
//              ulTime_p    = time for timer in ms
//              Argument_p  = argument for timer
//
//
// Returns:     tEplKernel  = errorcode
//
//
// State:
//
//---------------------------------------------------------------------------
tEplKernel PUBLIC EplTimeruSetTimerMs(tEplTimerHdl * pTimerHdl_p,
				      unsigned long ulTime_p,
				      tEplTimerArg Argument_p)
{
	tEplKernel Ret;

	Ret = kEplSuccessful;

	// check handle
	if (pTimerHdl_p == NULL) {
		Ret = kEplTimerInvalidHandle;
		goto Exit;
	}

      Exit:
	return Ret;
}

 //---------------------------------------------------------------------------
//
// Function:    EplTimeruModifyTimerMs
//
// Description: function change a timer and return a handle to the pointer
//
//
//
// Parameters:  pTimerHdl_p = pointer to a buffer to fill in the handle
//              ulTime_p    = time for timer in ms
//              Argument_p  = argument for timer
//
//
// Returns:     tEplKernel  = errorcode
//
//
// State:
//
//---------------------------------------------------------------------------
tEplKernel PUBLIC EplTimeruModifyTimerMs(tEplTimerHdl * pTimerHdl_p,
					 unsigned long ulTime_p,
					 tEplTimerArg Argument_p)
{
	tEplKernel Ret;

	Ret = kEplSuccessful;

	// check parameter
	if (pTimerHdl_p == NULL) {
		Ret = kEplTimerInvalidHandle;
		goto Exit;
	}

      Exit:
	return Ret;
}

 //---------------------------------------------------------------------------
//
// Function:    EplTimeruDeleteTimer
//
// Description: function delte a timer
//
//
//
// Parameters:  pTimerHdl_p = pointer to a buffer to fill in the handle
//
//
// Returns:     tEplKernel  = errorcode
//
//
// State:
//
//---------------------------------------------------------------------------
tEplKernel PUBLIC EplTimeruDeleteTimer(tEplTimerHdl * pTimerHdl_p)
{
	tEplKernel Ret;

	Ret = kEplSuccessful;

	// check parameter
	if (pTimerHdl_p == NULL) {
		Ret = kEplTimerInvalidHandle;
		goto Exit;
	}
	// set handle invalide
	*pTimerHdl_p = 0;

      Exit:
	return Ret;

}

//=========================================================================//
//                                                                         //
//          P R I V A T E   F U N C T I O N S                              //
//                                                                         //
//=========================================================================//

// EOF
