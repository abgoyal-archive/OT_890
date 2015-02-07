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

#ifndef LINUX_SSB_PRIVATE_H_
#define LINUX_SSB_PRIVATE_H_

#include <linux/ssb/ssb.h>
#include <linux/types.h>


#define PFX	"ssb: "

#ifdef CONFIG_SSB_SILENT
# define ssb_printk(fmt, x...)	do { /* nothing */ } while (0)
#else
# define ssb_printk		printk
#endif /* CONFIG_SSB_SILENT */

/* dprintk: Debugging printk; vanishes for non-debug compilation */
#ifdef CONFIG_SSB_DEBUG
# define ssb_dprintk(fmt, x...)	ssb_printk(fmt , ##x)
#else
# define ssb_dprintk(fmt, x...)	do { /* nothing */ } while (0)
#endif

#ifdef CONFIG_SSB_DEBUG
# define SSB_WARN_ON(x)		WARN_ON(x)
# define SSB_BUG_ON(x)		BUG_ON(x)
#else
static inline int __ssb_do_nothing(int x) { return x; }
# define SSB_WARN_ON(x)		__ssb_do_nothing(unlikely(!!(x)))
# define SSB_BUG_ON(x)		__ssb_do_nothing(unlikely(!!(x)))
#endif


/* pci.c */
#ifdef CONFIG_SSB_PCIHOST
extern int ssb_pci_switch_core(struct ssb_bus *bus,
			       struct ssb_device *dev);
extern int ssb_pci_switch_coreidx(struct ssb_bus *bus,
				  u8 coreidx);
extern int ssb_pci_xtal(struct ssb_bus *bus, u32 what,
			int turn_on);
extern int ssb_pci_get_invariants(struct ssb_bus *bus,
				  struct ssb_init_invariants *iv);
extern void ssb_pci_exit(struct ssb_bus *bus);
extern int ssb_pci_init(struct ssb_bus *bus);
extern const struct ssb_bus_ops ssb_pci_ops;

#else /* CONFIG_SSB_PCIHOST */

static inline int ssb_pci_switch_core(struct ssb_bus *bus,
				      struct ssb_device *dev)
{
	return 0;
}
static inline int ssb_pci_switch_coreidx(struct ssb_bus *bus,
					 u8 coreidx)
{
	return 0;
}
static inline int ssb_pci_xtal(struct ssb_bus *bus, u32 what,
			       int turn_on)
{
	return 0;
}
static inline void ssb_pci_exit(struct ssb_bus *bus)
{
}
static inline int ssb_pci_init(struct ssb_bus *bus)
{
	return 0;
}
#endif /* CONFIG_SSB_PCIHOST */


/* pcmcia.c */
#ifdef CONFIG_SSB_PCMCIAHOST
extern int ssb_pcmcia_switch_core(struct ssb_bus *bus,
				  struct ssb_device *dev);
extern int ssb_pcmcia_switch_coreidx(struct ssb_bus *bus,
				     u8 coreidx);
extern int ssb_pcmcia_switch_segment(struct ssb_bus *bus,
				     u8 seg);
extern int ssb_pcmcia_get_invariants(struct ssb_bus *bus,
				     struct ssb_init_invariants *iv);
extern int ssb_pcmcia_hardware_setup(struct ssb_bus *bus);
extern void ssb_pcmcia_exit(struct ssb_bus *bus);
extern int ssb_pcmcia_init(struct ssb_bus *bus);
extern const struct ssb_bus_ops ssb_pcmcia_ops;
#else /* CONFIG_SSB_PCMCIAHOST */
static inline int ssb_pcmcia_switch_core(struct ssb_bus *bus,
					 struct ssb_device *dev)
{
	return 0;
}
static inline int ssb_pcmcia_switch_coreidx(struct ssb_bus *bus,
					    u8 coreidx)
{
	return 0;
}
static inline int ssb_pcmcia_switch_segment(struct ssb_bus *bus,
					    u8 seg)
{
	return 0;
}
static inline int ssb_pcmcia_hardware_setup(struct ssb_bus *bus)
{
	return 0;
}
static inline void ssb_pcmcia_exit(struct ssb_bus *bus)
{
}
static inline int ssb_pcmcia_init(struct ssb_bus *bus)
{
	return 0;
}
#endif /* CONFIG_SSB_PCMCIAHOST */


/* scan.c */
extern const char *ssb_core_name(u16 coreid);
extern int ssb_bus_scan(struct ssb_bus *bus,
			unsigned long baseaddr);
extern void ssb_iounmap(struct ssb_bus *ssb);


/* sprom.c */
extern
ssize_t ssb_attr_sprom_show(struct ssb_bus *bus, char *buf,
			    int (*sprom_read)(struct ssb_bus *bus, u16 *sprom));
extern
ssize_t ssb_attr_sprom_store(struct ssb_bus *bus,
			     const char *buf, size_t count,
			     int (*sprom_check_crc)(const u16 *sprom, size_t size),
			     int (*sprom_write)(struct ssb_bus *bus, const u16 *sprom));


/* core.c */
extern u32 ssb_calc_clock_rate(u32 plltype, u32 n, u32 m);
extern int ssb_devices_freeze(struct ssb_bus *bus);
extern int ssb_devices_thaw(struct ssb_bus *bus);
extern struct ssb_bus *ssb_pci_dev_to_bus(struct pci_dev *pdev);
int ssb_for_each_bus_call(unsigned long data,
			  int (*func)(struct ssb_bus *bus, unsigned long data));
extern struct ssb_bus *ssb_pcmcia_dev_to_bus(struct pcmcia_device *pdev);


/* b43_pci_bridge.c */
#ifdef CONFIG_SSB_B43_PCI_BRIDGE
extern int __init b43_pci_ssb_bridge_init(void);
extern void __exit b43_pci_ssb_bridge_exit(void);
#else /* CONFIG_SSB_B43_PCI_BRIDGR */
static inline int b43_pci_ssb_bridge_init(void)
{
	return 0;
}
static inline void b43_pci_ssb_bridge_exit(void)
{
}
#endif /* CONFIG_SSB_PCIHOST */

#endif /* LINUX_SSB_PRIVATE_H_ */
