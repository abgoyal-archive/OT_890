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
 * addinitrd - program to add a initrd image to an ecoff kernel
 *
 * (C) 1999 Thomas Bogendoerfer
 * minor modifications, cleanup: Guido Guenther <agx@sigxcpu.org>
 * further cleanup: Maciej W. Rozycki
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>

#include "ecoff.h"

#define MIPS_PAGE_SIZE	4096
#define MIPS_PAGE_MASK	(MIPS_PAGE_SIZE-1)

#define swab16(x) \
        ((unsigned short)( \
                (((unsigned short)(x) & (unsigned short)0x00ffU) << 8) | \
                (((unsigned short)(x) & (unsigned short)0xff00U) >> 8) ))

#define swab32(x) \
        ((unsigned int)( \
                (((unsigned int)(x) & (unsigned int)0x000000ffUL) << 24) | \
                (((unsigned int)(x) & (unsigned int)0x0000ff00UL) <<  8) | \
                (((unsigned int)(x) & (unsigned int)0x00ff0000UL) >>  8) | \
                (((unsigned int)(x) & (unsigned int)0xff000000UL) >> 24) ))

#define SWAB(a)	(swab ? swab32(a) : (a))

void die(char *s)
{
	perror(s);
	exit(1);
}

int main(int argc, char *argv[])
{
	int fd_vmlinux, fd_initrd, fd_outfile;
	FILHDR efile;
	AOUTHDR eaout;
	SCNHDR esecs[3];
	struct stat st;
	char buf[1024];
	unsigned long loadaddr;
	unsigned long initrd_header[2];
	int i, cnt;
	int swab = 0;

	if (argc != 4) {
		printf("Usage: %s <vmlinux> <initrd> <outfile>\n", argv[0]);
		exit(1);
	}

	if ((fd_vmlinux = open (argv[1], O_RDONLY)) < 0)
		 die("open vmlinux");
	if (read (fd_vmlinux, &efile, sizeof efile) != sizeof efile)
		die("read file header");
	if (read (fd_vmlinux, &eaout, sizeof eaout) != sizeof eaout)
		die("read aout header");
	if (read (fd_vmlinux, esecs, sizeof esecs) != sizeof esecs)
		die("read section headers");
	/*
	 * check whether the file is good for us
	 */
	/* TBD */

	/*
	 * check, if we have to swab words
	 */
	if (ntohs(0xaa55) == 0xaa55) {
		if (efile.f_magic == swab16(MIPSELMAGIC))
			swab = 1;
	} else {
		if (efile.f_magic == swab16(MIPSEBMAGIC))
			swab = 1;
	}

	/* make sure we have an empty data segment for the initrd */
	if (eaout.dsize || esecs[1].s_size) {
		fprintf(stderr, "Data segment not empty. Giving up!\n");
		exit(1);
	}
	if ((fd_initrd = open (argv[2], O_RDONLY)) < 0)
		die("open initrd");
	if (fstat (fd_initrd, &st) < 0)
		die("fstat initrd");
	loadaddr = ((SWAB(esecs[2].s_vaddr) + SWAB(esecs[2].s_size)
			+ MIPS_PAGE_SIZE-1) & ~MIPS_PAGE_MASK) - 8;
	if (loadaddr < (SWAB(esecs[2].s_vaddr) + SWAB(esecs[2].s_size)))
		loadaddr += MIPS_PAGE_SIZE;
	initrd_header[0] = SWAB(0x494E5244);
	initrd_header[1] = SWAB(st.st_size);
	eaout.dsize = esecs[1].s_size = initrd_header[1] = SWAB(st.st_size+8);
	eaout.data_start = esecs[1].s_vaddr = esecs[1].s_paddr = SWAB(loadaddr);

	if ((fd_outfile = open (argv[3], O_RDWR|O_CREAT|O_TRUNC, 0666)) < 0)
		die("open outfile");
	if (write (fd_outfile, &efile, sizeof efile) != sizeof efile)
		die("write file header");
	if (write (fd_outfile, &eaout, sizeof eaout) != sizeof eaout)
		die("write aout header");
	if (write (fd_outfile, esecs, sizeof esecs) != sizeof esecs)
		die("write section headers");
	/* skip padding */
	if(lseek(fd_vmlinux, SWAB(esecs[0].s_scnptr), SEEK_SET) == (off_t)-1)
		die("lseek vmlinux");
	if(lseek(fd_outfile, SWAB(esecs[0].s_scnptr), SEEK_SET) == (off_t)-1)
		die("lseek outfile");
	/* copy text segment */
	cnt = SWAB(eaout.tsize);
	while (cnt) {
		if ((i = read (fd_vmlinux, buf, sizeof buf)) <= 0)
			die("read vmlinux");
		if (write (fd_outfile, buf, i) != i)
			die("write vmlinux");
		cnt -= i;
	}
	if (write (fd_outfile, initrd_header, sizeof initrd_header) != sizeof initrd_header)
		die("write initrd header");
	while ((i = read (fd_initrd, buf, sizeof buf)) > 0)
		if (write (fd_outfile, buf, i) != i)
			die("write initrd");
	close(fd_vmlinux);
	close(fd_initrd);
	return 0;
}
