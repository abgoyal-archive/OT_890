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
 * DTC controller, taken from T128 driver by...
 * Copyright 1993, Drew Eckhardt
 *	Visionary Computing
 *	(Unix and Linux consulting and custom programming)
 *	drew@colorado.edu
 *      +1 (303) 440-4894
 *
 * DISTRIBUTION RELEASE 2. 
 *
 * For more information, please consult 
 *
 * 
 * 
 * and 
 *
 * NCR 5380 Family
 * SCSI Protocol Controller
 * Databook
 *
 * NCR Microelectronics
 * 1635 Aeroplaza Drive
 * Colorado Springs, CO 80916
 * 1+ (719) 578-3400
 * 1+ (800) 334-5454
 */

#ifndef DTC3280_H
#define DTC3280_H

#define DTCDEBUG 0
#define DTCDEBUG_INIT	0x1
#define DTCDEBUG_TRANSFER 0x2

static int dtc_abort(Scsi_Cmnd *);
static int dtc_biosparam(struct scsi_device *, struct block_device *,
		         sector_t, int*);
static int dtc_detect(struct scsi_host_template *);
static int dtc_queue_command(Scsi_Cmnd *, void (*done)(Scsi_Cmnd *));
static int dtc_bus_reset(Scsi_Cmnd *);

#ifndef CMD_PER_LUN
#define CMD_PER_LUN 2
#endif

#ifndef CAN_QUEUE
#define CAN_QUEUE 32 
#endif

#define NCR5380_implementation_fields \
    void __iomem *base

#define NCR5380_local_declare() \
    void __iomem *base

#define NCR5380_setup(instance) \
    base = ((struct NCR5380_hostdata *)(instance)->hostdata)->base

#define DTC_address(reg) (base + DTC_5380_OFFSET + reg)

#define dbNCR5380_read(reg)                                              \
    (rval=readb(DTC_address(reg)), \
     (((unsigned char) printk("DTC : read register %d at addr %p is: %02x\n"\
    , (reg), DTC_address(reg), rval)), rval ) )

#define dbNCR5380_write(reg, value) do {                                  \
    printk("DTC : write %02x to register %d at address %p\n",         \
            (value), (reg), DTC_address(reg));     \
    writeb(value, DTC_address(reg));} while(0)


#if !(DTCDEBUG & DTCDEBUG_TRANSFER) 
#define NCR5380_read(reg) (readb(DTC_address(reg)))
#define NCR5380_write(reg, value) (writeb(value, DTC_address(reg)))
#else
#define NCR5380_read(reg) (readb(DTC_address(reg)))
#define xNCR5380_read(reg)						\
    (((unsigned char) printk("DTC : read register %d at address %p\n"\
    , (reg), DTC_address(reg))), readb(DTC_address(reg)))

#define NCR5380_write(reg, value) do {					\
    printk("DTC : write %02x to register %d at address %p\n", 	\
	    (value), (reg), DTC_address(reg));	\
    writeb(value, DTC_address(reg));} while(0)
#endif

#define NCR5380_intr			dtc_intr
#define NCR5380_queue_command		dtc_queue_command
#define NCR5380_abort			dtc_abort
#define NCR5380_bus_reset		dtc_bus_reset
#define NCR5380_proc_info		dtc_proc_info 

/* 15 12 11 10
   1001 1100 0000 0000 */

#define DTC_IRQS 0x9c00


#endif /* DTC3280_H */
