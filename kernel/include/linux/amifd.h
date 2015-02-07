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

#ifndef _AMIFD_H
#define _AMIFD_H

/* Definitions for the Amiga floppy driver */

#include <linux/fd.h>

#define FD_MAX_UNITS    4	/* Max. Number of drives */
#define FLOPPY_MAX_SECTORS	22	/* Max. Number of sectors per track */

#ifndef ASSEMBLER

struct fd_data_type {
    char *name;			/* description of data type */
    int sects;			/* sectors per track */
#ifdef __STDC__
    int (*read_fkt)(int);
    void (*write_fkt)(int);
#else
    int (*read_fkt)();		/* read whole track */
    void (*write_fkt)();		/* write whole track */
#endif
};

/*
** Floppy type descriptions
*/

struct fd_drive_type {
    unsigned long code;		/* code returned from drive */
    char *name;			/* description of drive */
    unsigned int tracks;	/* number of tracks */
    unsigned int heads;		/* number of heads */
    unsigned int read_size;	/* raw read size for one track */
    unsigned int write_size;	/* raw write size for one track */
    unsigned int sect_mult;	/* sectors and gap multiplier (HD = 2) */
    unsigned int precomp1;	/* start track for precomp 1 */
    unsigned int precomp2;	/* start track for precomp 2 */
    unsigned int step_delay;	/* time (in ms) for delay after step */
    unsigned int settle_time;	/* time to settle after dir change */
    unsigned int side_time;	/* time needed to change sides */
};

struct amiga_floppy_struct {
    struct fd_drive_type *type;	/* type of floppy for this unit */
    struct fd_data_type *dtype;	/* type of floppy for this unit */
    int track;			/* current track (-1 == unknown) */
    unsigned char *trackbuf;    /* current track (kmaloc()'d */

    int blocks;			/* total # blocks on disk */

    int changed;		/* true when not known */
    int disk;			/* disk in drive (-1 == unknown) */
    int motor;			/* true when motor is at speed */
    int busy;			/* true when drive is active */
    int dirty;			/* true when trackbuf is not on disk */
    int status;			/* current error code for unit */
    struct gendisk *gendisk;
};
#endif

#endif
