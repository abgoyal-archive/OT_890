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
   The compile-time configurable defaults for the Linux SCSI tape driver.

   Copyright 1995 Kai Makisara.
   
   Last modified: Wed Sep  2 21:24:07 1998 by root@home
   
   Changed (and renamed) for OnStream SCSI drives garloff@suse.de
   2000-06-21

   $Header$
*/

#ifndef _OSST_OPTIONS_H
#define _OSST_OPTIONS_H

/* The minimum limit for the number of SCSI tape devices is determined by
   OSST_MAX_TAPES. If the number of tape devices and the "slack" defined by
   OSST_EXTRA_DEVS exceeds OSST_MAX_TAPES, the large number is used. */
#define OSST_MAX_TAPES 4

/* If OSST_IN_FILE_POS is nonzero, the driver positions the tape after the
   record been read by the user program even if the tape has moved further
   because of buffered reads. Should be set to zero to support also drives
   that can't space backwards over records. NOTE: The tape will be
   spaced backwards over an "accidentally" crossed filemark in any case. */
#define OSST_IN_FILE_POS 1

/* The tape driver buffer size in kilobytes. */
/* Don't change, as this is the HW blocksize */
#define OSST_BUFFER_BLOCKS 32

/* The number of kilobytes of data in the buffer that triggers an
   asynchronous write in fixed block mode. See also OSST_ASYNC_WRITES
   below. */
#define OSST_WRITE_THRESHOLD_BLOCKS 32

/* OSST_EOM_RESERVE defines the number of frames are kept in reserve for
 *  * write error recovery when writing near end of medium. ENOSPC is returned
 *   * when write() is called and the tape write position is within this number
 *    * of blocks from the tape capacity. */
#define OSST_EOM_RESERVE 300

/* The maximum number of tape buffers the driver allocates. The number
   is also constrained by the number of drives detected. Determines the
   maximum number of concurrently active tape drives. */
#define OSST_MAX_BUFFERS OSST_MAX_TAPES 

/* Maximum number of scatter/gather segments */
/* Fit one buffer in pages and add one for the AUX header */
#define OSST_MAX_SG      (((OSST_BUFFER_BLOCKS*1024) / PAGE_SIZE) + 1)

/* The number of scatter/gather segments to allocate at first try (must be
   smaller or equal to the maximum). */
#define OSST_FIRST_SG    ((OSST_BUFFER_BLOCKS*1024) / PAGE_SIZE)

/* The size of the first scatter/gather segments (determines the maximum block
   size for SCSI adapters not supporting scatter/gather). The default is set
   to try to allocate the buffer as one chunk. */
#define OSST_FIRST_ORDER  (15-PAGE_SHIFT)


/* The following lines define defaults for properties that can be set
   separately for each drive using the MTSTOPTIONS ioctl. */

/* If OSST_TWO_FM is non-zero, the driver writes two filemarks after a
   file being written. Some drives can't handle two filemarks at the
   end of data. */
#define OSST_TWO_FM 0

/* If OSST_BUFFER_WRITES is non-zero, writes in fixed block mode are
   buffered until the driver buffer is full or asynchronous write is
   triggered. */
#define OSST_BUFFER_WRITES 1

/* If OSST_ASYNC_WRITES is non-zero, the SCSI write command may be started
   without waiting for it to finish. May cause problems in multiple
   tape backups. */
#define OSST_ASYNC_WRITES 1

/* If OSST_READ_AHEAD is non-zero, blocks are read ahead in fixed block
   mode. */
#define OSST_READ_AHEAD 1

/* If OSST_AUTO_LOCK is non-zero, the drive door is locked at the first
   read or write command after the device is opened. The door is opened
   when the device is closed. */
#define OSST_AUTO_LOCK 0

/* If OSST_FAST_MTEOM is non-zero, the MTEOM ioctl is done using the
   direct SCSI command. The file number status is lost but this method
   is fast with some drives. Otherwise MTEOM is done by spacing over
   files and the file number status is retained. */
#define OSST_FAST_MTEOM 0

/* If OSST_SCSI2LOGICAL is nonzero, the logical block addresses are used for
   MTIOCPOS and MTSEEK by default. Vendor addresses are used if OSST_SCSI2LOGICAL
   is zero. */
#define OSST_SCSI2LOGICAL 0

/* If OSST_SYSV is non-zero, the tape behaves according to the SYS V semantics.
   The default is BSD semantics. */
#define OSST_SYSV 0


#endif
