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
 * hack-coff.c - hack the header of an xcoff file to fill in
 * a few fields needed by the Open Firmware xcoff loader on
 * Power Macs but not initialized by objcopy.
 *
 * Copyright (C) Paul Mackerras 1997.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "rs6000.h"

#define AOUT_MAGIC	0x010b

#define get_16be(x)	((((unsigned char *)(x))[0] << 8) \
			 + ((unsigned char *)(x))[1])
#define put_16be(x, v)	(((unsigned char *)(x))[0] = (v) >> 8, \
			 ((unsigned char *)(x))[1] = (v) & 0xff)
#define get_32be(x)	((((unsigned char *)(x))[0] << 24) \
			 + (((unsigned char *)(x))[1] << 16) \
			 + (((unsigned char *)(x))[2] << 8) \
			 + ((unsigned char *)(x))[3])

int
main(int ac, char **av)
{
    int fd;
    int i, nsect;
    int aoutsz;
    struct external_filehdr fhdr;
    AOUTHDR aout;
    struct external_scnhdr shdr;

    if (ac != 2) {
	fprintf(stderr, "Usage: hack-coff coff-file\n");
	exit(1);
    }
    if ((fd = open(av[1], 2)) == -1) {
	perror(av[2]);
	exit(1);
    }
    if (read(fd, &fhdr, sizeof(fhdr)) != sizeof(fhdr))
	goto readerr;
    i = get_16be(fhdr.f_magic);
    if (i != U802TOCMAGIC && i != U802WRMAGIC && i != U802ROMAGIC) {
	fprintf(stderr, "%s: not an xcoff file\n", av[1]);
	exit(1);
    }
    aoutsz = get_16be(fhdr.f_opthdr);
    if (read(fd, &aout, aoutsz) != aoutsz)
	goto readerr;
    nsect = get_16be(fhdr.f_nscns);
    for (i = 0; i < nsect; ++i) {
	if (read(fd, &shdr, sizeof(shdr)) != sizeof(shdr))
	    goto readerr;
	if (strcmp(shdr.s_name, ".text") == 0) {
	    put_16be(aout.o_snentry, i+1);
	    put_16be(aout.o_sntext, i+1);
	} else if (strcmp(shdr.s_name, ".data") == 0) {
	    put_16be(aout.o_sndata, i+1);
	} else if (strcmp(shdr.s_name, ".bss") == 0) {
	    put_16be(aout.o_snbss, i+1);
	}
    }
    put_16be(aout.magic, AOUT_MAGIC);
    if (lseek(fd, (long) sizeof(struct external_filehdr), 0) == -1
	|| write(fd, &aout, aoutsz) != aoutsz) {
	fprintf(stderr, "%s: write error\n", av[1]);
	exit(1);
    }
    close(fd);
    exit(0);

readerr:
    fprintf(stderr, "%s: read error or file too short\n", av[1]);
    exit(1);
}
