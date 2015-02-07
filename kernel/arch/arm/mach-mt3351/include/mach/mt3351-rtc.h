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
 *  MT3351 Real Time Clock header file
 *
 *  Copyright (C) 2008 MTK
 *
 *
 */
#ifndef __MT3351_RTC_H

/******************************************************************************
MT3351 RTC Register Definition
******************************************************************************/
#include <mach/mt3351_reg_base.h>
#define MT3351_RTC_PROT1       ((volatile unsigned int   *)(RTC_base+0x0004))   
#define MT3351_RTC_PROT2       ((volatile unsigned int   *)(RTC_base+0x0008))   
#define MT3351_RTC_PROT3       ((volatile unsigned int   *)(RTC_base+0x000C))   
#define MT3351_RTC_PROT4       ((volatile unsigned int   *)(RTC_base+0x0010))   
#define MT3351_RTC_KEY         ((volatile unsigned int   *)(RTC_base+0x0014))   
#define MT3351_RTC_CTL         ((volatile unsigned int   *)(RTC_base+0x0018))  
#define MT3351_RTC_TC_YEA      ((volatile unsigned int   *)(RTC_base+0x001c))   
#define MT3351_RTC_TC_MON      ((volatile unsigned int   *)(RTC_base+0x0020))   
#define MT3351_RTC_TC_DOM      ((volatile unsigned int   *)(RTC_base+0x0024))   
#define MT3351_RTC_TC_DOW      ((volatile unsigned int   *)(RTC_base+0x0028))   
#define MT3351_RTC_TC_HOU      ((volatile unsigned int   *)(RTC_base+0x002C))   
#define MT3351_RTC_TC_MIN      ((volatile unsigned int   *)(RTC_base+0x0030))   
#define MT3351_RTC_TC_SEC      ((volatile unsigned int   *)(RTC_base+0x0034))   
#define MT3351_RTC_AL_CTL      ((volatile unsigned int   *)(RTC_base+0x0038))   
#define MT3351_RTC_AL_YEAR     ((volatile unsigned int   *)(RTC_base+0x003c))   
#define MT3351_RTC_AL_MON      ((volatile unsigned int   *)(RTC_base+0x0040))   
#define MT3351_RTC_AL_DOM      ((volatile unsigned int   *)(RTC_base+0x0044))   

#define MT3351_RTC_AL_DOW      ((volatile unsigned int   *)(RTC_base+0x0048))  
#define MT3351_RTC_AL_HOU      ((volatile unsigned int   *)(RTC_base+0x004c))   
#define MT3351_RTC_AL_MIN      ((volatile unsigned int   *)(RTC_base+0x0050))   
#define MT3351_RTC_AL_SEC      ((volatile unsigned int   *)(RTC_base+0x0054))   

#define MT3351_RTC_RIP_CTL     ((volatile unsigned int   *)(RTC_base+0x0058))   
#define MT3351_RTC_RIP_CNTH    ((volatile unsigned int   *)(RTC_base+0x005c))  
#define MT3351_RTC_RIP_CNTL    ((volatile unsigned int   *)(RTC_base+0x0060))  
#define MT3351_RTC_TIMER_CTL   ((volatile unsigned int   *)(RTC_base+0x0064))   

#define MT3351_RTC_TIMER_CNTH  ((volatile unsigned int   *)(RTC_base+0x0068))   
#define MT3351_RTC_TIMER_CNTL  ((volatile unsigned int   *)(RTC_base+0x006c))   

#define MT3351_RTC_XOSC_CFG    ((volatile unsigned int   *)(RTC_base+0x0070))   
#define MT3351_RTC_DEBOUNCE    ((volatile unsigned int   *)(RTC_base+0x0074))   

#define MT3351_RTC_WAVEOUT     ((volatile unsigned int   *)(RTC_base+0x0078))   
#define MT3351_RTC_IRQ_STA     ((volatile unsigned int   *)(RTC_base+0x007c))  


// RTC IRQ Status
#define MT3351_RTC_STA_ALARM_MASK 0x01
#define MT3351_RTC_STA_TIMER_MASK 0x02

#define MT3351_RTC_STA_ALARM      0x01
#define MT3351_RTC_STA_TIMER      0x02
//RTC internal control registers
#define MT3351_RC_STOP_MASK       0x01
#define MT3351_SIM_RTC_MASK       0x02
#define MT3351_RESERVE1_MASK      0x04
#define MT3351_RESERVE2_MASK      0x08
#define MT3351_PORT_PASS_MASK     0x10
#define MT3351_INVALID_WR_MASK    0x20
#define MT3351_INHIBIT_WR_MASK    0x40
#define MT3351_DEBNCE_OK_MASK     0x80

