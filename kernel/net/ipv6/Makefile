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
# Makefile for the Linux TCP/IP (INET6) layer.
#

obj-$(CONFIG_IPV6) += ipv6.o

ipv6-objs :=	af_inet6.o anycast.o ip6_output.o ip6_input.o addrconf.o \
		addrlabel.o \
		route.o ip6_fib.o ipv6_sockglue.o ndisc.o udp.o udplite.o \
		raw.o protocol.o icmp.o mcast.o reassembly.o tcp_ipv6.o \
		exthdrs.o datagram.o ip6_flowlabel.o inet6_connection_sock.o

ipv6-$(CONFIG_SYSCTL) = sysctl_net_ipv6.o
ipv6-$(CONFIG_IPV6_MROUTE) += ip6mr.o

ipv6-$(CONFIG_XFRM) += xfrm6_policy.o xfrm6_state.o xfrm6_input.o \
	xfrm6_output.o
ipv6-$(CONFIG_NETFILTER) += netfilter.o
ipv6-$(CONFIG_IPV6_MULTIPLE_TABLES) += fib6_rules.o
ipv6-$(CONFIG_PROC_FS) += proc.o
ipv6-$(CONFIG_SYN_COOKIES) += syncookies.o

ipv6-objs += $(ipv6-y)

obj-$(CONFIG_INET6_AH) += ah6.o
obj-$(CONFIG_INET6_ESP) += esp6.o
obj-$(CONFIG_INET6_IPCOMP) += ipcomp6.o
obj-$(CONFIG_INET6_XFRM_TUNNEL) += xfrm6_tunnel.o
obj-$(CONFIG_INET6_TUNNEL) += tunnel6.o
obj-$(CONFIG_INET6_XFRM_MODE_TRANSPORT) += xfrm6_mode_transport.o
obj-$(CONFIG_INET6_XFRM_MODE_TUNNEL) += xfrm6_mode_tunnel.o
obj-$(CONFIG_INET6_XFRM_MODE_ROUTEOPTIMIZATION) += xfrm6_mode_ro.o
obj-$(CONFIG_INET6_XFRM_MODE_BEET) += xfrm6_mode_beet.o
obj-$(CONFIG_IPV6_MIP6) += mip6.o
obj-$(CONFIG_NETFILTER)	+= netfilter/

obj-$(CONFIG_IPV6_SIT) += sit.o
obj-$(CONFIG_IPV6_TUNNEL) += ip6_tunnel.o

obj-y += addrconf_core.o exthdrs_core.o

obj-$(subst m,y,$(CONFIG_IPV6)) += inet6_hashtables.o
