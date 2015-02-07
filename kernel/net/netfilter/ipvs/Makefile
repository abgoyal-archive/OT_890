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
# Makefile for the IPVS modules on top of IPv4.
#

# IPVS transport protocol load balancing support
ip_vs_proto-objs-y :=
ip_vs_proto-objs-$(CONFIG_IP_VS_PROTO_TCP) += ip_vs_proto_tcp.o
ip_vs_proto-objs-$(CONFIG_IP_VS_PROTO_UDP) += ip_vs_proto_udp.o
ip_vs_proto-objs-$(CONFIG_IP_VS_PROTO_AH_ESP) += ip_vs_proto_ah_esp.o

ip_vs-objs :=	ip_vs_conn.o ip_vs_core.o ip_vs_ctl.o ip_vs_sched.o	   \
		ip_vs_xmit.o ip_vs_app.o ip_vs_sync.o	   		   \
		ip_vs_est.o ip_vs_proto.o 				   \
		$(ip_vs_proto-objs-y)


# IPVS core
obj-$(CONFIG_IP_VS) += ip_vs.o

# IPVS schedulers
obj-$(CONFIG_IP_VS_RR) += ip_vs_rr.o
obj-$(CONFIG_IP_VS_WRR) += ip_vs_wrr.o
obj-$(CONFIG_IP_VS_LC) += ip_vs_lc.o
obj-$(CONFIG_IP_VS_WLC) += ip_vs_wlc.o
obj-$(CONFIG_IP_VS_LBLC) += ip_vs_lblc.o
obj-$(CONFIG_IP_VS_LBLCR) += ip_vs_lblcr.o
obj-$(CONFIG_IP_VS_DH) += ip_vs_dh.o
obj-$(CONFIG_IP_VS_SH) += ip_vs_sh.o
obj-$(CONFIG_IP_VS_SED) += ip_vs_sed.o
obj-$(CONFIG_IP_VS_NQ) += ip_vs_nq.o

# IPVS application helpers
obj-$(CONFIG_IP_VS_FTP) += ip_vs_ftp.o
