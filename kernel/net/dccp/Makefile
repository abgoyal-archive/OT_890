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


obj-$(CONFIG_IP_DCCP) += dccp.o dccp_ipv4.o

dccp-y := ccid.o feat.o input.o minisocks.o options.o output.o proto.o timer.o

#
# CCID algorithms to be used by dccp.ko
#
# CCID-2 is default (RFC 4340, p. 77) and has Ack Vectors as dependency
dccp-y += ccids/ccid2.o ackvec.o
dccp-$(CONFIG_IP_DCCP_CCID3)	+= ccids/ccid3.o
dccp-$(CONFIG_IP_DCCP_TFRC_LIB) += ccids/lib/tfrc.o		\
				   ccids/lib/tfrc_equation.o	\
				   ccids/lib/packet_history.o	\
				   ccids/lib/loss_interval.o

dccp_ipv4-y := ipv4.o

# build dccp_ipv6 as module whenever either IPv6 or DCCP is a module
obj-$(subst y,$(CONFIG_IP_DCCP),$(CONFIG_IPV6)) += dccp_ipv6.o
dccp_ipv6-y := ipv6.o

obj-$(CONFIG_INET_DCCP_DIAG) += dccp_diag.o
obj-$(CONFIG_NET_DCCPPROBE) += dccp_probe.o

dccp-$(CONFIG_SYSCTL) += sysctl.o

dccp_diag-y := diag.o
dccp_probe-y := probe.o
