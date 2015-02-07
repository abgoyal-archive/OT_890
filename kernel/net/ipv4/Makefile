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
# Makefile for the Linux TCP/IP (INET) layer.
#

obj-y     := route.o inetpeer.o protocol.o \
	     ip_input.o ip_fragment.o ip_forward.o ip_options.o \
	     ip_output.o ip_sockglue.o inet_hashtables.o \
	     inet_timewait_sock.o inet_connection_sock.o \
	     tcp.o tcp_input.o tcp_output.o tcp_timer.o tcp_ipv4.o \
	     tcp_minisocks.o tcp_cong.o \
	     datagram.o raw.o udp.o udplite.o \
	     arp.o icmp.o devinet.o af_inet.o  igmp.o \
	     fib_frontend.o fib_semantics.o \
	     inet_fragment.o

obj-$(CONFIG_SYSCTL) += sysctl_net_ipv4.o
obj-$(CONFIG_SYSFS) += sysfs_net_ipv4.o
obj-$(CONFIG_IP_FIB_HASH) += fib_hash.o
obj-$(CONFIG_IP_FIB_TRIE) += fib_trie.o
obj-$(CONFIG_PROC_FS) += proc.o
obj-$(CONFIG_IP_MULTIPLE_TABLES) += fib_rules.o
obj-$(CONFIG_IP_MROUTE) += ipmr.o
obj-$(CONFIG_NET_IPIP) += ipip.o
obj-$(CONFIG_NET_IPGRE) += ip_gre.o
obj-$(CONFIG_SYN_COOKIES) += syncookies.o
obj-$(CONFIG_INET_AH) += ah4.o
obj-$(CONFIG_INET_ESP) += esp4.o
obj-$(CONFIG_INET_IPCOMP) += ipcomp.o
obj-$(CONFIG_INET_XFRM_TUNNEL) += xfrm4_tunnel.o
obj-$(CONFIG_INET_XFRM_MODE_BEET) += xfrm4_mode_beet.o
obj-$(CONFIG_INET_LRO) += inet_lro.o
obj-$(CONFIG_INET_TUNNEL) += tunnel4.o
obj-$(CONFIG_INET_XFRM_MODE_TRANSPORT) += xfrm4_mode_transport.o
obj-$(CONFIG_INET_XFRM_MODE_TUNNEL) += xfrm4_mode_tunnel.o
obj-$(CONFIG_IP_PNP) += ipconfig.o
obj-$(CONFIG_NETFILTER)	+= netfilter.o netfilter/
obj-$(CONFIG_INET_DIAG) += inet_diag.o 
obj-$(CONFIG_INET_TCP_DIAG) += tcp_diag.o
obj-$(CONFIG_NET_TCPPROBE) += tcp_probe.o
obj-$(CONFIG_TCP_CONG_BIC) += tcp_bic.o
obj-$(CONFIG_TCP_CONG_CUBIC) += tcp_cubic.o
obj-$(CONFIG_TCP_CONG_WESTWOOD) += tcp_westwood.o
obj-$(CONFIG_TCP_CONG_HSTCP) += tcp_highspeed.o
obj-$(CONFIG_TCP_CONG_HYBLA) += tcp_hybla.o
obj-$(CONFIG_TCP_CONG_HTCP) += tcp_htcp.o
obj-$(CONFIG_TCP_CONG_VEGAS) += tcp_vegas.o
obj-$(CONFIG_TCP_CONG_VENO) += tcp_veno.o
obj-$(CONFIG_TCP_CONG_SCALABLE) += tcp_scalable.o
obj-$(CONFIG_TCP_CONG_LP) += tcp_lp.o
obj-$(CONFIG_TCP_CONG_YEAH) += tcp_yeah.o
obj-$(CONFIG_TCP_CONG_ILLINOIS) += tcp_illinois.o
obj-$(CONFIG_NETLABEL) += cipso_ipv4.o

obj-$(CONFIG_XFRM) += xfrm4_policy.o xfrm4_state.o xfrm4_input.o \
		      xfrm4_output.o
