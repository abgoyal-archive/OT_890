# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.

# MediaTek Inc. (C) 2010. All rights reserved.
#
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
# RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
# AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
# AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.


#
# This is a simple Makefile to test some of the RAID-6 code
# from userspace.
#

CC	 = gcc
OPTFLAGS = -O2			# Adjust as desired
CFLAGS	 = -I.. -g $(OPTFLAGS)
LD	 = ld
PERL	 = perl
AR	 = ar
RANLIB	 = ranlib

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

%.c: ../%.c
	cp -f $< $@

%.uc: ../%.uc
	cp -f $< $@

all:	raid6.a raid6test

raid6.a: raid6int1.o raid6int2.o raid6int4.o raid6int8.o raid6int16.o \
	 raid6int32.o \
	 raid6mmx.o raid6sse1.o raid6sse2.o \
	 raid6altivec1.o raid6altivec2.o raid6altivec4.o raid6altivec8.o \
	 raid6recov.o raid6algos.o \
	 raid6tables.o
	 rm -f $@
	 $(AR) cq $@ $^
	 $(RANLIB) $@

raid6test: test.c raid6.a
	$(CC) $(CFLAGS) -o raid6test $^

raid6altivec1.c: raid6altivec.uc ../unroll.pl
	$(PERL) ../unroll.pl 1 < raid6altivec.uc > $@

raid6altivec2.c: raid6altivec.uc ../unroll.pl
	$(PERL) ../unroll.pl 2 < raid6altivec.uc > $@

raid6altivec4.c: raid6altivec.uc ../unroll.pl
	$(PERL) ../unroll.pl 4 < raid6altivec.uc > $@

raid6altivec8.c: raid6altivec.uc ../unroll.pl
	$(PERL) ../unroll.pl 8 < raid6altivec.uc > $@

raid6int1.c: raid6int.uc ../unroll.pl
	$(PERL) ../unroll.pl 1 < raid6int.uc > $@

raid6int2.c: raid6int.uc ../unroll.pl
	$(PERL) ../unroll.pl 2 < raid6int.uc > $@

raid6int4.c: raid6int.uc ../unroll.pl
	$(PERL) ../unroll.pl 4 < raid6int.uc > $@

raid6int8.c: raid6int.uc ../unroll.pl
	$(PERL) ../unroll.pl 8 < raid6int.uc > $@

raid6int16.c: raid6int.uc ../unroll.pl
	$(PERL) ../unroll.pl 16 < raid6int.uc > $@

raid6int32.c: raid6int.uc ../unroll.pl
	$(PERL) ../unroll.pl 32 < raid6int.uc > $@

raid6tables.c: mktables
	./mktables > raid6tables.c

clean:
	rm -f *.o *.a mktables mktables.c raid6int.uc raid6*.c raid6test

spotless: clean
	rm -f *~
