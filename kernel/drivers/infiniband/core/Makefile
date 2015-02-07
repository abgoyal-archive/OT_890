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


infiniband-$(CONFIG_INFINIBAND_ADDR_TRANS)	:= ib_addr.o rdma_cm.o
user_access-$(CONFIG_INFINIBAND_ADDR_TRANS)	:= rdma_ucm.o

obj-$(CONFIG_INFINIBAND) +=		ib_core.o ib_mad.o ib_sa.o \
					ib_cm.o iw_cm.o $(infiniband-y)
obj-$(CONFIG_INFINIBAND_USER_MAD) +=	ib_umad.o
obj-$(CONFIG_INFINIBAND_USER_ACCESS) +=	ib_uverbs.o ib_ucm.o \
					$(user_access-y)

ib_core-y :=			packer.o ud_header.o verbs.o sysfs.o \
				device.o fmr_pool.o cache.o
ib_core-$(CONFIG_INFINIBAND_USER_MEM) += umem.o

ib_mad-y :=			mad.o smi.o agent.o mad_rmpp.o

ib_sa-y :=			sa_query.o multicast.o

ib_cm-y :=			cm.o

iw_cm-y :=			iwcm.o

rdma_cm-y :=			cma.o

rdma_ucm-y :=			ucma.o

ib_addr-y :=			addr.o

ib_umad-y :=			user_mad.o

ib_ucm-y :=			ucm.o

ib_uverbs-y :=			uverbs_main.o uverbs_cmd.o uverbs_marshall.o
