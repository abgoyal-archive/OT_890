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


PROG=	aicasm

.SUFFIXES= .l .y .c .h

CSRCS=	aicasm.c aicasm_symbol.c
YSRCS=	aicasm_gram.y aicasm_macro_gram.y
LSRCS=	aicasm_scan.l aicasm_macro_scan.l

GENHDRS=	aicdb.h $(YSRCS:.y=.h)
GENSRCS=	$(YSRCS:.y=.c) $(LSRCS:.l=.c)

SRCS=	${CSRCS} ${GENSRCS}
LIBS=	-ldb
clean-files:= ${GENSRCS} ${GENHDRS} $(YSRCS:.y=.output) $(PROG)
# Override default kernel CFLAGS.  This is a userland app.
AICASM_CFLAGS:= -I/usr/include -I.
LEX= flex
YACC= bison
YFLAGS= -d

NOMAN=	noman

ifneq ($(HOSTCC),)
AICASM_CC= $(HOSTCC)
else
AICASM_CC= $(CC)
endif

ifdef DEBUG
CFLAGS+= -DDEBUG -g
YFLAGS+= -t -v
LFLAGS= -d
endif

$(PROG):  ${GENHDRS} $(SRCS)
	$(AICASM_CC) $(AICASM_CFLAGS) $(SRCS) -o $(PROG) $(LIBS)

aicdb.h:
	@if [ -e "/usr/include/db4/db_185.h" ]; then		\
		echo "#include <db4/db_185.h>" > aicdb.h;	\
	 elif [ -e "/usr/include/db3/db_185.h" ]; then		\
		echo "#include <db3/db_185.h>" > aicdb.h;	\
	 elif [ -e "/usr/include/db2/db_185.h" ]; then		\
		echo "#include <db2/db_185.h>" > aicdb.h;	\
	 elif [ -e "/usr/include/db1/db_185.h" ]; then		\
		echo "#include <db1/db_185.h>" > aicdb.h;	\
	 elif [ -e "/usr/include/db/db_185.h" ]; then		\
		echo "#include <db/db_185.h>" > aicdb.h;	\
	 elif [ -e "/usr/include/db_185.h" ]; then		\
		echo "#include <db_185.h>" > aicdb.h;		\
	 else							\
		echo "*** Install db development libraries";	\
	 fi

clean:
	rm -f $(clean-files)

# Create a dependency chain in generated files
# to avoid concurrent invocations of the single
# rule that builds them all.
aicasm_gram.c: aicasm_gram.h
aicasm_gram.c aicasm_gram.h: aicasm_gram.y
	$(YACC) $(YFLAGS) -b $(<:.y=) $<
	mv $(<:.y=).tab.c $(<:.y=.c)
	mv $(<:.y=).tab.h $(<:.y=.h)

# Create a dependency chain in generated files
# to avoid concurrent invocations of the single
# rule that builds them all.
aicasm_macro_gram.c: aicasm_macro_gram.h
aicasm_macro_gram.c aicasm_macro_gram.h: aicasm_macro_gram.y
	$(YACC) $(YFLAGS) -b $(<:.y=) -p mm $<
	mv $(<:.y=).tab.c $(<:.y=.c)
	mv $(<:.y=).tab.h $(<:.y=.h)

aicasm_scan.c: aicasm_scan.l
	$(LEX) $(LFLAGS) -o$@ $<

aicasm_macro_scan.c: aicasm_macro_scan.l
	$(LEX) $(LFLAGS) -Pmm -o$@ $<
