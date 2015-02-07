/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

/* $Date$ $RCSfile: tp.c,v $ $Revision$ */
#include "common.h"
#include "regs.h"
#include "tp.h"
#ifdef CONFIG_CHELSIO_T1_1G
#include "fpga_defs.h"
#endif

struct petp {
	adapter_t *adapter;
};

/* Pause deadlock avoidance parameters */
#define DROP_MSEC 16
#define DROP_PKTS_CNT  1

static void tp_init(adapter_t * ap, const struct tp_params *p,
		    unsigned int tp_clk)
{
	u32 val;

	if (!t1_is_asic(ap))
		return;

	val = F_TP_IN_CSPI_CPL | F_TP_IN_CSPI_CHECK_IP_CSUM |
		F_TP_IN_CSPI_CHECK_TCP_CSUM | F_TP_IN_ESPI_ETHERNET;
	if (!p->pm_size)
		val |= F_OFFLOAD_DISABLE;
	else
		val |= F_TP_IN_ESPI_CHECK_IP_CSUM | F_TP_IN_ESPI_CHECK_TCP_CSUM;
	writel(val, ap->regs + A_TP_IN_CONFIG);
	writel(F_TP_OUT_CSPI_CPL |
	       F_TP_OUT_ESPI_ETHERNET |
	       F_TP_OUT_ESPI_GENERATE_IP_CSUM |
	       F_TP_OUT_ESPI_GENERATE_TCP_CSUM, ap->regs + A_TP_OUT_CONFIG);
	writel(V_IP_TTL(64) |
	       F_PATH_MTU /* IP DF bit */  |
	       V_5TUPLE_LOOKUP(p->use_5tuple_mode) |
	       V_SYN_COOKIE_PARAMETER(29), ap->regs + A_TP_GLOBAL_CONFIG);
	/*
	 * Enable pause frame deadlock prevention.
	 */
	if (is_T2(ap) && ap->params.nports > 1) {
		u32 drop_ticks = DROP_MSEC * (tp_clk / 1000);

		writel(F_ENABLE_TX_DROP | F_ENABLE_TX_ERROR |
		       V_DROP_TICKS_CNT(drop_ticks) |
		       V_NUM_PKTS_DROPPED(DROP_PKTS_CNT),
		       ap->regs + A_TP_TX_DROP_CONFIG);
	}
}

void t1_tp_destroy(struct petp *tp)
{
	kfree(tp);
}

struct petp *__devinit t1_tp_create(adapter_t * adapter, struct tp_params *p)
{
	struct petp *tp = kzalloc(sizeof(*tp), GFP_KERNEL);

	if (!tp)
		return NULL;

	tp->adapter = adapter;

	return tp;
}

void t1_tp_intr_enable(struct petp *tp)
{
	u32 tp_intr = readl(tp->adapter->regs + A_PL_ENABLE);

#ifdef CONFIG_CHELSIO_T1_1G
	if (!t1_is_asic(tp->adapter)) {
		/* FPGA */
		writel(0xffffffff,
		       tp->adapter->regs + FPGA_TP_ADDR_INTERRUPT_ENABLE);
		writel(tp_intr | FPGA_PCIX_INTERRUPT_TP,
		       tp->adapter->regs + A_PL_ENABLE);
	} else
#endif
	{
		/* We don't use any TP interrupts */
		writel(0, tp->adapter->regs + A_TP_INT_ENABLE);
		writel(tp_intr | F_PL_INTR_TP,
		       tp->adapter->regs + A_PL_ENABLE);
	}
}

void t1_tp_intr_disable(struct petp *tp)
{
	u32 tp_intr = readl(tp->adapter->regs + A_PL_ENABLE);

#ifdef CONFIG_CHELSIO_T1_1G
	if (!t1_is_asic(tp->adapter)) {
		/* FPGA */
		writel(0, tp->adapter->regs + FPGA_TP_ADDR_INTERRUPT_ENABLE);
		writel(tp_intr & ~FPGA_PCIX_INTERRUPT_TP,
		       tp->adapter->regs + A_PL_ENABLE);
	} else
#endif
	{
		writel(0, tp->adapter->regs + A_TP_INT_ENABLE);
		writel(tp_intr & ~F_PL_INTR_TP,
		       tp->adapter->regs + A_PL_ENABLE);
	}
}

void t1_tp_intr_clear(struct petp *tp)
{
#ifdef CONFIG_CHELSIO_T1_1G
	if (!t1_is_asic(tp->adapter)) {
		writel(0xffffffff,
		       tp->adapter->regs + FPGA_TP_ADDR_INTERRUPT_CAUSE);
		writel(FPGA_PCIX_INTERRUPT_TP, tp->adapter->regs + A_PL_CAUSE);
		return;
	}
#endif
	writel(0xffffffff, tp->adapter->regs + A_TP_INT_CAUSE);
	writel(F_PL_INTR_TP, tp->adapter->regs + A_PL_CAUSE);
}

int t1_tp_intr_handler(struct petp *tp)
{
	u32 cause;

#ifdef CONFIG_CHELSIO_T1_1G
	/* FPGA doesn't support TP interrupts. */
	if (!t1_is_asic(tp->adapter))
		return 1;
#endif

	cause = readl(tp->adapter->regs + A_TP_INT_CAUSE);
	writel(cause, tp->adapter->regs + A_TP_INT_CAUSE);
	return 0;
}

static void set_csum_offload(struct petp *tp, u32 csum_bit, int enable)
{
	u32 val = readl(tp->adapter->regs + A_TP_GLOBAL_CONFIG);

	if (enable)
		val |= csum_bit;
	else
		val &= ~csum_bit;
	writel(val, tp->adapter->regs + A_TP_GLOBAL_CONFIG);
}

void t1_tp_set_ip_checksum_offload(struct petp *tp, int enable)
{
	set_csum_offload(tp, F_IP_CSUM, enable);
}

void t1_tp_set_udp_checksum_offload(struct petp *tp, int enable)
{
	set_csum_offload(tp, F_UDP_CSUM, enable);
}

void t1_tp_set_tcp_checksum_offload(struct petp *tp, int enable)
{
	set_csum_offload(tp, F_TCP_CSUM, enable);
}

/*
 * Initialize TP state.  tp_params contains initial settings for some TP
 * parameters, particularly the one-time PM and CM settings.
 */
int t1_tp_reset(struct petp *tp, struct tp_params *p, unsigned int tp_clk)
{
	adapter_t *adapter = tp->adapter;

	tp_init(adapter, p, tp_clk);
	writel(F_TP_RESET, adapter->regs +  A_TP_RESET);
	return 0;
}
