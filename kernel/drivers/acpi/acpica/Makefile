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
# Makefile for ACPICA Core interpreter
#

ccflags-y			:= -Os
ccflags-$(CONFIG_ACPI_DEBUG)	+= -DACPI_DEBUG_OUTPUT

obj-y := dsfield.o   dsmthdat.o  dsopcode.o  dswexec.o  dswscope.o \
	 dsmethod.o  dsobject.o  dsutils.o   dswload.o  dswstate.o \
	 dsinit.o

obj-y += evevent.o  evregion.o  evsci.o    evxfevnt.o \
	 evmisc.o   evrgnini.o  evxface.o  evxfregn.o \
	 evgpe.o    evgpeblk.o

obj-y += exconfig.o  exfield.o  exnames.o   exoparg6.o  exresolv.o  exstorob.o\
	 exconvrt.o  exfldio.o  exoparg1.o  exprep.o    exresop.o   exsystem.o\
	 excreate.o  exmisc.o   exoparg2.o  exregion.o  exstore.o   exutils.o \
	 exdump.o    exmutex.o  exoparg3.o  exresnte.o  exstoren.o

obj-y += hwacpi.o  hwgpe.o  hwregs.o  hwsleep.o hwxface.o

obj-$(ACPI_FUTURE_USAGE) += hwtimer.o

obj-y += nsaccess.o  nsload.o    nssearch.o  nsxfeval.o \
	 nsalloc.o   nseval.o    nsnames.o   nsutils.o   nsxfname.o \
	 nsdump.o    nsinit.o    nsobject.o  nswalk.o    nsxfobj.o  \
	 nsparse.o   nspredef.o

obj-$(ACPI_FUTURE_USAGE) += nsdumpdv.o

obj-y += psargs.o    psparse.o  psloop.o pstree.o   pswalk.o  \
	 psopcode.o  psscope.o  psutils.o  psxface.o

obj-y += rsaddr.o rscreate.o rsinfo.o rsio.o rslist.o rsmisc.o rsxface.o \
	 rscalc.o  rsirq.o  rsmemory.o  rsutils.o

obj-$(ACPI_FUTURE_USAGE) += rsdump.o

obj-y += tbxface.o tbinstal.o tbutils.o tbfind.o tbfadt.o tbxfroot.o

obj-y += utalloc.o utdebug.o uteval.o utinit.o utmisc.o utxface.o \
		utcopy.o utdelete.o utglobal.o utmath.o utobject.o \
		utstate.o utmutex.o utobject.o utresrc.o
