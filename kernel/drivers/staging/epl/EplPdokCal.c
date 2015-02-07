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

  Description:  source file for kernel PDO Communication Abstraction Layer module

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

                $RCSfile: EplPdokCal.c,v $

                $Author$

                $Revision$  $Date$

                $State: Exp $

                Build Environment:
                    GCC V3.4

  -------------------------------------------------------------------------

  Revision History:

  2006/06/27 d.k.:   start of the implementation, version 1.00

****************************************************************************/

#include "kernel/EplPdokCal.h"

#if (((EPL_MODULE_INTEGRATION) & (EPL_MODULE_PDOK)) != 0)

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
/*          C L A S S  EplPdokCal                                          */
/*                                                                         */
/*                                                                         */
/***************************************************************************/
//
// Description:
//
//
/***************************************************************************/

//=========================================================================//
//                                                                         //
//          P R I V A T E   D E F I N I T I O N S                          //
//                                                                         //
//=========================================================================//

//---------------------------------------------------------------------------
// const defines
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// local types
//---------------------------------------------------------------------------

typedef struct {
	BOOL m_fTpdosValid;

} tEplPdokCalInstance;

//---------------------------------------------------------------------------
// local vars
//---------------------------------------------------------------------------

static tEplPdokCalInstance EplPdokCalInstance_g;

//---------------------------------------------------------------------------
// local function prototypes
//---------------------------------------------------------------------------

//=========================================================================//
//                                                                         //
//          P U B L I C   F U N C T I O N S                                //
//                                                                         //
//=========================================================================//

//---------------------------------------------------------------------------
//
// Function:    EplPdokCalAddInstance()
//
// Description: add and initialize new instance of EPL stack
//
// Parameters:  none
//
// Returns:     tEplKernel              = error code
//
//
// State:
//
//---------------------------------------------------------------------------

tEplKernel EplPdokCalAddInstance(void)
{

	EPL_MEMSET(&EplPdokCalInstance_g, 0, sizeof(EplPdokCalInstance_g));

	return kEplSuccessful;
}

//---------------------------------------------------------------------------
//
// Function:    EplPdokCalDelInstance()
//
// Description: deletes an instance of EPL stack
//
// Parameters:  none
//
// Returns:     tEplKernel              = error code
//
//
// State:
//
//---------------------------------------------------------------------------

tEplKernel EplPdokCalDelInstance(void)
{

	return kEplSuccessful;
}

//---------------------------------------------------------------------------
//
// Function:    EplPdokCalSetTpdosValid()
//
// Description: This function sets the validity flag for TPDOs to the
//              specified value.
//
// Parameters:  fValid_p                = validity flag
//
// Returns:     tEplKernel              = error code
//
//
// State:
//
//---------------------------------------------------------------------------

tEplKernel EplPdokCalSetTpdosValid(BOOL fValid_p)
{
	tEplKernel Ret = kEplSuccessful;

	EplPdokCalInstance_g.m_fTpdosValid = fValid_p;

	return Ret;
}

//---------------------------------------------------------------------------
//
// Function:    EplPdokCalAreTpdosValid()
//
// Description: This function returns the validity flag for TPDOs.
//
// Parameters:  pfValid_p               = OUT: validity flag
//
// Returns:     tEplKernel              = error code
//
//
// State:
//
//---------------------------------------------------------------------------

tEplKernel EplPdokCalAreTpdosValid(BOOL * pfValid_p)
{
	tEplKernel Ret = kEplSuccessful;

	*pfValid_p = EplPdokCalInstance_g.m_fTpdosValid;

	return Ret;
}

//=========================================================================//
//                                                                         //
//          P R I V A T E   F U N C T I O N S                              //
//                                                                         //
//=========================================================================//

//---------------------------------------------------------------------------
//
// Function:
//
// Description:
//
//
//
// Parameters:
//
//
// Returns:
//
//
// State:
//
//---------------------------------------------------------------------------

#endif

// EOF
