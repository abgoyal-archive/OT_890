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
 * Copyright (c) 2007-2008 Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
/*                                                                      */
/*  Module Name : hb.c                                                  */
/*                                                                      */
/*  Abstract                                                            */
/*      This module contains house keeping and timer functions.         */
/*                                                                      */
/*  NOTES                                                               */
/*      None                                                            */
/*                                                                      */
/************************************************************************/
#include "cprecomp.h"

/* Called by wrapper every 10 msec */
void zfiHeartBeat(zdev_t* dev)
{
    zmw_get_wlan_dev(dev);

    wd->tick++;

#if 0
    /* => every 1.28 seconds */
    if (wd->cwm.cw_enable && ((wd->tick & 0x7f) == 0x3f))
    {
        zfHpCwmUpdate(dev);
    }
#endif
    /* => every 2.56 seconds */
    if ((wd->tick & 0xff) == 0)
    {
        zfAgingDefragList(dev, 1);
    }

    /* Watch Dog */
    //zfWatchDog();

    /* LED Control (per 100ms) */
    if ((wd->tick % 10) == 9)
    {
        zfLed100msCtrl(dev);
#ifdef ZM_ENABLE_BA_RATECTRL
        if (!wd->modeMDKEnable)
        {
            zfiDbgReadTally(dev);
        }
#endif
    }

#ifdef ZM_ENABLE_REWRITE_BEACON_START_ADDRESS
    if ( wd->wlanMode == ZM_MODE_IBSS )
    {
        if ( zfStaIsConnected(dev) )
        {
            zfReWriteBeaconStartAddress(dev);
        }
    }
#endif

    if ( wd->wlanMode == ZM_MODE_IBSS )
    {
        if ( zfStaIsConnected(dev) )
        {
            wd->tickIbssReceiveBeacon++;  // add 10ms

            if ( (wd->sta.ibssSiteSurveyStatus == 2) &&
                 (wd->tickIbssReceiveBeacon == 300) &&
                 (wd->sta.ibssReceiveBeaconCount < 3) )
            {
                zm_debug_msg0("It is happen!!! No error message");
                zfReSetCurrentFrequency(dev);
            }
        }
    }

    if(wd->sta.ReceivedPacketRateCounter <= 0)
    {
        wd->sta.ReceivedPktRatePerSecond = wd->sta.TotalNumberOfReceivePackets;
	//zm_debug_msg1("Receive Packet Per Second  = ", wd->sta.ReceivedPktRatePerSecond);
	    if (wd->sta.TotalNumberOfReceivePackets != 0)
	    {
	        wd->sta.avgSizeOfReceivePackets = wd->sta.TotalNumberOfReceiveBytes/wd->sta.TotalNumberOfReceivePackets;
	    }
	    else
	    {
	        wd->sta.avgSizeOfReceivePackets = 640;
	    }
        wd->sta.TotalNumberOfReceivePackets = 0;
        wd->sta.TotalNumberOfReceiveBytes = 0;
        wd->sta.ReceivedPacketRateCounter = 100; /*for another 1s*/
    }
    else
    {
        wd->sta.ReceivedPacketRateCounter--;
    }

	/* => every 1.28 seconds */
	if((wd->tick & 0x7f) == 0x3f)
	{
		if( wd->sta.NonNAPcount > 0)
		{
			wd->sta.RTSInAGGMode = TRUE;
			wd->sta.NonNAPcount = 0;
		}
		else
		{
			wd->sta.RTSInAGGMode = FALSE;
		}
	}



    /* Maintain management time tick */
    zfMmApTimeTick(dev);
    zfMmStaTimeTick(dev);

    //zfPhyCrTuning(dev);

    //zfTxPowerControl(dev);
    zfHpHeartBeat(dev);

}


void zfDumpBssList(zdev_t* dev)
{
    struct zsBssInfo* pBssInfo;
    u8_t   str[33];
    u8_t   i, j;
    u32_t  addr1, addr2;
    zmw_get_wlan_dev(dev);
    zmw_declare_for_critical_section();

    zm_debug_msg0("***** Bss scan result *****");
    zmw_enter_critical_section(dev);

    pBssInfo = wd->sta.bssList.head;

    for( i=0; i<wd->sta.bssList.bssCount; i++ )
    {
        if ( i )
        {
            zm_debug_msg0("---------------------------");
        }

        zm_debug_msg1("BSS #", i);
        for(j=0; j<pBssInfo->ssid[1]; j++)
        {
            str[j] = pBssInfo->ssid[2+j];
        }
        str[pBssInfo->ssid[1]] = 0;
        zm_debug_msg0("SSID = ");
        zm_debug_msg0(str);

        addr1 = (pBssInfo->bssid[0] << 16) + (pBssInfo->bssid[1] << 8 )
                + pBssInfo->bssid[2];
        addr2 = (pBssInfo->bssid[3] << 16) + (pBssInfo->bssid[4] << 8 )
                + pBssInfo->bssid[5];
        zm_debug_msg2("Bssid = ", addr1);
        zm_debug_msg2("        ", addr2);
        zm_debug_msg1("frequency = ", pBssInfo->frequency);
        zm_debug_msg1("security type = ", pBssInfo->securityType);
        zm_debug_msg1("WME = ", pBssInfo->wmeSupport);
        zm_debug_msg1("beacon interval = ", pBssInfo->beaconInterval[0]
                      + (pBssInfo->beaconInterval[1] << 8));
        zm_debug_msg1("capability = ", pBssInfo->capability[0]
                      + (pBssInfo->capability[1] << 8));
        if ( pBssInfo->supportedRates[1] > 0 )
        {
            for( j=0; j<pBssInfo->supportedRates[1]; j++ )
            {
                zm_debug_msg2("supported rates = ", pBssInfo->supportedRates[2+j]);
            }
        }

        for( j=0; j<pBssInfo->extSupportedRates[1]; j++ )
        {
            zm_debug_msg2("ext supported rates = ", pBssInfo->extSupportedRates[2+j]);
        }

        pBssInfo = pBssInfo->next;
    }
    zmw_leave_critical_section(dev);

    zm_debug_msg0("***************************");
}

