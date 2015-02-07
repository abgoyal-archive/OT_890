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
  * Copyright (c) 1997-2000 LAN Media Corporation (LMC)
  * All rights reserved.  www.lanmedia.com
  *
  * This code is written by:
  * Andrew Stanley-Jones (asj@cban.com)
  * Rob Braun (bbraun@vix.com),
  * Michael Graff (explorer@vix.com) and
  * Matt Thomas (matt@3am-software.com).
  *
  * With Help By:
  * David Boggs
  * Ron Crane
  * Allan Cox
  *
  * This software may be used and distributed according to the terms
  * of the GNU General Public License version 2, incorporated herein by reference.
  *
  * Driver for the LanMedia LMC5200, LMC5245, LMC1000, LMC1200 cards.
  */

#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/ptrace.h>
#include <linux/errno.h>
#include <linux/ioport.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/in.h>
#include <linux/if_arp.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/inet.h>
#include <linux/workqueue.h>
#include <linux/proc_fs.h>
#include <linux/bitops.h>
#include <asm/processor.h>             /* Processor type for cache alignment. */
#include <asm/io.h>
#include <asm/dma.h>
#include <linux/smp.h>

#include "lmc.h"
#include "lmc_var.h"
#include "lmc_debug.h"
#include "lmc_ioctl.h"
#include "lmc_proto.h"

// attach
void lmc_proto_attach(lmc_softc_t *sc) /*FOLD00*/
{
    lmc_trace(sc->lmc_device, "lmc_proto_attach in");
    switch(sc->if_type){
    case LMC_PPP:
        {
            struct net_device *dev = sc->lmc_device;
            dev->do_ioctl = lmc_ioctl;
        }
        break;
    case LMC_NET:
        {
            struct net_device *dev = sc->lmc_device;
            /*
	     * They set a few basics because they don't use HDLC
             */
            dev->flags |= IFF_POINTOPOINT;

            dev->hard_header_len = 0;
            dev->addr_len = 0;
        }
    case LMC_RAW: /* Setup the task queue, maybe we should notify someone? */
        {
        }
    default:
        break;
    }
    lmc_trace(sc->lmc_device, "lmc_proto_attach out");
}

int lmc_proto_ioctl(lmc_softc_t *sc, struct ifreq *ifr, int cmd)
{
	lmc_trace(sc->lmc_device, "lmc_proto_ioctl");
	if (sc->if_type == LMC_PPP)
		return hdlc_ioctl(sc->lmc_device, ifr, cmd);
	return -EOPNOTSUPP;
}

int lmc_proto_open(lmc_softc_t *sc)
{
	int ret = 0;

	lmc_trace(sc->lmc_device, "lmc_proto_open in");

	if (sc->if_type == LMC_PPP) {
		ret = hdlc_open(sc->lmc_device);
		if (ret < 0)
			printk(KERN_WARNING "%s: HDLC open failed: %d\n",
			       sc->name, ret);
	}

	lmc_trace(sc->lmc_device, "lmc_proto_open out");
	return ret;
}

void lmc_proto_close(lmc_softc_t *sc)
{
	lmc_trace(sc->lmc_device, "lmc_proto_close in");

	if (sc->if_type == LMC_PPP)
		hdlc_close(sc->lmc_device);

	lmc_trace(sc->lmc_device, "lmc_proto_close out");
}

__be16 lmc_proto_type(lmc_softc_t *sc, struct sk_buff *skb) /*FOLD00*/
{
    lmc_trace(sc->lmc_device, "lmc_proto_type in");
    switch(sc->if_type){
    case LMC_PPP:
	    return hdlc_type_trans(skb, sc->lmc_device);
	    break;
    case LMC_NET:
        return htons(ETH_P_802_2);
        break;
    case LMC_RAW: /* Packet type for skbuff kind of useless */
        return htons(ETH_P_802_2);
        break;
    default:
        printk(KERN_WARNING "%s: No protocol set for this interface, assuming 802.2 (which is wrong!!)\n", sc->name);
        return htons(ETH_P_802_2);
        break;
    }
    lmc_trace(sc->lmc_device, "lmc_proto_tye out");

}

void lmc_proto_netif(lmc_softc_t *sc, struct sk_buff *skb) /*FOLD00*/
{
    lmc_trace(sc->lmc_device, "lmc_proto_netif in");
    switch(sc->if_type){
    case LMC_PPP:
    case LMC_NET:
    default:
        netif_rx(skb);
        break;
    case LMC_RAW:
        break;
    }
    lmc_trace(sc->lmc_device, "lmc_proto_netif out");
}
