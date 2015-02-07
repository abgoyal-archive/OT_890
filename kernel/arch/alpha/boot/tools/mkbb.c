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

/* This utility makes a bootblock suitable for the SRM console/miniloader */

/* Usage:
 *	mkbb <device> <lxboot>
 *
 * Where <device> is the name of the device to install the bootblock on,
 * and <lxboot> is the name of a bootblock to merge in.  This bootblock
 * contains the offset and size of the bootloader.  It must be exactly
 * 512 bytes long.
 */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/* Minimal definition of disklabel, so we don't have to include
 * asm/disklabel.h (confuses make)
 */
#ifndef MAXPARTITIONS
#define MAXPARTITIONS   8                       /* max. # of partitions */
#endif

#ifndef u8
#define u8 unsigned char
#endif

#ifndef u16
#define u16 unsigned short
#endif

#ifndef u32
#define u32 unsigned int
#endif

struct disklabel {
    u32	d_magic;				/* must be DISKLABELMAGIC */
    u16	d_type, d_subtype;
    u8	d_typename[16];
    u8	d_packname[16];
    u32	d_secsize;
    u32	d_nsectors;
    u32	d_ntracks;
    u32	d_ncylinders;
    u32	d_secpercyl;
    u32	d_secprtunit;
    u16	d_sparespertrack;
    u16	d_sparespercyl;
    u32	d_acylinders;
    u16	d_rpm, d_interleave, d_trackskew, d_cylskew;
    u32	d_headswitch, d_trkseek, d_flags;
    u32	d_drivedata[5];
    u32	d_spare[5];
    u32	d_magic2;				/* must be DISKLABELMAGIC */
    u16	d_checksum;
    u16	d_npartitions;
    u32	d_bbsize, d_sbsize;
    struct d_partition {
	u32	p_size;
	u32	p_offset;
	u32	p_fsize;
	u8	p_fstype;
	u8	p_frag;
	u16	p_cpg;
    } d_partitions[MAXPARTITIONS];
};


typedef union __bootblock {
    struct {
        char			__pad1[64];
        struct disklabel	__label;
    } __u1;
    struct {
	unsigned long		__pad2[63];
	unsigned long		__checksum;
    } __u2;
    char		bootblock_bytes[512];
    unsigned long	bootblock_quadwords[64];
} bootblock;

#define	bootblock_label		__u1.__label
#define bootblock_checksum	__u2.__checksum

int main(int argc, char ** argv)
{
    bootblock		bootblock_from_disk;
    bootblock		bootloader_image;
    int			dev, fd;
    int			i;
    int			nread;

    /* Make sure of the arg count */
    if(argc != 3) {
	fprintf(stderr, "Usage: %s device lxboot\n", argv[0]);
	exit(0);
    }

    /* First, open the device and make sure it's accessible */
    dev = open(argv[1], O_RDWR);
    if(dev < 0) {
	perror(argv[1]);
	exit(0);
    }

    /* Now open the lxboot and make sure it's reasonable */
    fd = open(argv[2], O_RDONLY);
    if(fd < 0) {
	perror(argv[2]);
	close(dev);
	exit(0);
    }

    /* Read in the lxboot */
    nread = read(fd, &bootloader_image, sizeof(bootblock));
    if(nread != sizeof(bootblock)) {
	perror("lxboot read");
	fprintf(stderr, "expected %zd, got %d\n", sizeof(bootblock), nread);
	exit(0);
    }

    /* Read in the bootblock from disk. */
    nread = read(dev, &bootblock_from_disk, sizeof(bootblock));
    if(nread != sizeof(bootblock)) {
	perror("bootblock read");
	fprintf(stderr, "expected %zd, got %d\n", sizeof(bootblock), nread);
	exit(0);
    }

    /* Swap the bootblock's disklabel into the bootloader */
    bootloader_image.bootblock_label = bootblock_from_disk.bootblock_label;

    /* Calculate the bootblock checksum */
    bootloader_image.bootblock_checksum = 0;
    for(i = 0; i < 63; i++) {
	bootloader_image.bootblock_checksum += 
			bootloader_image.bootblock_quadwords[i];
    }

    /* Write the whole thing out! */
    lseek(dev, 0L, SEEK_SET);
    if(write(dev, &bootloader_image, sizeof(bootblock)) != sizeof(bootblock)) {
	perror("bootblock write");
	exit(0);
    }

    close(fd);
    close(dev);
    exit(0);
}


