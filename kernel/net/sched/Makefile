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
# Makefile for the Linux Traffic Control Unit.
#

obj-y	:= sch_generic.o

obj-$(CONFIG_NET_SCHED)		+= sch_api.o sch_blackhole.o
obj-$(CONFIG_NET_CLS)		+= cls_api.o
obj-$(CONFIG_NET_CLS_ACT)	+= act_api.o
obj-$(CONFIG_NET_ACT_POLICE)	+= act_police.o
obj-$(CONFIG_NET_ACT_GACT)	+= act_gact.o
obj-$(CONFIG_NET_ACT_MIRRED)	+= act_mirred.o
obj-$(CONFIG_NET_ACT_IPT)	+= act_ipt.o
obj-$(CONFIG_NET_ACT_NAT)	+= act_nat.o
obj-$(CONFIG_NET_ACT_PEDIT)	+= act_pedit.o
obj-$(CONFIG_NET_ACT_SIMP)	+= act_simple.o
obj-$(CONFIG_NET_ACT_SKBEDIT)	+= act_skbedit.o
obj-$(CONFIG_NET_SCH_FIFO)	+= sch_fifo.o
obj-$(CONFIG_NET_SCH_CBQ)	+= sch_cbq.o
obj-$(CONFIG_NET_SCH_HTB)	+= sch_htb.o
obj-$(CONFIG_NET_SCH_HFSC)	+= sch_hfsc.o
obj-$(CONFIG_NET_SCH_RED)	+= sch_red.o
obj-$(CONFIG_NET_SCH_GRED)	+= sch_gred.o
obj-$(CONFIG_NET_SCH_INGRESS)	+= sch_ingress.o 
obj-$(CONFIG_NET_SCH_DSMARK)	+= sch_dsmark.o
obj-$(CONFIG_NET_SCH_SFQ)	+= sch_sfq.o
obj-$(CONFIG_NET_SCH_TBF)	+= sch_tbf.o
obj-$(CONFIG_NET_SCH_TEQL)	+= sch_teql.o
obj-$(CONFIG_NET_SCH_PRIO)	+= sch_prio.o
obj-$(CONFIG_NET_SCH_MULTIQ)	+= sch_multiq.o
obj-$(CONFIG_NET_SCH_ATM)	+= sch_atm.o
obj-$(CONFIG_NET_SCH_NETEM)	+= sch_netem.o
obj-$(CONFIG_NET_SCH_DRR)	+= sch_drr.o
obj-$(CONFIG_NET_CLS_U32)	+= cls_u32.o
obj-$(CONFIG_NET_CLS_ROUTE4)	+= cls_route.o
obj-$(CONFIG_NET_CLS_FW)	+= cls_fw.o
obj-$(CONFIG_NET_CLS_RSVP)	+= cls_rsvp.o
obj-$(CONFIG_NET_CLS_TCINDEX)	+= cls_tcindex.o
obj-$(CONFIG_NET_CLS_RSVP6)	+= cls_rsvp6.o
obj-$(CONFIG_NET_CLS_BASIC)	+= cls_basic.o
obj-$(CONFIG_NET_CLS_FLOW)	+= cls_flow.o
obj-$(CONFIG_NET_CLS_CGROUP)	+= cls_cgroup.o
obj-$(CONFIG_NET_EMATCH)	+= ematch.o
obj-$(CONFIG_NET_EMATCH_CMP)	+= em_cmp.o
obj-$(CONFIG_NET_EMATCH_NBYTE)	+= em_nbyte.o
obj-$(CONFIG_NET_EMATCH_U32)	+= em_u32.o
obj-$(CONFIG_NET_EMATCH_META)	+= em_meta.o
obj-$(CONFIG_NET_EMATCH_TEXT)	+= em_text.o
