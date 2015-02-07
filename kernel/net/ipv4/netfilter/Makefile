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
# Makefile for the netfilter modules on top of IPv4.
#

# objects for l3 independent conntrack
nf_conntrack_ipv4-objs  :=  nf_conntrack_l3proto_ipv4.o nf_conntrack_proto_icmp.o
ifeq ($(CONFIG_NF_CONNTRACK_PROC_COMPAT),y)
ifeq ($(CONFIG_PROC_FS),y)
nf_conntrack_ipv4-objs	+= nf_conntrack_l3proto_ipv4_compat.o
endif
endif

nf_nat-objs		:= nf_nat_core.o nf_nat_helper.o nf_nat_proto_unknown.o nf_nat_proto_common.o nf_nat_proto_tcp.o nf_nat_proto_udp.o nf_nat_proto_icmp.o
iptable_nat-objs	:= nf_nat_rule.o nf_nat_standalone.o

# connection tracking
obj-$(CONFIG_NF_CONNTRACK_IPV4) += nf_conntrack_ipv4.o

obj-$(CONFIG_NF_NAT) += nf_nat.o

# defrag
obj-$(CONFIG_NF_DEFRAG_IPV4) += nf_defrag_ipv4.o

# NAT helpers (nf_conntrack)
obj-$(CONFIG_NF_NAT_AMANDA) += nf_nat_amanda.o
obj-$(CONFIG_NF_NAT_FTP) += nf_nat_ftp.o
obj-$(CONFIG_NF_NAT_H323) += nf_nat_h323.o
obj-$(CONFIG_NF_NAT_IRC) += nf_nat_irc.o
obj-$(CONFIG_NF_NAT_PPTP) += nf_nat_pptp.o
obj-$(CONFIG_NF_NAT_SIP) += nf_nat_sip.o
obj-$(CONFIG_NF_NAT_SNMP_BASIC) += nf_nat_snmp_basic.o
obj-$(CONFIG_NF_NAT_TFTP) += nf_nat_tftp.o

# NAT protocols (nf_nat)
obj-$(CONFIG_NF_NAT_PROTO_DCCP) += nf_nat_proto_dccp.o
obj-$(CONFIG_NF_NAT_PROTO_GRE) += nf_nat_proto_gre.o
obj-$(CONFIG_NF_NAT_PROTO_UDPLITE) += nf_nat_proto_udplite.o
obj-$(CONFIG_NF_NAT_PROTO_SCTP) += nf_nat_proto_sctp.o

# generic IP tables 
obj-$(CONFIG_IP_NF_IPTABLES) += ip_tables.o

# the three instances of ip_tables
obj-$(CONFIG_IP_NF_FILTER) += iptable_filter.o
obj-$(CONFIG_IP_NF_MANGLE) += iptable_mangle.o
obj-$(CONFIG_NF_NAT) += iptable_nat.o
obj-$(CONFIG_IP_NF_RAW) += iptable_raw.o
obj-$(CONFIG_IP_NF_SECURITY) += iptable_security.o

# matches
obj-$(CONFIG_IP_NF_MATCH_ADDRTYPE) += ipt_addrtype.o
obj-$(CONFIG_IP_NF_MATCH_AH) += ipt_ah.o
obj-$(CONFIG_IP_NF_MATCH_ECN) += ipt_ecn.o
obj-$(CONFIG_IP_NF_MATCH_TTL) += ipt_ttl.o

# targets
obj-$(CONFIG_IP_NF_TARGET_CLUSTERIP) += ipt_CLUSTERIP.o
obj-$(CONFIG_IP_NF_TARGET_ECN) += ipt_ECN.o
obj-$(CONFIG_IP_NF_TARGET_LOG) += ipt_LOG.o
obj-$(CONFIG_IP_NF_TARGET_MASQUERADE) += ipt_MASQUERADE.o
obj-$(CONFIG_IP_NF_TARGET_NETMAP) += ipt_NETMAP.o
obj-$(CONFIG_IP_NF_TARGET_REDIRECT) += ipt_REDIRECT.o
obj-$(CONFIG_IP_NF_TARGET_REJECT) += ipt_REJECT.o
obj-$(CONFIG_IP_NF_TARGET_TTL) += ipt_TTL.o
obj-$(CONFIG_IP_NF_TARGET_ULOG) += ipt_ULOG.o

# generic ARP tables
obj-$(CONFIG_IP_NF_ARPTABLES) += arp_tables.o
obj-$(CONFIG_IP_NF_ARP_MANGLE) += arpt_mangle.o

# just filtering instance of ARP tables for now
obj-$(CONFIG_IP_NF_ARPFILTER) += arptable_filter.o

obj-$(CONFIG_IP_NF_QUEUE) += ip_queue.o

