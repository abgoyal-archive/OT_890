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

#ifndef _LMC_DEBUG_H_
#define _LMC_DEBUG_H_

#ifdef DEBUG
#ifdef LMC_PACKET_LOG
#define LMC_CONSOLE_LOG(x,y,z) lmcConsoleLog((x), (y), (z))
#else
#define LMC_CONSOLE_LOG(x,y,z)
#endif
#else
#define LMC_CONSOLE_LOG(x,y,z)
#endif



/* Debug --- Event log definitions --- */
/* EVENTLOGSIZE*EVENTLOGARGS needs to be a power of 2 */
#define LMC_EVENTLOGSIZE 1024	/* number of events in eventlog */
#define LMC_EVENTLOGARGS 4		/* number of args for each event */

/* event indicators */
#define LMC_EVENT_XMT           1
#define LMC_EVENT_XMTEND        2
#define LMC_EVENT_XMTINT        3
#define LMC_EVENT_RCVINT        4
#define LMC_EVENT_RCVEND        5
#define LMC_EVENT_INT           6
#define LMC_EVENT_XMTINTTMO     7
#define LMC_EVENT_XMTPRCTMO     8
#define LMC_EVENT_INTEND        9
#define LMC_EVENT_RESET1       10
#define LMC_EVENT_RESET2       11
#define LMC_EVENT_FORCEDRESET  12
#define LMC_EVENT_WATCHDOG     13
#define LMC_EVENT_BADPKTSURGE  14
#define LMC_EVENT_TBUSY0       15
#define LMC_EVENT_TBUSY1       16


#ifdef DEBUG
extern u32 lmcEventLogIndex;
extern u32 lmcEventLogBuf[LMC_EVENTLOGSIZE * LMC_EVENTLOGARGS];
#define LMC_EVENT_LOG(x, y, z) lmcEventLog((x), (y), (z))
#else
#define LMC_EVENT_LOG(x,y,z)
#endif /* end ifdef _DBG_EVENTLOG */

void lmcConsoleLog(char *type, unsigned char *ucData, int iLen);
void lmcEventLog(u32 EventNum, u32 arg2, u32 arg3);
void lmc_trace(struct net_device *dev, char *msg);

#endif
