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

/*
 *************************************************************************
 * Ralink Tech Inc.
 * 5F., No.36, Taiyuan St., Jhubei City,
 * Hsinchu County 302,
 * Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2007, Ralink Technology, Inc.
 *
 * This program is free software; you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation; either version 2 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * This program is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with this program; if not, write to the                         *
 * Free Software Foundation, Inc.,                                       *
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 *                                                                       *
 *************************************************************************

	Module Name:
	auth_rsp.c

	Abstract:

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	John		2004-10-1		copy from RT2560
*/
#include "../rt_config.h"

/*
    ==========================================================================
    Description:
        authentication state machine init procedure
    Parameters:
        Sm - the state machine

	IRQL = PASSIVE_LEVEL

    ==========================================================================
 */
VOID AuthRspStateMachineInit(
    IN PRTMP_ADAPTER pAd,
    IN PSTATE_MACHINE Sm,
    IN STATE_MACHINE_FUNC Trans[])
{
    StateMachineInit(Sm, Trans, MAX_AUTH_RSP_STATE, MAX_AUTH_RSP_MSG, (STATE_MACHINE_FUNC)Drop, AUTH_RSP_IDLE, AUTH_RSP_MACHINE_BASE);

    // column 1
    StateMachineSetAction(Sm, AUTH_RSP_IDLE, MT2_PEER_DEAUTH, (STATE_MACHINE_FUNC)PeerDeauthAction);

    // column 2
    StateMachineSetAction(Sm, AUTH_RSP_WAIT_CHAL, MT2_PEER_DEAUTH, (STATE_MACHINE_FUNC)PeerDeauthAction);

}

/*
    ==========================================================================
    Description:

	IRQL = DISPATCH_LEVEL

    ==========================================================================
*/
VOID PeerAuthSimpleRspGenAndSend(
    IN PRTMP_ADAPTER pAd,
    IN PHEADER_802_11 pHdr80211,
    IN USHORT Alg,
    IN USHORT Seq,
    IN USHORT Reason,
    IN USHORT Status)
{
    HEADER_802_11     AuthHdr;
    ULONG             FrameLen = 0;
    PUCHAR            pOutBuffer = NULL;
    NDIS_STATUS       NStatus;

    if (Reason != MLME_SUCCESS)
    {
        DBGPRINT(RT_DEBUG_TRACE, ("Peer AUTH fail...\n"));
        return;
    }

	//Get an unused nonpaged memory
    NStatus = MlmeAllocateMemory(pAd, &pOutBuffer);
    if (NStatus != NDIS_STATUS_SUCCESS)
        return;

    DBGPRINT(RT_DEBUG_TRACE, ("Send AUTH response (seq#2)...\n"));
    MgtMacHeaderInit(pAd, &AuthHdr, SUBTYPE_AUTH, 0, pHdr80211->Addr2, pAd->MlmeAux.Bssid);
    MakeOutgoingFrame(pOutBuffer,               &FrameLen,
                      sizeof(HEADER_802_11),    &AuthHdr,
                      2,                        &Alg,
                      2,                        &Seq,
                      2,                        &Reason,
                      END_OF_ARGS);
    MiniportMMRequest(pAd, 0, pOutBuffer, FrameLen);
	MlmeFreeMemory(pAd, pOutBuffer);
}

/*
    ==========================================================================
    Description:

	IRQL = DISPATCH_LEVEL

    ==========================================================================
*/
VOID PeerDeauthAction(
    IN PRTMP_ADAPTER pAd,
    IN PMLME_QUEUE_ELEM Elem)
{
    UCHAR       Addr2[MAC_ADDR_LEN];
    USHORT      Reason;

    if (PeerDeauthSanity(pAd, Elem->Msg, Elem->MsgLen, Addr2, &Reason))
    {
        if (INFRA_ON(pAd) && MAC_ADDR_EQUAL(Addr2, pAd->CommonCfg.Bssid))
        {
            DBGPRINT(RT_DEBUG_TRACE,("AUTH_RSP - receive DE-AUTH from our AP (Reason=%d)\n", Reason));


#ifdef NATIVE_WPA_SUPPLICANT_SUPPORT
            {
                union iwreq_data    wrqu;
                memset(wrqu.ap_addr.sa_data, 0, MAC_ADDR_LEN);
                wireless_send_event(pAd->net_dev, SIOCGIWAP, &wrqu, NULL);
            }
#endif // NATIVE_WPA_SUPPLICANT_SUPPORT //


			// send wireless event - for deauthentication
			if (pAd->CommonCfg.bWirelessEvent)
				RTMPSendWirelessEvent(pAd, IW_DEAUTH_EVENT_FLAG, pAd->MacTab.Content[BSSID_WCID].Addr, BSS0, 0);

            LinkDown(pAd, TRUE);

            // Authentication Mode Cisco_LEAP has start a timer
            // We should cancel it if using LEAP
#ifdef LEAP_SUPPORT
            if (pAd->StaCfg.LeapAuthMode == CISCO_AuthModeLEAP)
            {
                RTMPCancelTimer(&pAd->StaCfg.LeapAuthTimer, &TimerCancelled);
                //Check is it mach the LEAP Authentication failed as possible a Rogue AP
                //on it's PortSecured not equal to WPA_802_1X_PORT_SECURED while process the Authenticaton.
                if ((pAd->StaCfg.PortSecured != WPA_802_1X_PORT_SECURED) && (pAd->Mlme.LeapMachine.CurrState != LEAP_IDLE))
                {
                    RogueApTableSetEntry(pAd, &pAd->StaCfg.RogueApTab, Addr2, LEAP_REASON_AUTH_TIMEOUT);
                }
            }
#endif // LEAP_SUPPORT //
        }
    }
    else
    {
        DBGPRINT(RT_DEBUG_TRACE,("AUTH_RSP - PeerDeauthAction() sanity check fail\n"));
    }
}