//RTC Alarm control
#define MT3351_Alarm_EN_MASK      0x01
#define MT3351_Alarm_SEC_MASK     0x02
#define MT3351_Alarm_MIN_MASK     0x04
#define MT3351_Alarm_HOR_MASK     0x08
#define MT3351_Alarm_DOW_MASK     0x10
#define MT3351_Alarm_DOM_MASK     0x20
#define MT3351_Alarm_MON_MASK     0x40
#define MT3351_Alarm_YEA_MASK     0x80

// RTC_TIMER_CTL
#define MT3351_EXTEN_MASK         0x01
#define MT3351_INTEN_MASK         0x02
#define MT3351_DRV_MASK           0x0C
#define MT3351_32K_OUT_EN_MASK    0x10

#define MT3351_INTEN_ON           0x02
#define MT3351_TR_DRV             0x08   
// Software Key
#define MT3351_RTC_KEY_VALUE      0x12

// hardware Key
#define RTC_PROT1_KEY             0xA3
#define RTC_PROT2_KEY             0x57
#define RTC_PROT3_KEY             0x67
#define RTC_PROT4_KEY             0xD2

// default year to second for RTC timer
#define MT3351_RTC_YEAR           8
#define MT3351_RTC_MON            1
#define MT3351_RTC_DOM            1
#define MT3351_RTC_HOU            0
#define MT3351_RTC_MIN            0
#define MT3351_RTC_SEC            0

/*******************************************************************************
 * LOCAL TYPE DEFINATION
 ******************************************************************************/
#define MAX_DEBNCE_TIMES          0x1fffffff
#define RTC_DEVICE	              "mt3351-rtc"
#define MT3351_RTC_VERSION	      "v 0.2"

/* Those are the bits from a classic RTC we want to mimic */
#define RTC_IRQF		          0x80	/* any of the following 3 is active */
#define RTC_PF			          0x40
#define RTC_AF		              0x20
#define RTC_UF		              0x10
#define RTC_IS_OPEN		          0x01	/* means /dev/rtc is in use	*/

/* When simulating PIRQ, int handler resets timer counter each time. 
 * Before the next time counter checking(when counter =0, interrupt happens)
 * it need  1/32 sec (1 tick of the timer).
 */ 
#define TIMER_SETCNT_TIME         1 
//bool type
#ifndef bool
    #define bool                  unsigned char
#endif
#ifndef FALSE
    #define false                 0
#endif

#ifndef TRUE
    #define true                  1
#endif


/******************************************************************************
Function Prototype
******************************************************************************/
/*
extern static bool mt3351_rtc_IsHWkeyReady(void);
extern static bool mt3351_rtc_IsProt_Pass(void);
extern static bool mt3351_rtc_IsDebounceOk(void);
extern static bool mt3351_rtc_IsInhibit_WR(void);
extern static ssize_t mt3351_rtc_read(struct file *file, char *buf,size_t count, loff_t *ppos);
extern static int mt3351_rtc_fasync (int fd, struct file *filp, int on);
extern static unsigned int mt3351_rtc_poll(struct file *file, poll_table *wait);
extern static int mt3351_rtc_proc_output(char *buf);
extern static int mt3351_rtc_read_proc(char *page, char **start, off_t off,int count, int *eof, void *data);
extern static void mt3351_rtc_gettimeofday(void);
extern static void mt3351_rtc_settimeofday(void);
extern static int mt3351_set_rtc(void);
extern static irqreturn_t mt3351_rtc_interrupt(int irq, void *dev_id, struct pt_regs *regs);
extern static void mt3351_rtc_power_inconsistent_init(void);
extern static int mt3351_rtc_enable(void);
extern static int mt3351_rtc_open(struct inode *inode, struct file *file);
extern static int mt3351_rtc_release(struct inode *inode, struct file *file);
extern static int mt3351_rtc_ioctl(struct inode *inode, struct file *file, unsigned int cmd,unsigned long arg);
*/
int get_rtc_time(struct device *dev,struct rtc_time *rtc_tm);
int set_rtc_time(struct device *dev, struct rtc_time *rtc_tm);
int get_rtc_alm_time(struct device *dev, struct rtc_time *rtc_tm);
int set_rtc_alm_time(struct device *dev, struct rtc_time *rtc_tm);
/*
void get_rtc_wkalm_time(struct device *dev, struct rtc_wkalrm *wkalrm_tm);
int set_rtc_wkalm_time(struct device *dev, struct rtc_wkalrm *wkalrm_tm);
*/
#endif
