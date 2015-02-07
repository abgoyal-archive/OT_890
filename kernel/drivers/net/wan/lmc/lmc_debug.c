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

#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/interrupt.h>

#include "lmc_debug.h"

/*
 * Prints out len, max to 80 octets using printk, 20 per line
 */
#ifdef DEBUG
#ifdef LMC_PACKET_LOG
void lmcConsoleLog(char *type, unsigned char *ucData, int iLen)
{
  int iNewLine = 1;
  char str[80], *pstr;
  
  sprintf(str, KERN_DEBUG "lmc: %s: ", type);
  pstr = str+strlen(str);
  
  if(iLen > 240){
      printk(KERN_DEBUG "lmc: Printing 240 chars... out of: %d\n", iLen);
    iLen = 240;
  }
  else{
      printk(KERN_DEBUG "lmc: Printing %d chars\n", iLen);
  }

  while(iLen > 0) 
    {
      sprintf(pstr, "%02x ", *ucData);
      pstr+=3;
      ucData++;
      if( !(iNewLine % 20))
	{
	  sprintf(pstr, "\n");
	  printk(str);
	  sprintf(str, KERN_DEBUG "lmc: %s: ", type);
	  pstr=str+strlen(str);
	}
      iNewLine++;
      iLen--;
    }
  sprintf(pstr, "\n");
  printk(str);
}
#endif
#endif

#ifdef DEBUG
u32 lmcEventLogIndex;
u32 lmcEventLogBuf[LMC_EVENTLOGSIZE * LMC_EVENTLOGARGS];

void lmcEventLog(u32 EventNum, u32 arg2, u32 arg3)
{
  lmcEventLogBuf[lmcEventLogIndex++] = EventNum;
  lmcEventLogBuf[lmcEventLogIndex++] = arg2;
  lmcEventLogBuf[lmcEventLogIndex++] = arg3;
  lmcEventLogBuf[lmcEventLogIndex++] = jiffies;

  lmcEventLogIndex &= (LMC_EVENTLOGSIZE * LMC_EVENTLOGARGS) - 1;
}
#endif  /*  DEBUG  */

void lmc_trace(struct net_device *dev, char *msg){
#ifdef LMC_TRACE
    unsigned long j = jiffies + 3; /* Wait for 50 ms */

    if(in_interrupt()){
        printk("%s: * %s\n", dev->name, msg);
//        while(time_before(jiffies, j+10))
//            ;
    }
    else {
        printk("%s: %s\n", dev->name, msg);
        while(time_before(jiffies, j))
            schedule();
    }
#endif
}


/* --------------------------- end if_lmc_linux.c ------------------------ */
