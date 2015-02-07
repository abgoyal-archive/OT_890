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

/* USB OTG (On The Go) defines */
/*
 *
 * These APIs may be used between USB controllers.  USB device drivers
 * (for either host or peripheral roles) don't use these calls; they
 * continue to use just usb_device and usb_gadget.
 */

#ifndef __LINUX_USB_OTG_H
#define __LINUX_USB_OTG_H

/* OTG defines lots of enumeration states before device reset */
enum usb_otg_state {
	OTG_STATE_UNDEFINED = 0,

	/* single-role peripheral, and dual-role default-b */
	OTG_STATE_B_IDLE,
	OTG_STATE_B_SRP_INIT,
	OTG_STATE_B_PERIPHERAL,

	/* extra dual-role default-b states */
	OTG_STATE_B_WAIT_ACON,
	OTG_STATE_B_HOST,

	/* dual-role default-a */
	OTG_STATE_A_IDLE,
	OTG_STATE_A_WAIT_VRISE,
	OTG_STATE_A_WAIT_BCON,
	OTG_STATE_A_HOST,
	OTG_STATE_A_SUSPEND,
	OTG_STATE_A_PERIPHERAL,
	OTG_STATE_A_WAIT_VFALL,
	OTG_STATE_A_VBUS_ERR,
};

/*
 * the otg driver needs to interact with both device side and host side
 * usb controllers.  it decides which controller is active at a given
 * moment, using the transceiver, ID signal, HNP and sometimes static
 * configuration information (including "board isn't wired for otg").
 */
struct otg_transceiver {
	struct device		*dev;
	const char		*label;

	u8			default_a;
	enum usb_otg_state	state;

	struct usb_bus		*host;
	struct usb_gadget	*gadget;

	/* to pass extra port status to the root hub */
	u16			port_status;
	u16			port_change;

	/* bind/unbind the host controller */
	int	(*set_host)(struct otg_transceiver *otg,
				struct usb_bus *host);

	/* bind/unbind the peripheral controller */
	int	(*set_peripheral)(struct otg_transceiver *otg,
				struct usb_gadget *gadget);

	/* effective for B devices, ignored for A-peripheral */
	int	(*set_power)(struct otg_transceiver *otg,
				unsigned mA);

	/* for non-OTG B devices: set transceiver into suspend mode */
	int	(*set_suspend)(struct otg_transceiver *otg,
				int suspend);

	/* for B devices only:  start session with A-Host */
	int	(*start_srp)(struct otg_transceiver *otg);

	/* start or continue HNP role switch */
	int	(*start_hnp)(struct otg_transceiver *otg);

};


/* for board-specific init logic */
extern int otg_set_transceiver(struct otg_transceiver *);


/* for usb host and peripheral controller drivers */
extern struct otg_transceiver *otg_get_transceiver(void);
extern void otg_put_transceiver(struct otg_transceiver *);

static inline int
otg_start_hnp(struct otg_transceiver *otg)
{
	return otg->start_hnp(otg);
}


/* for HCDs */
static inline int
otg_set_host(struct otg_transceiver *otg, struct usb_bus *host)
{
	return otg->set_host(otg, host);
}


/* for usb peripheral controller drivers */
static inline int
otg_set_peripheral(struct otg_transceiver *otg, struct usb_gadget *periph)
{
	return otg->set_peripheral(otg, periph);
}

static inline int
otg_set_power(struct otg_transceiver *otg, unsigned mA)
{
	return otg->set_power(otg, mA);
}

static inline int
otg_set_suspend(struct otg_transceiver *otg, int suspend)
{
	if (otg->set_suspend != NULL)
		return otg->set_suspend(otg, suspend);
	else
		return 0;
}

static inline int
otg_start_srp(struct otg_transceiver *otg)
{
	return otg->start_srp(otg);
}


/* for OTG controller drivers (and maybe other stuff) */
extern int usb_bus_start_enum(struct usb_bus *bus, unsigned port_num);

#endif /* __LINUX_USB_OTG_H */
